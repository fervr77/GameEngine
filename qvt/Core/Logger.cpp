#include "Logger.h"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace qvt
{
    std::shared_ptr<spdlog::logger> Logger::CoreLogger;
    std::shared_ptr<spdlog::logger> Logger::UserLogger;

    void Logger::Init()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        CoreLogger = spdlog::stdout_color_mt("QVT");
        CoreLogger->set_level(spdlog::level::trace);
        UserLogger = spdlog::stdout_color_mt("User");
        UserLogger->set_level(spdlog::level::trace);

    }

    std::shared_ptr<spdlog::logger>& Logger::GetCoreLogger()
    {
        return CoreLogger;
    }

    std::shared_ptr<spdlog::logger>& Logger::GetUserLogger()
    {
        return UserLogger;
    }

}