#include "datatype.hpp"

#include <unordered_map>

#include "../utilities/exception.hpp"
#include "../core/private-context.hpp"

namespace Casimir::framework {
    
    struct __DataTypeHandle {
        EDataType dataType;
        cuint size;
        std::unordered_map<utilities::Uuid, DataType::Parameters> parametersByUuid;
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
    
    CASIMIR_EXPORT utilities::Optional<DataType::Parameters> DataType::at(const utilities::Uuid& key) const {
        if(m_handle->dataType != EDataType::Structure) {
            return utilities::Optional<DataType::Parameters>::empty();
        }
        
        // Find out the parameters that correspond to the given key
        auto it = m_handle->parametersByUuid.find(key);
        return (it == m_handle->parametersByUuid.end()) ?
               utilities::Optional<DataType::Parameters>::empty() :
               utilities::Optional<DataType::Parameters>::of(it->second);
    }
    
    
    CASIMIR_EXPORT DataTypeBuilder::DataTypeBuilder()
        : m_handle(std::make_shared<__DataTypeHandle>()), m_builderDisabled(std::make_shared<bool>(false)) {
        m_handle->dataType = EDataType::None;
    }
    
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
        return ComplexDataTypeBuilder(m_handle);
    }
    
    CASIMIR_EXPORT DataType DataTypeBuilder::create(CasimirContext ctx) {
#ifdef CASIMIR_SAFE_CHECK
        if(*m_builderDisabled) {
            CASIMIR_THROW_EXCEPTION("InvalidUsage", "This builder has been destruct and cannot be used");
        }
        if(m_handle->dataType == EDataType::Structure || m_handle->dataType == EDataType::None) {
            CASIMIR_THROW_EXCEPTION("InvalidArgument", "Cannot create an basic parameters of type None or structure");
        }
#endif
        ctx->logger(PrivateLogging::Note) << "Register new basic-type at " << m_handle.get();
        *m_builderDisabled = true; // Disable the Builder
        m_handle->size = sizeOf(m_handle->dataType);
        return DataType(ctx, m_handle);
    }
    
    
    CASIMIR_EXPORT ComplexDataTypeBuilder::ComplexDataTypeBuilder(std::shared_ptr<__DataTypeHandle> handle)
            : m_handle(std::move(handle)), m_builderDisabled(std::make_shared<bool>(false)),
              m_currentOffset(std::make_shared<cuint>(0)){}
    
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
        DataType::Parameters parameters{};
        parameters.dtype = dtype;
        parameters.offset = *m_currentOffset;
        parameters.size = sizeOf(dtype);
        m_handle->parametersByUuid.insert(std::make_pair(uuid, parameters));
        *m_currentOffset += sizeOf(dtype);
        return *this;
    }
    
    CASIMIR_EXPORT DataType ComplexDataTypeBuilder::create(CasimirContext ctx) {
#ifdef CASIMIR_SAFE_CHECK
        if(*m_builderDisabled) {
            CASIMIR_THROW_EXCEPTION("InvalidUsage", "This builder has been destruct and cannot be used");
        }
#endif
        ctx->logger(PrivateLogging::Note) << "Register new complex type at " << m_handle.get();
        *m_builderDisabled = true; // Disable the Builder
        m_handle->dataType = EDataType::Structure;
        m_handle->size = *m_currentOffset;
        return DataType(ctx, m_handle);
    }
}
