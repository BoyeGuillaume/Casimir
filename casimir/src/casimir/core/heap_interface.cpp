#include "heap_interface.hpp"
#include "private-context.hpp"
#include "heap_allocator.hpp"

namespace Casimir::core {
    
    CASIMIR_EXPORT HeapInterface::HeapInterface(CasimirContext ctx, HeapConfiguration config)
    : framework::AbstractInterface(ctx, Heap) {
        // Debug the construction of the interface
        if(m_config.debug()) {
            ctx->logger(PrivateLogging::Note) << "Register HeapInterface at " << this << " with uuid " << uuid();
        }
    }
    
    CASIMIR_EXPORT HeapInterface::~HeapInterface() {
        // First debug the destruction
        if(m_config.debug()) {
            ctx()->logger(PrivateLogging::Note) << "Destruction of the HeapInterface at " << this << " with uuid " << uuid();
        }
        
        // Then destruct the sub allocator
        delete m_allocator;
    }
    
    CASIMIR_EXPORT framework::AbstractAllocator* HeapInterface::allocator() const {
        return m_allocator;
    }
    
    CASIMIR_EXPORT utilities::String HeapInterface::name() const {
        return "HeapInterface()";
    }
    
    
}
