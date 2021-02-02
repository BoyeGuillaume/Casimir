#ifndef CASIMIR_CONTEXT_HPP_
#define CASIMIR_CONTEXT_HPP_

#include "../configuration.hpp"

namespace Casimir {
    class PrivateCasimirContext;
    typedef PrivateCasimirContext* CasimirContext;

    CASIMIR_EXPORT CasimirContext createContext();
    CASIMIR_EXPORT void releaseContext(CasimirContext ctx);
};

#endif
