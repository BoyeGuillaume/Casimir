#include "datachunk.hpp"
#include "../core/private-context.hpp"

namespace Casimir::framework {
    
    using namespace literals;
    using namespace utilities;
    
    CASIMIR_EXPORT DataChunk::DataChunk(CasimirContext ctx, AbstractAllocator* allocator, const DataType& dtype,
                                        cuint length)
    : ContextualObject(ctx),
    m_allocator(allocator),
    m_rawData(nullptr),
    m_ownData(true),
    m_dtype(dtype),
    m_offset(0),
    m_length(length),
    m_slice(0) {
#ifdef CASIMIR_SAFE_CHECK
        if (ctx != allocator->ctx()) {
            CASIMIR_THROW_EXCEPTION("InvalidArgument", "The two given context are incompatible");
        }
        if (m_length == 0) {
            CASIMIR_THROW_EXCEPTION("InvalidArgument", "Cannot allocate data of length 0");
        }
#endif
    }
    
    CASIMIR_EXPORT DataChunk::DataChunk(CasimirContext ctx, RawData* rawData, const DataType& dtype, cuint offset,
                                        cuint length, cuint slice)
    : ContextualObject(ctx),
    m_allocator(rawData->allocator()),
    m_rawData(rawData),
    m_ownData(false),
    m_dtype(dtype),
    m_offset(offset),
    m_length(length),
    m_slice(0) {
#ifdef CASIMIR_SAFE_CHECK
        if (m_offset + m_length * (dtype.sizeOf() + m_slice) > rawData->size() ||
            m_length * (dtype.sizeOf() + m_slice) > rawData->size()) {
            CASIMIR_THROW_EXCEPTION("IndexOutOfRange", "Cannot create the given DataChunk as the specified range"
                                                       "isn't contains in the range");
        }
        if (ctx != rawData->ctx()) {
            CASIMIR_THROW_EXCEPTION("InvalidArgument", "The two given context are incompatible");
        }
#endif
    }
    
    CASIMIR_EXPORT utilities::String DataChunk::toString() const {
        return "DataChunk(length=" + String::toString((cint) m_length) + ", slice=" + String::toString((cint) m_slice) +
               ", offset=" + String::toString((cint) m_offset) + ")";
    }
    
    CASIMIR_EXPORT Optional<void*> DataChunk::at(cuint position) const {
#ifdef CASIMIR_SAFE_CHECK
        if (position >= m_length) {
            CASIMIR_THROW_EXCEPTION("IndexOutOfRange", "The given index isn't contains in the current chunk of data");
        }
#endif
        // Evaluate the position
        return (m_rawData == nullptr) ? Optional<void*>::empty() : Optional<void*>::of(
                offsetOf(m_rawData->data(), m_offset + position * (m_length + m_slice)));
    }
    
    CASIMIR_EXPORT Optional<void*> DataChunk::at(cuint position, const utilities::Uuid& parameters) const {
#ifdef CASIMIR_SAFE_CHECK
        if (position >= m_length) {
            CASIMIR_THROW_EXCEPTION("IndexOutOfRange", "The given index isn't contains in the current chunk of data");
        }
#endif
        // Evaluator the position
        return (m_rawData == nullptr) ?
            Optional<void*>::empty() :
            Optional<void*>::of(offsetOf(m_rawData->data(),m_offset + position * (m_length + m_slice) +
                                                                                                 m_dtype.at(parameters).get().offset()));
    }
    
    CASIMIR_EXPORT void DataChunk::copy(DataChunk* dest, const DataChunk* source) {
        // Trivial case not handle
        if (dest == source) return;
        
        // First assert the copy is possible
#ifdef CASIMIR_SAFE_CHECK
        if (dest->length() != source->length() || dest->dtype() != source->dtype()) {
            CASIMIR_THROW_EXCEPTION("InvalidArgument",
                                    "Cannot copy if both chunks doesn't share the same length and size");
        }
        if(dest->m_rawData == nullptr || source->m_rawData == nullptr) {
            CASIMIR_THROW_EXCEPTION("InvalidArgument", "Cannot copy from or to a non-allocated dataChunk");
        }
#endif
        
        // Perform the copy
        // If slice is equal to 0 IN BOTH CASES the copy is trivial
        if (dest->slice() == 0 && source->slice() == 0) {
            AbstractAllocator::copy(dest->rawData(), source->rawData(), dest->length() * dest->dtype().sizeOf(),
                                    dest->m_offset, source->m_offset);
        }
        
        // In case the slice of one isn't equal to 0 then the copy take more times
        else {
            for (cuint blockId = 0; blockId < dest->length(); ++blockId) {
                const cuint offsetSource = source->m_offset + blockId * (source->m_length + source->m_slice);
                const cuint offsetDestination = dest->m_offset + blockId * (dest->m_length + dest->m_slice);
                AbstractAllocator::copy(dest->rawData(), source->rawData(), dest->dtype().sizeOf(), offsetDestination,
                                        offsetSource);
            }
        }
    }
    
    CASIMIR_EXPORT void DataChunk::malloc() {
        // First detect if such operation is possible
        if(isAllocated()) {
            ctx()->logger(PrivateLogging::Warning) << "Cannot allocate the chunk at " << this << " as the chunk is already allocated";
            return;
        }
        
        // Then allocate
        m_rawData = m_allocator->allocate(m_length * m_dtype.sizeOf());
        m_ownData = true;
    }
    
    CASIMIR_EXPORT void DataChunk::free() {
        // Detect if such operation is possible
        if(!isAllocated()) {
            ctx()->logger(PrivateLogging::Warning) << "Cannot free the chunk at " << this << " as the chunk is already free";
            return;
        }
        if(!m_ownData) {
            CASIMIR_THROW_EXCEPTION("InvalidOperation", "Cannot free the data as the current chunk doesn't actually own the data");
        }
        
        // Free the data
        delete m_rawData;
        m_rawData = nullptr;
    }
    
    CASIMIR_EXPORT DataChunk::~DataChunk() {
        if(isAllocated() && m_ownData) {
            free();
        }
    }
    
    
    CASIMIR_EXPORT void* offsetOf(volatile void* data, cuint offset) {
        return (ubyte*) data + offset;
    }
}
