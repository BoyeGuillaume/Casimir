#ifndef CASIMIR_CONTEXT_HPP_
#define CASIMIR_CONTEXT_HPP_

#include "../configuration.hpp"

namespace Casimir {

    /**
     * @brief Opaque handle of the context
     */
    struct PrivateCasimirContext;

    /**
     * @brief Pointer to the PrivateCasimirContext
     */
    typedef PrivateCasimirContext* CasimirContext;

    /**
     * @brief Create a new CasimirContext with a given `logfile`
     * @param logfile The path to the file we want to log to. Note that if the file doesn't exists it will be created
     * @throw Casimir::Exception if we cannot open / create or write into the given filepath
     * @return A new CasimirContext instance. Note that this instance must be destroyed using the Casimir::releaseContext function
     */
    CASIMIR_EXPORT CasimirContext createContext(const char* logfile);

    /**
     * @brief Destruct a given CasimirContext. This method perform multiple clean-up task and has to be called manually
     * when we no longer require any Casimir functionality
     * @param ctx the context to be destroyed
     */
    CASIMIR_EXPORT void releaseContext(CasimirContext ctx);
};

#endif
