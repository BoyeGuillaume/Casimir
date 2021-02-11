#include "datatype.hpp"

#include <unordered_map>
#include <utility>

#include "../utilities/exception.hpp"
#include "../core/private-context.hpp"

namespace Casimir::framework {
    
    struct __DataTypeHandle {
        EDataType dataType;
        cuint size;
        cuint offset = 0;
        std::unordered_map<utilities::Uuid, DataType> parametersByUuid;
        
        CASIMIR_EXPORT __DataTypeHandle(EDataType dtype, cuint size, std::unordered_map<utilities::Uuid, DataType> parameters)
        : dataType(dtype),
        size(size),
        parametersByUuid(std::move(parameters)) {}
        
        CASIMIR_EXPORT __DataTypeHandle(EDataType dtype, cuint size)
        : dataType(dtype),
        size(size) {}
    };
    
    CASIMIR_EXPORT cuint sizeOf(EDataType dtype) {
        switch (dtype) {
            case EDataType::Float:
            case EDataType::Int32:
                return 4;
            case EDataType::Double:
            case EDataType::Int64:
                return 8;
            default:
                return 0;
        }
    }
    
    CASIMIR_EXPORT DataType::DataType(CasimirContext ctx, std::shared_ptr<__DataTypeHandle> handle)
            : ContextualObject(ctx), m_handle(std::move(handle)) {}
    
    CASIMIR_EXPORT DataType::~DataType() = default;
    
    CASIMIR_EXPORT cuint DataType::sizeOf() const {
        return m_handle->size;
    }
    
    CASIMIR_EXPORT EDataType DataType::dtype() const {
        return m_handle->dataType;
    }
    
    CASIMIR_EXPORT bool DataType::has(const utilities::Uuid& key) const {
        return (m_handle->dataType == EDataType::Structure) &&
               (m_handle->parametersByUuid.find(key) != m_handle->parametersByUuid.end());
    }
    
    CASIMIR_EXPORT utilities::Optional<DataType> DataType::at(const utilities::Uuid& key) const {
        if(m_handle->dataType != EDataType::Structure) {
            return utilities::Optional<DataType>::empty();
        }
        
        // Find out the parameters that correspond to the given key
        auto it = m_handle->parametersByUuid.find(key);
        return (it == m_handle->parametersByUuid.end()) ?
               utilities::Optional<DataType>::empty() :
               utilities::Optional<DataType>::of(it->second);
    }
    
    CASIMIR_EXPORT cuint DataType::offset() const {
        return m_handle->offset;
    }
    
    CASIMIR_EXPORT bool DataType::operator==(const DataType& dataType) const {
        if(dataType.m_handle == m_handle) return true;
        if(dataType.dtype() != dtype() || dataType.sizeOf() != sizeOf()) {
            return false;
        }
        if(!dataType.isComplexType()) {
            return true;
        }
        if(dataType.m_handle->parametersByUuid.size() != m_handle->parametersByUuid.size()) {
            return false;
        }
        for(const auto& pair : dataType.m_handle->parametersByUuid) {
            auto it = m_handle->parametersByUuid.find(pair.first);
            // Notice the `!=` operator in here is a recursion BUT we already known that the depth is at most 1
            // Indeed each children of an DataType is a basic type and therefore we know for sure that the second call
            // to this methods won't reach this line so the usage to a recursion here is totally fine
            if(it == m_handle->parametersByUuid.end() || it->second != pair.second) {
                return false;
            }
        }
        return  true;
    }
    
    
    CASIMIR_EXPORT DataTypeBuilder::DataTypeBuilder(CasimirContext ctx)
        : m_handle(std::make_shared<__DataTypeHandle>(EDataType::None, 0)), m_builderDisabled(std::make_shared<bool>(false)),
        m_ctx(ctx) {}
    
    CASIMIR_EXPORT DataTypeBuilder& DataTypeBuilder::setType(EDataType dtype) {
#ifdef CASIMIR_SAFE_CHECK
        if(*m_builderDisabled) {
            CASIMIR_THROW_EXCEPTION("InvalidUsage", "This builder has been destruct and cannot be used");
        }
#endif
        m_handle->dataType = dtype;
        return *this;
    }
    
    CASIMIR_EXPORT ComplexDataTypeBuilder DataTypeBuilder::asStructure() {
#ifdef CASIMIR_SAFE_CHECK
        if(*m_builderDisabled) {
            CASIMIR_THROW_EXCEPTION("InvalidUsage", "This builder has been destruct and cannot be used");
        }
#endif
        *m_builderDisabled = true; // Disable the Builder
        return ComplexDataTypeBuilder(m_ctx, m_handle);
    }
    
    CASIMIR_EXPORT DataType DataTypeBuilder::create() {
#ifdef CASIMIR_SAFE_CHECK
        if(*m_builderDisabled) {
            CASIMIR_THROW_EXCEPTION("InvalidUsage", "This builder has been destruct and cannot be used");
        }
        if(m_handle->dataType == EDataType::Structure || m_handle->dataType == EDataType::None) {
            CASIMIR_THROW_EXCEPTION("InvalidArgument", "Cannot create an basic parameters of type None or structure");
        }
#endif
        m_ctx->logger(PrivateLogging::Note) << "Register new basic-type at " << m_handle.get();
        *m_builderDisabled = true; // Disable the Builder
        m_handle->size = sizeOf(m_handle->dataType);
        return DataType(m_ctx, m_handle);
    }
    
    
    CASIMIR_EXPORT ComplexDataTypeBuilder::ComplexDataTypeBuilder(CasimirContext ctx, std::shared_ptr<__DataTypeHandle> handle)
            : m_handle(std::move(handle)), m_builderDisabled(std::make_shared<bool>(false)),
              m_currentOffset(std::make_shared<cuint>(0)),
              m_ctx(ctx) {}
    
    CASIMIR_EXPORT ComplexDataTypeBuilder& ComplexDataTypeBuilder::registerSubType(const utilities::Uuid& uuid, EDataType dtype) {
#ifdef CASIMIR_SAFE_CHECK
        if(*m_builderDisabled) {
            CASIMIR_THROW_EXCEPTION("InvalidUsage", "This builder has been destruct and cannot be used");
        }
        if(dtype == EDataType::Structure || dtype == EDataType::None) {
            CASIMIR_THROW_EXCEPTION("InvalidArgument", "Cannot create an sub-parameters of type None or structure");
        }
        if(m_handle->parametersByUuid.find(uuid) != m_handle->parametersByUuid.end()) {
            CASIMIR_THROW_EXCEPTION("InvalidArgument", "The given uuid already correspond to an known parameters");
        }
#endif
        std::shared_ptr<__DataTypeHandle> handle = std::make_shared<__DataTypeHandle>(dtype, sizeOf(dtype));
        handle->offset = *m_currentOffset;
        m_handle->parametersByUuid.insert(std::make_pair(uuid, DataType(m_ctx, handle)));
        *m_currentOffset += sizeOf(dtype);
        return *this;
    }
    
    CASIMIR_EXPORT DataType ComplexDataTypeBuilder::create() {
#ifdef CASIMIR_SAFE_CHECK
        if(*m_builderDisabled) {
            CASIMIR_THROW_EXCEPTION("InvalidUsage", "This builder has been destruct and cannot be used");
        }
#endif
        m_ctx->logger(PrivateLogging::Note) << "Register new complex type at " << m_handle.get();
        *m_builderDisabled = true; // Disable the Builder
        m_handle->dataType = EDataType::Structure;
        m_handle->size = *m_currentOffset;
        return DataType(m_ctx, m_handle);
    }
}
