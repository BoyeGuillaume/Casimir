#include "logger.hpp"
#include "exception.hpp"

#include <utility>
#include <iostream>


namespace Casimir::utilities {

    CASIMIR_EXPORT utilities::LoggerChannelAdapter::~LoggerChannelAdapter() {
        const String msg = m_parser(m_msg);
        for (const auto& channel : m_loggerChannel) {
            channel->log(msg);
        }
    }

    CASIMIR_EXPORT utilities::AbstractLoggerChannel::~AbstractLoggerChannel() = default;

    class __Logger {
        CASIMIR_DISABLE_COPY_MOVE(__Logger);
    private:
        std::unordered_map<Uuid, Logger::__LoggerChannelStorage> m_channels;

    public:
        CASIMIR_EXPORT explicit __Logger(
                std::unordered_map<Uuid, Logger::__LoggerChannelStorage> channels)
                : m_channels(std::move(channels)) {

        }

        CASIMIR_EXPORT LoggerChannelAdapter get(const Uuid& uuid) const {
            auto it = m_channels.find(uuid);
            if (it == m_channels.end()) {
                return LoggerChannelAdapter([](const String& a){ return a; }, {});
            }
            return LoggerChannelAdapter(it->second.parser, it->second.channels);
        }
    };

    CASIMIR_EXPORT Logger::Logger(
            const std::unordered_map<Uuid, __LoggerChannelStorage>& channels) {
        m_handle = std::make_shared<__Logger>(channels);
    }

    CASIMIR_EXPORT LoggerChannelAdapter Logger::at(const Uuid& uuid) const {
        return m_handle->get(uuid);
    }

    CASIMIR_EXPORT LoggerBuilder& LoggerBuilder::registerChannelAt(const Uuid& uuid,
                                                                   const std::shared_ptr<AbstractLoggerChannel>& channel,
                                                                   const std::function<String(const String&)>& parser) {
        auto it = m_channels.find(uuid);
        if (it == m_channels.end()) {
            m_channels.insert(std::make_pair(uuid, Logger::__LoggerChannelStorage{
                std::vector<std::shared_ptr<AbstractLoggerChannel>>{channel},parser
            }));
        } else {
            it->second.channels.push_back(channel);
        }
        return *this;
    }

    CASIMIR_EXPORT Logger LoggerBuilder::create() const {
        return Logger(m_channels);
    }

    CASIMIR_EXPORT ShellLogger::ShellLogger() = default;

    CASIMIR_EXPORT void ShellLogger::log(const String& msg) {
        m_mutex.acquireLock();
        std::cout << msg.str();
        m_mutex.releaseLock();
    }

    CASIMIR_EXPORT FileLogger::FileLogger(const String& filepath)
            : m_fstream(new std::fstream()) {
        m_fstream->open(filepath.c_str(), std::ios_base::out | std::ios_base::app);
        if (!m_fstream->is_open()) { // In case of any exception
            const String what = std::system_error(errno, std::system_category(),
                                                  "Cannot open file " + filepath.str()).what();
            // Delete the file stream
            delete m_fstream;
            m_fstream = nullptr;

            // Throw the exception
            CASIMIR_THROW_EXCEPTION("SystemException", what);
        }
    }

    CASIMIR_EXPORT void FileLogger::log(const String& msg) {
#ifdef CASIMIR_SAFE_CHECK
        if(!m_fstream) {
            CASIMIR_THROW_EXCEPTION("InvalidUsage", "Cannot call the log function after the FileLogger constructor failed");
        }
#endif
        m_fstream->write(msg.c_str(), msg.length());
        m_fstream->flush();
    }

    CASIMIR_EXPORT FileLogger::~FileLogger() {
        m_fstream->close();
        delete m_fstream;
    }

};
