#ifndef CASIMIR_ALLOCATOR_HPP_
#define CASIMIR_ALLOCATOR_HPP_

#include <functional>
#include <utility>

#include "../casimir.hpp"
#include "../utilities/uuid.hpp"
#include "../utilities/string_serializable.hpp"
#include "../utilities/string.hpp"
#include "../utilities/optional.hpp"
#include "contextual_object.hpp"
#include "interface.hpp"

namespace Casimir::framework {
    
    /**
     * @brief RawData store the pointer to a data chunk allocated by an Allocator.
     * It also keep track of the size of the block as long as the allocator and it's validity
     */
    class RawData : public utilities::StringSerializable, public ContextualObject {
        CASIMIR_DISABLE_COPY_MOVE(RawData)
        friend class AbstractAllocator;
    private:
        void* m_ptr;
        cuint m_size;
        AbstractAllocator* m_allocator;
        
        /**
         * @brief Default private constructor of the RawData
         * @param ctx RawData are ContextualObject and required to live in a given context
         * @param ptr The data chunk pointer (not necessary in the memory)
         * @param size The size of the data chunk hold by this block
         * @param allocator The allocator that has allocate this chunk of data
         */
        inline explicit RawData(CasimirContext ctx, void* ptr, cuint size, AbstractAllocator* allocator)
                : ContextualObject(ctx),
                m_ptr(ptr), m_size(size),
                m_allocator(allocator) {}

    public:
        /**
         * @brief Convert the current block to a String for debug purpose
         * @return A utilities::String that describe the current state of the RawData
         */
        CASIMIR_EXPORT utilities::String toString() const override;
        
        /**
         * @brief Default destructor of the RawData. This methods will call the AbstractAllocator::internalFree
         * with a reference to self if the block is the root block (has no parents)
         */
        CASIMIR_EXPORT virtual ~RawData();
   
        /**
         * @brief Return the data hold by the block
         * @throw utilities::Exception if the data is no longer valid
         * @return the data hold by the block
         */
        CASIMIR_EXPORT void* data() const;
        
        /**
         * @brief Get the allocator that has create this RawData
         * @return The AbstractAllocator* that create the current block
         */
        inline AbstractAllocator* allocator() const {
            return m_allocator;
        }
        
        /**
         * @brief Get the size of the current chunk of data
         * @return The size of the current chunk of data
         */
        inline cuint size() const {
            return m_size;
        }
    };
    
    /**
     * @brief The AbstractAllocator is aimed at managing the memory of a given interface (for instance GPU).
     */
    class AbstractAllocator : public utilities::StringSerializable, public ContextualObject, public IndexableObject {
        CASIMIR_DISABLE_COPY_MOVE(AbstractAllocator)
        friend class RawData;
    protected:
        /**
         * @brief This protected function defines a new copyFunction. This function enable to copy data between multiple interface
         * @throw utilities::Exception if the two Allocators doesn't share the same context
         * @param copyFunction The function to be called to transfer the data to a given RawData from a source RawData
         * @param destinationAllocator The destination allocator
         * @param sourceAllocator The source allocator
         */
        CASIMIR_EXPORT static void registerCopyFunction(const std::function<void(RawData*, const RawData*, cuint, cuint, cuint)>& copyFunction,
                             AbstractAllocator* destinationAllocator,
                             AbstractAllocator* sourceAllocator);
        
        /**
         * @brief Default constructor of the AbstractAllocator
         * @param ctx Because an allocator is an contextual object require a context to constructor the object
         */
        inline explicit AbstractAllocator(CasimirContext ctx, utilities::Uuid uuid)
                : ContextualObject(ctx),
                  IndexableObject(std::move(uuid)) {}
        
        /**
         * @brief Create a RawData
         * @param ptr The pointer to the data that will be used to create the RawData
         * @param size The size of the chunk of data
         * @param allocator The allocator that called this function
         * @return A new instance of RawData based on that information
         * @warning One pointer must not be shared between multiple RawData. If you want to create (for instance a sub-block)
         * use the methods RawData::subBlock
         */
        inline static RawData* createDataBlock(void* ptr, cuint size, AbstractAllocator* allocator) {
            return new RawData(allocator->ctx(), ptr, size, allocator);
        }
        
        /**
         * @brief This methods may only be called by the RawData is used to free a given RawData
         * @param dataBlock The dataBlock to be free
         */
        virtual void internalFree(RawData* dataBlock) = 0;
    
    public:
        /**
         * @brief Copy data source a RawData dest another. RawData must have the same size
         * @param dest The block where the data will be copied
         * @param source The source data block
         * @param length The length of the copy source the start in bytes. (How many bytes will we copied)
         * @param offsetDestination The offset of the copy (the destination) source the start in bytes
         * @param offsetSource The offset of the copy (the source) source the start in bytes.
         */
        CASIMIR_EXPORT static void copy(RawData* dest, const RawData* source, cuint length, cuint offsetDestination, cuint offsetSource);
        
        /**
         * @brief Allocate a RawData of a given size
         * @throw utilities::Exception if the allocation process failed
         * @param size The size of the chunk of data we want to allocate
         * @return The resulting RawData that hold both the data addresses and the size
         */
        virtual RawData* allocate(cuint size) = 0;
        
        /**
         * @brief Return the interface that uses the current allocator
         * @return the interface linked to the current allocator
         */
        virtual AbstractInterface* interface() const = 0;
    };
    
}

#endif
