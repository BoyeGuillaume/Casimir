#ifndef CASIMIR_LOGGER_HPP_
#define CASIMIR_LOGGER_HPP_

#include <unordered_map>
#include <utility>
#include <memory>
#include <type_traits>
#include <functional>

#include "../casimir.hpp"
#include "string.hpp"
#include "uuid.hpp"
#include "cmutex.hpp"

namespace Casimir {

    namespace utilities {

        class Logger;
        class __Logger;
        class LoggerBuilder;

        /**
         * @brief An abstract interface that defines the behavior awaiting by an LoggerChannel
         * A LoggerChannel must be able to log String and had a unique Uuid
         */
        class AbstractLoggerChannel {
        public:
            /**
             * @brief Handle a \link utilities::String `msg`
             * @param msg the message to be handled by the LoggerChannel.
             */
            virtual void log(const String& msg) = 0;
        };

        /**
         * @brief An LoggerChannelAdapter is an class that adapt a LoggerChannel to be used from anyone. It enable
         * the user to log data directly into the LoggerChannel without having to deal with conversion...
         */
        class LoggerChannelAdapter {
            CASIMIR_DISABLE_COPY(LoggerChannelAdapter)
            friend class __Logger;
        private:
            std::vector<std::shared_ptr<AbstractLoggerChannel>> m_loggerChannel;
            String m_msg;

            /**
             * @brief Private internal constructor of LoggerChannelAdapter
             * @param loggerChannel The loggerChannel linked to the current adapter
             */
            inline explicit LoggerChannelAdapter(std::vector<std::shared_ptr<AbstractLoggerChannel>> loggerChannel)
            : m_loggerChannel(std::move(loggerChannel)), m_msg("") {}
        public:
            /**
             * @brief Destructor of the LoggerChannelAdapter. Notice that it is during the destruction operation that
             * the logging process take place.
             */
            CASIMIR_EXPORT ~LoggerChannelAdapter();

            /**
             * @brief Append a String to the end of the current logging message
             * @param str the String to be appended
             * @return A self-reference
             */
            inline LoggerChannelAdapter& operator<<(const String& str) {
                m_msg.append(str);
                return *this;
            }

            /**
             * @brief Append a value to the end of the current logging message
             * @param value the unsigned integer to be appended
             * @return A self-reference
             */
            inline LoggerChannelAdapter& operator<<(cuint value) {
                m_msg.append(String::toString((cint) value));
                return *this;
            }

            /**
             * @brief Append a value to the end of the current logging message
             * @param value the integer to be appended
             * @return A self-reference
             */
            inline LoggerChannelAdapter& operator<<(cint value) {
                m_msg.append(String::toString(value));
                return *this;
            }

            /**
             * @brief Append a value to the end of the current logging message
             * @param value the float to be appended
             * @return A self-reference
             */
            inline LoggerChannelAdapter& operator<<(float value) {
                m_msg.append(String::toString(value));
                return *this;
            }

            /**
             * @brief Append a value to the end of the current logging message
             * @param value the double to be appended
             * @return A self-reference
             */
            inline LoggerChannelAdapter& operator<<(double value) {
                m_msg.append(String::toString(value));
                return *this;
            }

            /**
             * @brief Append the value of a pointer to the end of the current logging message
             * @param value the pointer to be displayed. Notice that the display is in hexadecimal
             * @return A self-reference
             */
            inline LoggerChannelAdapter& operator<<(const void* ptr) {
                m_msg.append(String((char*)&ptr, sizeof(void*)).encodeToHex());
                return *this;
            }

            /**
             * @brief Append an StringSerializable object to the logging message
             * @param stringSerializable the stringSerializable object (that will be convert to String)
             * @return A self-reference
             */
            inline LoggerChannelAdapter& operator<<(const StringSerializable& stringSerializable) {
                m_msg.append(stringSerializable.toString());
                return *this;
            }

            /**
             * @brief Append a C-Style string to the end of the logging message
             * @param msg the C-Style string to be appended
             * @return A self-reference
             */
            inline LoggerChannelAdapter& operator<<(const char* msg) {
                m_msg.append(msg);
                return *this;
            }
        };

        /**
         * @brief The logger is a class that manager every channels and can instantiate a LoggerChannelAdapter from an Uuid
         * @note Once constructed the logging process is granted to be thread-safe
         */
        class Logger {
            friend class LoggerBuilder;
        private:
            std::shared_ptr<__Logger> m_handle;

            /**
             * @brief Private constructor of Logger
             * @param channels A map of all the channels by Uuid
             */
            CASIMIR_EXPORT explicit Logger(const std::unordered_map<Uuid, std::vector<std::shared_ptr<AbstractLoggerChannel>>>& channels);

        public:
            /**
             * @brief Retrieve the LoggerChannelAdapter corresponding to the given Uuid
             * @param uuid the Uuid under which the channel as been registered
             * @return The LoggerChannelAdapter that can be used to log directly into the channel with the specified
             * Uuid
             */
            CASIMIR_EXPORT LoggerChannelAdapter at(const Uuid& uuid) const;

            /**
             * @brief Retrieve the LoggerChannelAdapter corresponding to the given Uuid
             * @param uuid the Uuid under which the channel as been registered
             * @return The LoggerChannelAdapter that can be used to log directly into the channel with the specified
             * Uuid
             */
            inline LoggerChannelAdapter operator()(const Uuid& uuid) const {
                return at(uuid);
            }

            /**
             * @brief Retrieve the LoggerChannelAdapter corresponding to the given Uuid
             * @param uuid the Uuid under which the channel as been registered
             * @return The LoggerChannelAdapter that can be used to log directly into the channel with the specified
             * Uuid
             */
            inline LoggerChannelAdapter operator[](const Uuid& uuid) const {
                return at(uuid);
            }
        };

        /**
         * @brief Class aimed at easing the construction of the Logger class above
         */
        class LoggerBuilder {
        private:
            std::unordered_map<Uuid, std::vector<std::shared_ptr<AbstractLoggerChannel>>> m_channels;

        public:
            /**
             * @brief Register a new channel in the future Logger
             * @param channel A shared pointer to an instance of AbstractLoggerChannel
             * @return A self-reference
             */
            CASIMIR_EXPORT LoggerBuilder& registerChannelAt(const Uuid& uuid, const std::shared_ptr<AbstractLoggerChannel>& channel);

            /**
             * @brief Create a new instance of logger based on the configuration above
             * @return The newly created instance of logger
             */
            CASIMIR_EXPORT Logger create() const;
        };

        /**
         * @brief Simple logger that log the resulting message to the terminal
         */
        class ShellLogger : public AbstractLoggerChannel {
        private:
            Mutex m_mutex;
            std::function<String(const String&)> m_parser;

        public:
            /**
             * @brief Default constructor that take a parser as input
             * @param parser An function that parse a String to another String. This function will be called
             * every single time the shellLogger::log is called
             * @note The function pass as the parse must be thread-safe
             */
            CASIMIR_EXPORT explicit ShellLogger(std::function<String(const String&)> parser);

            /**
             * @brief Override the log from the AbstractLoggerChannel. Log a message to the screen.
             * @note This methods is granted to be thread-safe
             * @param msg the message to be logged
             */
            CASIMIR_EXPORT void log(const String &msg) override;
        };


    };

};

#endif
