#include "context.hpp"
#include "private-context.hpp"

namespace Casimir {

    CASIMIR_EXPORT CasimirContext createContext(const char* filepath) {
        // Instantiate the logger
        utilities::Logger logger = instantiateLogger(filepath);
        
        // Display the header in the logger
        logger(PrivateLogging::Raw) << utilities::String('=', 100) << "\n";
        logger(PrivateLogging::Raw) << utilities::String(' ', 35) << "Starting Casimir v" << CASIMIR_VERSION << " context at" << "\n";
        logger(PrivateLogging::Raw) << utilities::String(' ', 35) << formattedTime() << "\n";
        logger(PrivateLogging::Raw) << utilities::String('=', 100) << "\n";
        
        return (CasimirContext) new PrivateCasimirContext{logger};
    }

    CASIMIR_EXPORT void releaseContext(CasimirContext ctx) {
        // Display the end footer
        ctx->logger(PrivateLogging::Raw) << utilities::String('=', 100) << "\n\n\n\n\n";
        
        // Delete the context
        delete (PrivateCasimirContext*) ctx;
    }

};
