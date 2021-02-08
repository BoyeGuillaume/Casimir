#include "heap_interface.hpp"
#include "private-context.hpp"
#include "heap_allocator.hpp"

namespace Casimir::core {
    
    using namespace literals;
    
    CASIMIR_EXPORT HeapInterface::HeapInterface(CasimirContext ctx, HeapConfiguration config)
    : framework::AbstractInterface(ctx, Heap) {
        // Debug the construction of the interface
        if(m_config.debug()) {
            ctx->logger(PrivateLogging::Note) << "Register HeapInterface at 0x" << this << " with uuid " << uuid();
        }

        // Instantiate a new allocator
        m_allocator = new HeapAllocator(this);
    }
    
    CASIMIR_EXPORT HeapInterface::~HeapInterface() {        
        // Then destruct the sub allocator
        delete m_allocator;
    }
    
    CASIMIR_EXPORT framework::AbstractAllocator* HeapInterface::allocator() const {
        return m_allocator;
    }
    
    CASIMIR_EXPORT utilities::String HeapInterface::name() const {
        return "HeapInterface()";
    }
    
    CASIMIR_EXPORT utilities::String HeapInterface::toString() const {
        return "HeapInterface(allocator=" + utilities::String((char*) m_allocator, sizeof(void*)).encodeToHex()
        + ", uuid=" + uuid() + ")";
    }
    
    
}
