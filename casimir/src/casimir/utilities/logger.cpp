#include "logger.hpp"

#include <utility>
#include <iostream>

namespace Casimir {

    CASIMIR_EXPORT utilities::LoggerChannelAdapter::~LoggerChannelAdapter() {
        for(const auto& channel : m_loggerChannel) {
            channel->log(m_msg);
        }
    }

    namespace utilities {

        class __Logger {
            CASIMIR_DISABLE_COPY_MOVE(__Logger);
        private:
            std::unordered_map<Uuid, std::vector<std::shared_ptr<AbstractLoggerChannel>>> m_channels;

        public:
            CASIMIR_EXPORT explicit __Logger(std::unordered_map<Uuid, std::vector<std::shared_ptr<AbstractLoggerChannel>>> channels)
                    : m_channels(std::move(channels)) {

            }

            CASIMIR_EXPORT LoggerChannelAdapter get(const Uuid& uuid) const {
                auto it = m_channels.find(uuid);
                if(it == m_channels.end()) {
                    return LoggerChannelAdapter({});
                }
                return LoggerChannelAdapter(it->second);
            }
        };

        CASIMIR_EXPORT  Logger::Logger(const std::unordered_map<Uuid, std::vector<std::shared_ptr<AbstractLoggerChannel>>>& channels) {
            m_handle = std::make_shared<__Logger>(channels);
        }

        CASIMIR_EXPORT LoggerChannelAdapter Logger::at(const Uuid& uuid) const {
            return m_handle->get(uuid);
        }

        CASIMIR_EXPORT LoggerBuilder& LoggerBuilder::registerChannelAt(const Uuid& uuid,
                                                                       const std::shared_ptr<AbstractLoggerChannel>& channel) {
            auto it = m_channels.find(uuid);
            if(it == m_channels.end()) {
                m_channels.insert(std::make_pair(uuid, std::vector<std::shared_ptr<AbstractLoggerChannel>>{channel}));
            }
            else {
                it->second.push_back(channel);
            }
            return *this;
        }

        CASIMIR_EXPORT Logger LoggerBuilder::create() const {
            return Logger(m_channels);
        }

        CASIMIR_EXPORT ShellLogger::ShellLogger(std::function<String(const String&)> parser)
        : m_parser(std::move(parser)) {

        }

        CASIMIR_EXPORT void ShellLogger::log(const String &msg) {
            m_mutex.acquireLock();
            std::cout << m_parser(msg).str() << std::endl;
            m_mutex.releaseLock();
        }

    }




};
