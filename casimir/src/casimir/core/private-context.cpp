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
        tm* now = new tm();
#ifdef _WIN32
        gmtime_s(now, &timePoint);
#else
        *now = *gmtime(&timePoint);
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

        // Create the header part
        String output = "[ " + currentTime + " ] ";
        output.append(toUnsigned((cint) timeAndDateSize - (cint) output.length() - (cint) channelNameF.length()), ' ');
        output.append(channelNameF);

        // Retrieve all the word of the current output
        std::vector<String> words = str.split(" ", true);
        String appended;
        for (cuint i = 0; i < words.size(); ++i) {
            // And the new words
            appended.append(words[i] + " ");

            // If there's more than only one line then split the next words
            if (i + 1 == words.size() || appended.length() + words[i + 1].length() > lineContentWidth) {
                output.append(appended);
                if (i + 1 < words.size()) {
                    output.append(toUnsigned((cint)lineContentWidth - (cint)appended.length()), ' ');
                    output.append("\n");
                    output.append(timeAndDateSize - 2, ' ');
                    output.append("| ");
                }
                appended = "";
            }
        }
        
        return output + "\n";
    }

    CASIMIR_EXPORT utilities::Logger instantiateLogger(const utilities::String& filepath, bool logToShell) {
        std::shared_ptr<FileLogger> fileLogger = nullptr;
        if (!filepath.isEmpty()) {
            fileLogger = std::make_shared<FileLogger>(filepath);
        }
        std::shared_ptr<ShellLogger> shellLogger = std::make_shared<ShellLogger>();

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
            if(logToShell) {
                builder.registerChannelAt(channel.first, shellLogger, parser);
            }
            if(fileLogger) {
                builder.registerChannelAt(channel.first, fileLogger, parser);
            }
        }

        // Adding the raw channel that doesn't perform any complex parsing
        if(logToShell) {
            builder.registerChannelAt(PrivateLogging::Raw, shellLogger, [](const String& msg){ return msg; });
        }
        if(fileLogger) {
            builder.registerChannelAt(PrivateLogging::Raw, fileLogger, [](const String& msg){ return msg; });
        }

        // Return the constructed Logger
        return builder.create();
    }
}
