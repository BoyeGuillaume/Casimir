#include "private-context.hpp"

#include <ctime>
#include <memory>
#include <map>

namespace Casimir {

    using namespace utilities;
    using namespace literals;

    CASIMIR_EXPORT String formattedTime() {
        // Retrieve current time point
        time_t timePoint;
        time(&timePoint);

        // Convert it to tm structure
#ifdef _WIN32
        tm* now = new tm();
        gmtime_s(now, &timePoint);
#else
        tm* now = gmtime(&timePoint);
#endif

        // Retrieve the buffer
        char buffer[250];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d at %H:%M:%S [ UTC%z ]", now);

        // No long need for now
        delete now;

        return String(buffer);
    }

    CASIMIR_EXPORT String formattedParser(const utilities::String& str, const utilities::String& channelName) {
        // The expected format is the following one
        // [ DATE ] channelName :   text multiline if require (always aligned)
        //                      |   with wrap capabilities
        const String currentTime = formattedTime();
        const String channelNameF = channelName + " : ";
        const cuint timeAndDateSize = 50;
        const cuint lineContentWidth = 60;
        const cuint minLineContentWidth = 30;

        // Create the header part
        String output = "[ " + currentTime + " ] ";
        output.append(toUnsigned((cint) timeAndDateSize - (cint) output.length() - (cint) channelNameF.length()), ' ');
        output.append(channelNameF);

        // Loop other the string and wrap each word if required
        cuint position = 0;
        cuint lineStartingPosition = 0;
        while (position < str.length()) {
            // Find next position with a space
            cuint nextPosition = str.findFirstOf(" ", position);

            // If the next word doesn't wrap into the line
            if (nextPosition - lineStartingPosition >= lineContentWidth) {
                // Either word-wrap or space-wrap
                if (position - lineStartingPosition > minLineContentWidth) { // Space wrap
                    output.append(str.substr(lineStartingPosition, position - lineStartingPosition));
                    output.append(lineContentWidth - (position - lineStartingPosition), ' ');
                    output.append("\n");
                    output.append(timeAndDateSize - 2, ' ');
                    output.append("| ");
                    lineStartingPosition = position;
                    position = nextPosition + 1 > nextPosition ? nextPosition + 1 : nextPosition;
                } else { // Word-wrap (assert nextPosition is valid)
                    const cuint length = std::min(lineContentWidth - 1, str.length() - lineStartingPosition);
                    if (str.length() - lineStartingPosition < lineContentWidth) { // If no more line after this one
                        output.append(str.substr(lineStartingPosition, length) + "\n");
                    }
                    else {
                        output.append(str.substr(lineStartingPosition, length) + "-\n");
                        output.append(timeAndDateSize - 2, ' ');
                        output.append("| ");
                    }
                    lineStartingPosition += length;
                    position = lineStartingPosition;
                }
            } else {
                position = nextPosition + 1;
            }
        }
        return output;
    }

    CASIMIR_EXPORT utilities::Logger instantiateLogger(const String& filepath) {
        std::shared_ptr<ShellLogger> shellLogger = std::make_shared<ShellLogger>();
        std::shared_ptr<FileLogger> fileLogger = std::make_shared<FileLogger>(filepath);

        LoggerBuilder builder = LoggerBuilder();
        std::vector<std::pair<Uuid, String>> channels = {
                {PrivateLogging::Error, "ERROR"},
                {PrivateLogging::Info, "INFO"},
                {PrivateLogging::Warning, "WARN"},
                {PrivateLogging::Note, "NOTE"},
        };
        // Adding all the channels that perform parsing
        for(const auto& channel : channels) {
            std::function<String(const String&)> parser = [channel](const String& msg){ return formattedParser(msg, channel.second); };
            builder.registerChannelAt(channel.first, shellLogger, parser);
            builder.registerChannelAt(channel.first, fileLogger, parser);
        }

        // Adding the raw channel that doesn't perform any complex parsing
        builder.registerChannelAt(PrivateLogging::Raw, shellLogger, [](const String& msg){ return msg; });
        builder.registerChannelAt(PrivateLogging::Raw, fileLogger, [](const String& msg){ return msg; });

        // Return the constructed Logger
        return builder.create();
    }
}
