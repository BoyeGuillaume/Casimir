#ifndef CASIMIR_CONTEXT_HPP_
#define CASIMIR_CONTEXT_HPP_

#include <string>
#include <utility>

#include "../configuration.hpp"

namespace Casimir {
    
    class ContextConfiguration;

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
    CASIMIR_EXPORT CasimirContext createContext(const ContextConfiguration& configuration);

    /**
     * @brief Destruct a given CasimirContext. This method perform multiple clean-up task and has to be called manually
     * when we no longer require any Casimir functionality
     * @param ctx the context to be destroyed
     */
    CASIMIR_EXPORT void releaseContext(CasimirContext ctx);
    
    /**
     * @brief Class that defines all the configuration of a given context
     */
    class ContextConfiguration {
        friend CASIMIR_EXPORT CasimirContext createContext(const ContextConfiguration& configuration);
    private:
        std::string m_logFile;
        bool m_logToShell = true;
        
    public:
        /**
         * @brief Sets the logFile used by the context
         * @param logFile the path to the log file
         * @return A self-reference
         */
        inline ContextConfiguration& withLogFile(std::string logFile) {
            m_logFile = std::move(logFile);
            return *this;
        }
        
        /**
         * @brief Sets whether or not the log will be print to the shell
         * @param value the value of this parameters
         * @return A self-reference
         */
        inline ContextConfiguration& withLogToShell(bool value) {
            m_logToShell = value;
            return *this;
        }
        
    };
};

#endif
