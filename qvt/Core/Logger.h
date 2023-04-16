#ifndef LOGGER_H
#define LOGGER_H
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <memory>

namespace qvt
{
    class Logger
    {
        public:
            static void Init();
            static std::shared_ptr<spdlog::logger>& GetCoreLogger();
            static std::shared_ptr<spdlog::logger>& GetUserLogger();


        private:
            static std::shared_ptr<spdlog::logger> CoreLogger;
            static std::shared_ptr<spdlog::logger> UserLogger;

    };

}
#endif


#define QVT_CORE_TRACE(...) ::qvt::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define QVT_CORE_INFO(...) ::qvt::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define QVT_CORE_WARN(...) ::qvt::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define QVT_CORE_CRITICAL(...) ::qvt::Logger::GetCoreLogger()->critical(__VA_ARGS__)
#define QVT_CORE_ERROR(...) ::qvt::Logger::GetCoreLogger()->error(__VA_ARGS__)

#define QVT_USER_TRACE(...) ::qvt::Logger::GetUserLogger()->trace(__VA_ARGS__)
#define QVT_USER_INFO(...) ::qvt::Logger::GetUserLogger()->info(__VA_ARGS__)
#define QVT_USER_WARN(...) ::qvt::Logger::GetUserLogger()->warn(__VA_ARGS__)
#define QVT_USER_CRITICAL(...) ::qvt::Logger::GetUserLogger()->critical(__VA_ARGS__)
#define QVT_USER_ERROR(...) ::qvt::Logger::GetUserLogger()->error(__VA_ARGS__)

