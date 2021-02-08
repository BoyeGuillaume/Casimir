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
     * @brief DataBlock store the pointer to a data chunk allocated by an Allocator.
     * It also keep track of the size of the block as long as the allocator and it's validity
     */
    class DataBlock : public utilities::StringSerializable, public ContextualObject {
        CASIMIR_DISABLE_COPY_MOVE(DataBlock)
        friend class AbstractAllocator;
    private:
        void* m_ptr;
        cuint m_size;
        AbstractAllocator* m_allocator;
        DataBlock* m_parent;
        std::shared_ptr<bool> m_validity;
        
        /**
         * @brief Default private constructor of the DataBlock
         * @param ctx DataBlock are ContextualObject and required to live in a given context
         * @param ptr The data chunk pointer (not necessary in the memory)
         * @param size The size of the data chunk hold by this block
         * @param allocator The allocator that has malloc this chunk of data
         * @param parent The parent DataBlock. By default nullptr is used if this Block is actually a SubBlock
         * of some more important data
         * @param validity The validity of the data hold by the current block (true by default became false if the data
         * has been deleted)
         */
        inline explicit DataBlock(CasimirContext ctx, void* ptr, cuint size, AbstractAllocator* allocator,
                                  DataBlock* parent, std::shared_ptr<bool> validity)
                : ContextualObject(ctx),
                m_ptr(ptr), m_size(size),
                m_allocator(allocator),
                m_parent(parent),
                m_validity(std::move(validity)) {}

    public:
        /**
         * @brief Extract a subBlock from the current block of data
         * @throw utilities::Exception if the specified range isn't contained by the current block
         * @note Deleting a sub-block doesn't affect the parent data but deleting the root object will delete the data
         * of all the block
         * @param offset The offset from the current pointer
         * @param size The size of the subChunk of data
         * @return A new DataBlock that shares a part of the data hold by the current instance
         */
        CASIMIR_EXPORT DataBlock* subBlock(cuint offset, cuint size);
        
        /**
         * @brief Convert the current block to a String for debug purpose
         * @return A utilities::String that describe the current state of the DataBlock
         */
        CASIMIR_EXPORT utilities::String toString() const override;
        
        /**
         * @brief Default destructor of the DataBlock. This methods will call the AbstractAllocator::internalFree
         * with a reference to self if the block is the root block (has no parents)
         */
        CASIMIR_EXPORT ~DataBlock();
   
        /**
         * @brief Return the data hold by the block
         * @throw utilities::Exception if the data is no longer valid
         * @return the data hold by the block
         */
        CASIMIR_EXPORT void* data() const;
        
        /**
         * @brief Get the parent object if exists
         * @return The parent object of the current block. If the current DataBlock doesn't have any parent (is a root object)
         * then simply return the nullptr
         */
        inline DataBlock* parent() const {
            return m_parent;
        }
        
        /**
         * @brief Get the allocator that has create this DataBlock
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
    
        inline DataBlock* parentOrElseSelf() {
            return (m_parent != nullptr) ? m_parent : this;
        }
    };
    
    /**
     * @brief The AbstractAllocator is aimed at managing the memory of a given interface (for instance GPU).
     */
    class AbstractAllocator : public utilities::StringSerializable, public ContextualObject, public IndexableObject {
        CASIMIR_DISABLE_COPY_MOVE(AbstractAllocator)
        friend class DataBlock;
    protected:
        /**
         * @brief This protected function defines a new copyFunction. This function enable to copy data between multiple interface
         * @throw utilities::Exception if the two Allocators doesn't share the same context
         * @param copyFunction The function to be called to transfer the data to a given DataBlock from a source DataBlock
         * @param destinationAllocator The destination allocator
         * @param sourceAllocator The source allocator
         */
        CASIMIR_EXPORT static void registerCopyFunction(std::function<void(DataBlock*, const DataBlock*)> copyFunction,
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
         * @brief Create a DataBlock
         * @param ptr The pointer to the data that will be used to create the DataBlock
         * @param size The size of the chunk of data
         * @param allocator The allocator that called this function
         * @return A new instance of DataBlock based on that information
         * @warning One pointer must not be shared between multiple DataBlock. If you want to create (for instance a sub-block)
         * use the methods DataBlock::subBlock
         */
        inline static DataBlock* createDataBlock(void* ptr, cuint size, AbstractAllocator* allocator) {
            return new DataBlock(allocator->ctx(), ptr, size, allocator, nullptr, std::make_shared<bool>(true));
        }
        
        /**
         * @brief This methods may only be called by the DataBlock is used to free a given DataBlock
         * @param dataBlock The dataBlock to be free
         */
        virtual void internalFree(DataBlock* dataBlock) = 0;
    
    public:
        /**
         * @brief Copy data from a DataBlock to another. DataBlock must have the same size
         * @param to The block where the data will be copied
         * @param from The source data block
         */
        CASIMIR_EXPORT static void copy(DataBlock* to, const DataBlock* from);
        
        /**
         * @brief Allocate a DataBlock of a given size
         * @throw utilities::Exception if the allocation process failed
         * @param size The size of the chunk of data we want to allocate
         * @return The resulting DataBlock that hold both the data addresses and the size
         */
        virtual DataBlock* malloc(cuint size) = 0;
        
        /**
         * @brief Return the interface that uses the current allocator
         * @return the interface linked to the current allocator
         */
        virtual AbstractInterface* interface() const = 0;
    };
    
}

#endif
