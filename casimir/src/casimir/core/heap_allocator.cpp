#include "heap_allocator.hpp"
#include "private-context.hpp"
#include "../utilities/exception.hpp"

#include <memory>
#include <cstdlib>

#ifdef _WIN32
#define aligned_alloc(size, alignement) _aligned_malloc(size, alignement)
#else
// #define aligned_alloc(size, alignement) aligned_alloc(size, alignement)
#endif

namespace Casimir::core {
    
    CASIMIR_EXPORT HeapAllocator::HeapAllocator(HeapInterface* heapInterface)
            : AbstractAllocator(heapInterface->ctx(), heapInterface->uuid()), m_interface(heapInterface) {
        // Debug the construction if required
        if(heapInterface->m_config.debug()) {
            ctx()->logger(PrivateLogging::Info) << "New allocator heap registered at " << this << " with uuid " << uuid();
        }
    }
    
    CASIMIR_EXPORT HeapAllocator::~HeapAllocator() {
        // Debug the destruction
        if(m_interface->m_config.debug()) {
            ctx()->logger(PrivateLogging::Info) << "Destruction of the HeapAllocator at " << this << " with uuid " << uuid();
        }
        
        // Free all the tracked blocks of data
        for(framework::DataBlock* block : m_blocks) {
            delete block;
        }
    }
    
    CASIMIR_EXPORT framework::DataBlock* HeapAllocator::malloc(cuint size) {
        const cuint blockAlignment = m_interface->m_config.blockAlignment();
        
        // Debug the memory
        if(m_interface->m_config.debugMemory()) {
            ctx()->logger(PrivateLogging::Note) << "Allocate a chunk of " << size << " bytes into the heap";
        }
        
        // Allocate the data
        void *data = (blockAlignment == 0) ? malloc(size) : aligned_alloc(size, blockAlignment);
        
        // If the allocation failed
        if(data == nullptr) {
            ctx()->logger(PrivateLogging::Error) << "The requested allocation in the Heap failed";
            CASIMIR_THROW_EXCEPTION("InvalidAllocation", "Cannot allocate the required data");
        }
        
        // Otherwise create the DataBlock
        framework::DataBlock* block = createDataBlock(data, size, this);
        if(m_interface->m_config.doesKeepTrackOfDataChunk()) {
            m_blocks.insert(block);
        }
        return block;
    }
    
    CASIMIR_EXPORT void HeapAllocator::internalFree(framework::DataBlock* dataBlock) {
#ifdef CASIMIR_SAFE_CHECK
        if(dataBlock->allocator() != this) {
            CASIMIR_THROW_EXCEPTION("InvalidArgument", "Cannot free the given data block as not been initialized with"
                                                       "the current allocator");
        }
#endif
        
        // Remove the data from the hashset
        if(m_interface->m_config.doesKeepTrackOfDataChunk()) {
            auto it = m_blocks.find(dataBlock);
#ifdef CASIMIR_SAFE_CHECK
            if(it == m_blocks.end()) {
                CASIMIR_THROW_EXCEPTION("NoSuchValueException", "The given dataBlocks cannot be found into the stored blocks");
            }
#endif
            m_blocks.erase(it);
        }
        
        // Free the data
        free(dataBlock->parentOrElseSelf()->data());

        // Debug
        if(m_interface->m_config.debugMemory()) {
            ctx()->logger(PrivateLogging::Note) << "Free the chunk of data at " << dataBlock->parentOrElseSelf()->data();
        }
    }
    
}
