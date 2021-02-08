#include "context.hpp"
#include "private-context.hpp"

namespace Casimir {

    CASIMIR_EXPORT CasimirContext createContext(const ContextConfiguration& configuration) {
        // Instantiate the logger
        utilities::Logger logger = instantiateLogger(configuration.m_logFile, configuration.m_logToShell);
        
        // Display the header in the logger
        logger(PrivateLogging::Raw) << utilities::String('=', 110) << "\n";
        logger(PrivateLogging::Raw) << utilities::String(' ', 40) << "Starting Casimir v" << CASIMIR_VERSION << " context at" << "\n";
        logger(PrivateLogging::Raw) << utilities::String(' ', 40) << formattedTime() << "\n";
        logger(PrivateLogging::Raw) << utilities::String('=', 110) << "\n";
        
        return (CasimirContext) new PrivateCasimirContext{logger};
    }

    CASIMIR_EXPORT void releaseContext(CasimirContext ctx) {
        // Display the end footer
        ctx->logger(PrivateLogging::Raw) << utilities::String('=', 110) << "\n\n\n\n\n";
        
        // Delete the context
        delete (PrivateCasimirContext*) ctx;
    }
    
};
