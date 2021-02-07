#include "allocator.hpp"
#include "../core/private-context.hpp"
#include "../utilities/exception.hpp"

namespace Casimir::framework {
    
    using namespace literals;
    using namespace utilities;
    
    CASIMIR_EXPORT utilities::String DataBlock::toString() const {
        return "DataBlock(data=" + String::toString((cint) m_ptr).encodeToHex()
        + ", size=" + String::toString((cint) m_size) + ", allocator=" + m_allocator->toString() + ")";
    }
    
    CASIMIR_EXPORT DataBlock::~DataBlock() {
        if(m_parent == nullptr) {
            m_allocator->internalFree(this);
            *m_validity = false;
        }
    }
    
    CASIMIR_EXPORT DataBlock* DataBlock::subBlock(cuint offset, cuint size) {
#ifdef CASIMIR_SAFE_CHECK
        if(offset + size > m_size) {
            CASIMIR_THROW_EXCEPTION("IndexOutOfRange", "Cannot extract the required subBlock as the given range isn't valid");
        }
#endif

        return new DataBlock(ctx(), (ubyte*) data() + offset, size, allocator(), this, m_validity);
    }
    
    CASIMIR_EXPORT void* DataBlock::data() const {
#ifdef CASIMIR_SAFE_CHECK
        if(!*m_validity) {
            CASIMIR_THROW_EXCEPTION("InvalidPointer", "Cannot access a deleted DataBlock");
        }
#endif
        return m_ptr;
    }
    
    
    CASIMIR_EXPORT void AbstractAllocator::registerCopyFunction(std::function<void(DataBlock*, const DataBlock*)> copyFunction,
                                                 AbstractAllocator* destinationAllocator,
                                                 AbstractAllocator* sourceAllocator) {

#ifdef CASIMIR_SAFE_CHECK
      if(destinationAllocator->ctx() !=  sourceAllocator->ctx()) {
          CASIMIR_THROW_EXCEPTION("IncompatibleContext", "Cannot move data between different context");
      }
#endif
        
        // Create the pair of allocator in orders
        // destination - source
        std::pair<IndexableObject, IndexableObject> keys = std::make_pair(destinationAllocator->index(), sourceAllocator->index());
        
        // Insert the new value
        destinationAllocator->ctx()->copyFunctions.insert(std::make_pair(keys, copyFunction));
    }
    
    CASIMIR_EXPORT void AbstractAllocator::copy(DataBlock* to, const DataBlock* from) {
#ifdef CASIMIR_SAFE_CHECK
        if(to->ctx() != from->ctx()) {
            CASIMIR_THROW_EXCEPTION("IncompatibleContext", "Cannot move data between different context");
        }
#endif
        // Retrieve the context pointer
        CasimirContext  ctx = to->ctx();
        
        // First retrieve the source and the destination allocators
        std::pair<IndexableObject, IndexableObject> keys = std::make_pair(to->allocator()->index(), from->allocator()->index());
        
        // Find the value corresponding to the given keys
        auto it = ctx->copyFunctions.find(keys);

#ifdef CASIMIR_SAFE_CHECK
        if(it == ctx->copyFunctions.end()) {
            CASIMIR_THROW_EXCEPTION("OperationNotSupported", "Cannot perform the required operation because no"
                                                             "copy can be perform between the two DataBlock");
        }
        if(to->size() != from->size()) {
            CASIMIR_THROW_EXCEPTION("IndexOutOfRange", "Cannot perform the required copy as the size of the two blocks aren't compatible");
        }
#endif
        
        // Then perform the operation
        (it->second)(to, from);
        
        // Log the operation
        ctx->logger(PrivateLogging::Info) << "Copy " << from->size() << " bytes from " << from->data() << " to " << to->data();
    }
    
};
