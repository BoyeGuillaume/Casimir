#ifndef CASIMIR_DATACHUNK_HPP_
#define CASIMIR_DATACHUNK_HPP_

#include "../casimir.hpp"
#include "datatype.hpp"
#include "interface.hpp"
#include "allocator.hpp"
#include "indexable.hpp"
#include "contextual_object.hpp"
#include "../utilities/uuid.hpp"
#include "../utilities/string_serializable.hpp"

namespace Casimir::framework {

    /**
     * @brief DataChunk defines a chunk with a length a type and a offset. It is used to introduce the concept
     * of type into a data. It is also responsible of the inline mapping of the data structure into a inline vector
     * It can be allocated and free if not constructed directly from an existing RawData
     */
    class DataChunk : public ContextualObject, public utilities::StringSerializable {
        CASIMIR_DISABLE_COPY_MOVE(DataChunk)
    private:
        AbstractAllocator* m_allocator;
        RawData* m_rawData;
        bool m_ownData;
        DataType m_dtype;
        cuint m_offset;
        cuint m_length; // Number of elements (NOT BYTES) contained in the chunk
        cuint m_slice;  // slice in BYTES between elements
        
    public:
        /**
         * @brief Default DataChunk constructor that take only an allocator and a type as well as a length. The resulting
         * chunk won't be allocated but will own its data
         * @param ctx Because the DataChunk is a ContextualObject it does require a context to work properly
         * @param allocator The allocator that will be used to allocate / free the data
         * @param dtype The type of the data allocated
         * @param length The number of element we will have in the output result
         */
        CASIMIR_EXPORT DataChunk(CasimirContext ctx, AbstractAllocator* allocator, const DataType& dtype, cuint length);
        
        /**
         * @brief Default constructor of the DataChunk object (the resulting chunk WILL NOT own the data and therefore
         * won't be able to free it's data)
         * @param ctx Because the DataChunk is a Contextual object it does require a context to work properly
         * @param rawData The raw data used to instantiate the DataChunk
         * @param dtype The type that will be linked to the given raw Data
         * @param offset The offset from the start of the data
         * @param length The number (in elements not in bytes) of elements of type `dtype` contains in the chunk
         * @param slice A slice is defined as the offset between two consecutive elements in bytes
         * @throw utilities::Exception if the given sequence isn't contains in the RawData
         */
        CASIMIR_EXPORT DataChunk(CasimirContext ctx, RawData* rawData, const DataType& dtype, cuint offset, cuint length, cuint slice);
        
        /**
         * @brief Default destructor
         */
        CASIMIR_EXPORT virtual ~DataChunk();
        
        /**
         * @brief Constructor of the DataChunk object
         * @param ctx Because the DataChunk is a Contextual object it does require a context to work properly
         * @param rawData The raw data used to instantiate the DataChunk
         * @param dtype The type that will be linked to the given raw Data
         * @param offset The offset from the start of the data
         * @param length The number (in elements not in bytes) of elements of type `dtype` contains in the chunk
         * @throw utilities::Exception if the given sequence isn't contains in the RawData
         */
        inline DataChunk(CasimirContext ctx, RawData* rawData, const DataType& dtype, cuint offset, cuint length)
        : DataChunk(ctx, rawData, dtype, offset, length, 0) {}
        
        /**
         * @brief Convert the current object to a human-readable String
         * @return A utilities::String that describe the current state of the object
         */
        CASIMIR_EXPORT utilities::String toString() const override;
        
        /**
         * @brief Return the pointer for a given position
         * @param position the index of the elements we want to retrieve
         * @throw utilities::Exception if the given `position` isn't contains in the chunk
         * the methods DataChunk::at(cuint, const utilities::Uuid&)
         * @return The pointer at the given index if data is allocated nullptr otherwise
         */
        CASIMIR_EXPORT void* at(cuint position) const;
        
        /**
         * @brief Return the pointer for a given position and a given parameters
         * @param position the index of the elements we want to retrviee
         * @param parameters the field utilities::Uuid that will be retrieved
         * @throw utilities::Exception if the given `position` isn't contains in the chunk
         * @throw utilities::Exception if the given `parameters` isn't found in the current dtype
         * @return The pointer at the given index & parameters if data is allocated nullptr otherwise
         */
        CASIMIR_EXPORT void* at(cuint position, const utilities::Uuid& parameters) const;
        
        /**
         * @brief Allocate the internal data to the required size. Produce a warning if the data is already allocated
         */
        CASIMIR_EXPORT void malloc();
        
        /**
         * @brief Free the internal data. Produce a warning if no data has been found
         * @throw utilities::Exception if the current chunk doesn't own the data
         */
        CASIMIR_EXPORT void free();
        
        /**
         * @brief Return the size of the given parameters
         * @param parameters the field utilities::Uuid that will be retrieved
         * @throw utilities::Exception if the given `parameters` isn't found in the current dtype
         * @return The size of the given field
         */
        inline cuint sizeOf(const utilities::Uuid& parameters) const {
            return m_dtype.at(parameters).get().sizeOf();
        }
        
        /**
         * @brief Return the type of the given parameters
         * @param parameters the field utilities::Uuid that will be retrieved
         * @throw utilities::Exception if the given `parameters` isn't found in the current dtype
         * @return The type of the given field
         */
        inline EDataType typeOf(const utilities::Uuid& parameters) const {
            return m_dtype.at(parameters).get().dtype();
        }
        
        /**
         * @brief A pointer to the raw Data used by the current chunk
         * @return The rawData pointer
         */
        inline RawData* rawData() const {
            return m_rawData;
        }
        
        /**
         * @brief Determine whether or not the current data chunk is allocated or not
         * @return Whether or not the current chunk is allocated
         */
        inline bool isAllocated() const {
            return m_rawData != nullptr;
        }
        
        /**
         * @brief The data hold by the current chunk as been allocated by the chunk and therefore the chunk
         * own the data. (This is not the case if the current instance has been created using the constructor with
         * RawData
         * @return Whether or not the current chunk own the rawData
         */
        inline bool doesOwnData() const {
            return m_ownData;
        }
        
        /**
         * @brief The type associate to the current chunk
         * @return The type of the current chunk
         */
        inline DataType dtype() const {
            return m_dtype;
        }
        
        /**
         * @brief Return the offset between the current chunk and the start of the rawData
         * @return the offset between the start
         */
        inline cuint offset() const {
            return m_offset;
        }
        
        /**
         * @brief The number of elements contains in the current Chunk
         * @return The number of elements contains in the current instance
         */
        inline cuint length() const {
            return m_length;
        }
        
        /**
         * @brief The offset between two consecutive elements in the dataObject
         * @return The slice associate to the current chunk
         */
        inline cuint slice() const {
            return m_slice;
        }
    
    
        /**
         * @brief Copy data from a `source` DataChunk to a destination DataChunk
         * @param dest The destination DataChunk
         * @param source The source DataChunk
         * @throw utilities::Exception if one of the DataChunk isn't allocated or if both doesn't share the same size and type
         */
        CASIMIR_EXPORT static void copy(DataChunk* dest, const DataChunk* source);
    
    };
    
    /**
     * @brief Offset `data` of a given constant (in bytes)
     * @param data The data to be offset
     * @param offset The offset to be applied to the dat
     * @return The resulting pointer offset of `offset` from the source data
     */
    CASIMIR_EXPORT void* offsetOf(volatile void* data, cuint offset);
    
}

#endif
