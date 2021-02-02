#include "context.hpp"
#include "private-context.hpp"

namespace Casimir {

    CASIMIR_EXPORT CasimirContext createContext() {
        return new PrivateCasimirContext();
    }

    CASIMIR_EXPORT void releaseContext(CasimirContext ctx) {
        delete (PrivateCasimirContext*) ctx;
    }

};
