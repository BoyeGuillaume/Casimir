#include "datachunk.hpp"

namespace Casimir::framework {
    
    using namespace literals;
    using namespace utilities;
    
    CASIMIR_EXPORT DataChunk::DataChunk(CasimirContext ctx, RawData* rawData, const DataType& dtype, cuint offset, cuint length,
                         cuint slice)
    : ContextualObject(ctx),
    m_rawData(rawData),
    m_dtype(dtype),
    m_offset(offset),
    m_length(length),
    m_slice(0) {
#ifdef CASIMIR_SAFE_CHECK
        if(m_offset + m_length * (dtype.sizeOf() + m_slice) > rawData->size() || m_length * (dtype.sizeOf() + m_slice) > rawData->size()) {
            CASIMIR_THROW_EXCEPTION("IndexOutOfRange", "Cannot create the given DataChunk as the specified range"
                                                       "isn't contains in the range");
        }
#endif
    }
    
    CASIMIR_EXPORT utilities::String DataChunk::toString() const {
        return "DataChunk(length=" + String::toString((cint) m_length)
        + ", slice=" + String::toString((cint) m_slice)
        + ", offset=" + String::toString((cint) m_offset) + ")";
    }
    
    CASIMIR_EXPORT void* DataChunk::at(cuint position) const {
#ifdef CASIMIR_SAFE_CHECK
        if(position >= m_length) {
            CASIMIR_THROW_EXCEPTION("IndexOutOfRange", "The given index isn't contains in the current chunk of data");
        }
#endif
        // Evaluate the position
        return offsetOf(m_rawData->data(), m_offset + position * (m_length + m_slice));
    }
    
    CASIMIR_EXPORT void* DataChunk::at(cuint position, const utilities::Uuid& parameters) const {
#ifdef CASIMIR_SAFE_CHECK
        if(position >= m_length) {
            CASIMIR_THROW_EXCEPTION("IndexOutOfRange", "The given index isn't contains in the current chunk of data");
        }
#endif
        // Evaluator the position
        return offsetOf(m_rawData->data(), m_offset + position * (m_length + m_slice) + m_dtype.at(parameters).get().offset());
    }
    
    CASIMIR_EXPORT void DataChunk::copy(DataChunk* dest, const DataChunk* source) {
        // Trivial case not handle
        if(dest == source) return;
        
        // First assert the copy is possible
#ifdef CASIMIR_SAFE_CHECK
        if(dest->length() != source->length() || dest->dtype() != source->dtype()) {
            CASIMIR_THROW_EXCEPTION("InvalidArgument", "Cannot copy that doesn't share the same length and size");
        }
#endif

        // Perform the copy
        // If slice is equal to 0 IN BOTH CASES the copy is trivial
        if(dest->slice() == 0 && source->slice() == 0) {
            AbstractAllocator::copy(dest->rawData(), source->rawData(), dest->length() * dest->dtype().sizeOf(), source->m_offset, dest->m_offset);
        }
        // In case the slice of one isn't equal to 0 then the copy take more times
        else {
            for(cuint blockId = 0; blockId < dest->length(); ++blockId) {
                const cuint offsetSource = source->m_offset + blockId * (source->m_length + source->m_slice);
                const cuint offsetDestination = dest->m_offset + blockId * (dest->m_length + dest->m_slice);
                AbstractAllocator::copy(dest->rawData(), source->rawData(), dest->dtype().sizeOf(), offsetSource, offsetDestination);
            }
        }
    }
    
    
    CASIMIR_EXPORT void* offsetOf(volatile void* data, cuint offset) {
        return (ubyte*) data + offset;
    }
}
