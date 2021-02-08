#include "interface.hpp"

#include <utility>
#include "../core/private-context.hpp"

namespace Casimir::framework {
    
    CASIMIR_EXPORT AbstractInterface::AbstractInterface(CasimirContext ctx, utilities::Uuid uuid)
    : ContextualObject(ctx), IndexableObject(std::move(uuid)) {
        ctx->interfaces.insert(this);
    }
    
    CASIMIR_EXPORT AbstractInterface::~AbstractInterface() {
        ctx()->logger(PrivateLogging::Note) << "Unregister interface " << uuid();
        auto it = ctx()->interfaces.find(this);
        ctx()->interfaces.erase(it);
    }
};
