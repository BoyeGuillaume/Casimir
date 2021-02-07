#ifndef CASIMIR_PRIVATE_CONTEXT_HPP_
#define CASIMIR_PRIVATE_CONTEXT_HPP_

#include <unordered_map>
#include <utility>
#include <functional>

#include "../configuration.hpp"
#include "../utilities/uuid.hpp"
#include "../utilities/logger.hpp"
#include "../framework/indexable.hpp"

namespace Casimir {

    namespace framework {
        class DataBlock;
        class AbstractAllocator;
    }
    
    /**
     * @brief Definition of the opaque handle PrivateCasimirContext. Notice that this file MUST NEVER BE INCLUDED
     * in any exported header of the project has it will cause error.
     */
    struct PrivateCasimirContext {
        utilities::Logger logger;
        
        /**
         * @brief Map of all the copy function that enable to copy data from one allocator (interface) to another one
         *
         * The key is a pair of two Uuid that represent in order the destination allocator uuid and the source
         * allocator uuid
         *
         * The value is a function that take as input the destination and the source DataBlock and perform the conversion         *
         */
        std::unordered_map<std::pair<framework::IndexableObject, framework::IndexableObject>,
            std::function<void(framework::DataBlock*, const framework::DataBlock*)>> copyFunctions;
    };

    namespace PrivateLogging {
        /**
         * @brief This channel uses the standard parsing and display any warning message
         */
        static const utilities::Uuid Warning = utilities::Uuid(17707170361395375139U, 4856449981596320257U);

        /**
         * @brief This channel uses the standard parsing and display any error
         */
        static const utilities::Uuid Error   = utilities::Uuid(9631149747179961252U, 4669447320871580547U);

        /**
         * @brief This channel uses the standard parsing and display a note
         */
        static const utilities::Uuid Note    = utilities::Uuid(2499535614363860142U, 15273922473455247291U);

        /**
         * @brief This channel uses the standard parsing and display an Information
         */
        static const utilities::Uuid Info    = utilities::Uuid(5424077037978735843U, 1871455045818645681U);

        /**
         * @brief This channel uses no parsing and display a raw string
         */
        static const utilities::Uuid Raw     = utilities::Uuid(1927683511390330006U, 7972939591306549178U);
    }

    /**
     * @brief Private function that return a String that represent the current time formatted for log
     * @warning This function may no be thread-safe due to the usage of gmtime and gmtime_s (depending on the platform)
     * @return A utilities::String that represent the current formatted time
     */
    CASIMIR_EXPORT utilities::String formattedTime();

    /**
     * @brief Return a formatted String from a raw message and a channel name
     * @param str The utilities::String that contains the message to be formatted
     * @param channelName The utilities::String that contains the channel name (small name such as ERROR / WARN)
     * @return The resulting formatted utilities::String
     */
    CASIMIR_EXPORT utilities::String formattedParser(const utilities::String& str, const utilities::String& channelName);

    /**
     * @brief Create a logger based on a filepath where to log. It will use the formattedParser for most of the channels
     * @param filepath The filepath where we want to log the message. If the file doesn't exists will be created.
     * @throw Casimir::Exception if we cannot open / create or write into the given filepath
     * @return The resulting utilities::Logger
     */
    CASIMIR_EXPORT utilities::Logger instantiateLogger(const utilities::String& filepath, bool logToShell);
}

#endif
