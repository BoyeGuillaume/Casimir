#ifndef CASIMIR_INTERFACE_HPP_
#define CASIMIR_INTERFACE_HPP_

#include <utility>

#include "../casimir.hpp"
#include "../utilities/uuid.hpp"
#include "../utilities/string_serializable.hpp"
#include "../utilities/string.hpp"
#include "contextual_object.hpp"
#include "indexable.hpp"

namespace Casimir::framework {
 
    class AbstractAllocator;
    
    /**
     * @brief An AbstractInterface (an interface) defines a specific computational capable hardware object. It can be
     * a CPU / a GPU, etc... It must have a dedicated memory (use the allocator to manage it)
     */
    class AbstractInterface : public utilities::StringSerializable, ContextualObject, IndexableObject {
        CASIMIR_DISABLE_COPY_MOVE(AbstractInterface)
    protected:
        /**
         * @brief Default protected constructor
         * @param ctx Because AbstractInterface is a ContextualObject each object are constructed with a given
         * context
         */
        inline AbstractInterface(CasimirContext ctx, utilities::Uuid uuid)
            : ContextualObject(ctx),
              IndexableObject(std::move(uuid)) {}
        
    public:
        /**
         * @brief Retrieve the AbstractAllocator object linked to the current interface. It is used to manager memory
         * @return The linked AbstractAllocator*
         */
        virtual AbstractAllocator* allocator() const = 0;
        
        /**
         * @brief The readable-name of the interface
         * @return A utilities::String that hold the name of the interface
         */
        virtual utilities::String name() const = 0;
        
    };
    
}

#endif