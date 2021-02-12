#include "heap_allocator.hpp"
#include "private-context.hpp"
#include "../utilities/exception.hpp"
#include "heap_interface.hpp"
#include "../framework/datachunk.hpp"

#include <memory>
#include <cstdlib>
#include <cstring>

#ifdef _WIN32
#define aligned_alloc(size, alignement) _aligned_malloc(size, alignement)
#else
// #define aligned_alloc(size, alignement) aligned_alloc(size, alignement)
#endif

namespace Casimir::core {
    
    using namespace literals;
    using namespace framework;
    
    CASIMIR_EXPORT void copyFromHeapAllocatorToHeapAllocator(RawData* dest, const RawData* source,
                                                             cuint length, cuint offsetDestination, cuint offsetSource) {
        memcpy(offsetOf(dest->data(), offsetDestination), offsetOf(source->data(), offsetSource), length);
    }
    
    CASIMIR_EXPORT HeapAllocator::HeapAllocator(HeapInterface* heapInterface)
            : AbstractAllocator(heapInterface->ctx(), heapInterface->uuid()), m_interface(heapInterface) {
        // Debug the construction if required
        if(heapInterface->m_config.debug()) {
            ctx()->logger(PrivateLogging::Info) << "New allocator heap registered at " << this << " with uuid " << uuid();
        }
        
        // Register the new copy function (internal copy)
        const auto& key = std::make_pair(heapInterface->uuid(), heapInterface->uuid());
        ctx()->copyFunctions.insert(std::make_pair(key, copyFromHeapAllocatorToHeapAllocator));
    }
    
    CASIMIR_EXPORT HeapAllocator::~HeapAllocator() {
        // Debug the destruction
        if(m_interface->m_config.debug()) {
            ctx()->logger(PrivateLogging::Info) << "Destruction of the HeapAllocator at " << this << " with uuid " << uuid();
        }
        
        // Free all the tracked blocks of data
        while (!m_blocks.empty()) {
            delete *m_blocks.begin();
        }

    }
    
    CASIMIR_EXPORT framework::RawData* HeapAllocator::allocate(cuint size) {
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
        
        // Otherwise create the RawData
        framework::RawData* block = createDataBlock(data, size, this);
        if(m_interface->m_config.doesKeepTrackOfDataChunk()) {
            m_blocks.insert(block);
        }
        return block;
    }
    
    CASIMIR_EXPORT void HeapAllocator::internalFree(framework::RawData* dataBlock) {
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
        free(dataBlock->data());

        // Debug
        if(m_interface->m_config.debugMemory()) {
            ctx()->logger(PrivateLogging::Note) << "Free the chunk of data at " << dataBlock->data();
        }
    }
    
    CASIMIR_EXPORT framework::AbstractInterface* HeapAllocator::interface() const  {
        return (framework::AbstractInterface*) m_interface;
    }
    
    CASIMIR_EXPORT utilities::String HeapAllocator::toString() const {
        return "HeapAllocator(uuid=" + uuid() + ", trackedSize=" + utilities::String::toString((cint) m_blocks.size()) + ")";
    }
    
}
