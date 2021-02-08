#ifndef CASIMIR_HEAPALLOCATOR_HPP_
#define CASIMIR_HEAPALLOCATOR_HPP_

#include <unordered_set>

#include "../casimir.hpp"
#include "../framework/allocator.hpp"
#include "../framework/interface.hpp"

namespace Casimir::core {
    
    class HeapInterface;
    
    /**
     * @brief An HeapAllocator is the standard base allocator. It defines the standard memory
     * allocated directly into the memory accessible directly from this program
     */
    class HeapAllocator : public framework::AbstractAllocator {
        CASIMIR_DISABLE_COPY_MOVE(HeapAllocator)
        friend class HeapInterface;

    private:
        HeapInterface* m_interface;
        std::unordered_set<framework::DataBlock*> m_blocks;
        
    protected:
        /**
         * @brief free the given dataBlock
         * @throw utilities::Exception in case of operation failure
         * @param dataBlock An framework::DataBlock that will be free. This operation is automatically called whenever
         * a root dataBlock is deleted
         */
        CASIMIR_EXPORT void internalFree(framework::DataBlock *dataBlock) override;
        
        /**
         * @brief Default HeapAllocator destructor (only accessible from the heap interface
         * @note If the configuration `trackBlocks` flags is HIGH then this operation will free all the data tracked by
         * the current allocator
         */
        CASIMIR_EXPORT ~HeapAllocator();

        /**
         * @brief Default HeapAllocator constructor
         * @param heapInterface The heap interface attached to the current allocator
         */
        CASIMIR_EXPORT explicit HeapAllocator(HeapInterface* heapInterface);
        
    public:
        /**
         * @brief Allocate a chunk of data stored into a given framework::DataBlock
         * @param size the size of the data we want to allocate
         * @throw utilities::Exception if the operation failed
         * @return A new instance of framework::DataBlock that contains at least `size` bytes of raw data
         */
        CASIMIR_EXPORT framework::DataBlock* allocate(cuint size) override;
        
        /**
         * @brief Return the parent interface attached to the current allocator
         * @return The parent interface attached to the current allocator
         */
        CASIMIR_EXPORT framework::AbstractInterface* interface() const override;
        
        /**
         * @brief Return a utilities::String that describe the current state of HeapAllocator
         * @return A string that describe the current object
         */
        CASIMIR_EXPORT utilities::String toString() const override;
    };
    
}

#endif
