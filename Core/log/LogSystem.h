#ifndef LOG_SYSTEM_H
#define LOG_SYSTEM_H

#include <spdlog/spdlog.h>
#include <memory>

namespace Cola
{
    class LogSystem
    {
    public:
        enum LogLevel : int
        {
            debug,
            info,
            warn,
            error,
            fatal
        };
        LogSystem();
        ~LogSystem();

        static LogSystem* Instance()
        {
            if (mLogSystemInstance == nullptr)
            {
                mLogSystemInstance = new LogSystem;
            }
            return mLogSystemInstance;
        }

        template<typename... TARGS>
        void log(LogLevel level, TARGS&&... args)
        {
            switch (level)
            {
            case LogLevel::debug :
                m_logger->debug(std::forward<TARGS>(args)...);
                break;
            case LogLevel::info :
                m_logger->info(std::forward<TARGS>(args)...);
                break;
            case LogLevel::warn :
                m_logger->warn(std::forward<TARGS>(args)...);
                break;
            case LogLevel::error :
                m_logger->error(std::forward<TARGS>(args)...);
                break;
            case LogLevel::fatal :
                m_logger->critical(std::forward<TARGS>(args)...);
                break;
            default:
                break;
            }
        }

    private:
        std::shared_ptr<spdlog::logger> m_logger;
        static LogSystem *mLogSystemInstance;
    };

#define LOG_HELPER(LOG_LEVEL, ...) \
    LogSystem::Instance()->log(LOG_LEVEL, "[" + std::string(__FUNCTION__) + "] " + __VA_ARGS__);


#define LOG_DEBUG(...) LOG_HELPER(LogSystem::LogLevel::debug, __VA_ARGS__);

#define LOG_INFO(...) LOG_HELPER(LogSystem::LogLevel::info, __VA_ARGS__);

#define LOG_WARN(...) LOG_HELPER(LogSystem::LogLevel::warn, __VA_ARGS__);

#define LOG_ERROR(...) LOG_HELPER(LogSystem::LogLevel::error, __VA_ARGS__);

#define LOG_FATAL(...) LOG_HELPER(LogSystem::LogLevel::fatal, __VA_ARGS__);

}


#endif