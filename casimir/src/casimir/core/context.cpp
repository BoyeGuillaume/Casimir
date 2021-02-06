#include "context.hpp"
#include "private-context.hpp"

namespace Casimir {

    CASIMIR_EXPORT CasimirContext createContext(const char* filepath) {

        std::cout << __FILE__ << ", " << __LINE__ << std::endl;
        utilities::Logger logger = instantiateLogger(filepath);


        std::cout << __FILE__ << ", " << __LINE__ << std::endl;
        // Display the header in the logger
        logger(PrivateLogging::Raw) << utilities::String('=', 100) << "\n";
        logger(PrivateLogging::Raw) << utilities::String(' ', 35) << "Starting Casimir v" << CASIMIR_VERSION << " context at" << "\n";
        logger(PrivateLogging::Raw) << utilities::String(' ', 35) << formattedTime() << "\n";
        logger(PrivateLogging::Raw) << utilities::String('=', 100) << "\n";


        std::cout << __FILE__ << ", " << __LINE__ << std::endl;
        return (CasimirContext) new PrivateCasimirContext{logger};
    }

    CASIMIR_EXPORT void releaseContext(CasimirContext ctx) {

        std::cout << __FILE__ << ", " << __LINE__ << std::endl;
        ctx->logger(PrivateLogging::Raw) << utilities::String('=', 100) << "\n\n\n\n\n";

        std::cout << __FILE__ << ", " << __LINE__ << std::endl;
        delete (PrivateCasimirContext*) ctx;
    }

};
