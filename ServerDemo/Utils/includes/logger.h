#ifndef UTILS_LOGGER_H
#define UTILS_LOGGER_H

#include "datetime.h"


#include <cstdarg>

namespace utils
{

class Logger
{
public:
    enum Level
    {
        OFF,
        ERROR,
        WARN,
        INFO,
        DEBUG,

        LEN
    };

    const char *LEVEL_NAMES[LEN] =
    {
        "NONE",
        "ERROR",
        "WARN",
        "INFO",
        "DEBUG"
    };

public:
    static Logger *instance()
    {
        if (!m_logger)
        {
            m_logger = new Logger();
        }

        return m_logger;
    }

    void setLevel(Level level)
    {
        m_level = level;
    }

    void write(Level level, const char *file, int line, const char *func, const char *fmt, ...)
	{
		if (level > m_level)
		{
			return;
		}

        auto datetime = utils::Datetime();
        std::string purefile = file;
        purefile = purefile.substr(purefile.find_last_of("/") + 1);

        std::string msg;
        {
            va_list ap;
            va_start(ap, fmt);
            auto len = vsnprintf(nullptr, 0, fmt, ap);
            auto buf = new char[len + 1];
            va_start(ap, fmt);
            len = vsnprintf(buf, len + 1, fmt, ap);
            buf[len] = '\0';
            msg = buf;
            va_end(ap);
        }

		printf("[%s][%s][%s]%s",
            datetime.ymdhmss().c_str(),
            LEVEL_NAMES[level],
            (purefile + ":" + std::to_string(line)).c_str(),
            msg.c_str());
        (void)func;
	}

private:
    Logger() = default;
    Logger(const Logger&) = delete;
    Logger &operator=(const Logger&) = delete;

private:
    Level m_level = Level(LEN - 1);
    static Logger* m_logger;
};

Logger* Logger::m_logger = nullptr;

} // namespace utils

#define LOG_ERROR( fmt, ...) utils::Logger::instance()->write(utils::Logger::ERROR,  __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
#define LOG_WARN(  fmt, ...) utils::Logger::instance()->write(utils::Logger::WARN,   __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
#define LOG_INFO(  fmt, ...) utils::Logger::instance()->write(utils::Logger::INFO,   __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
#define LOG_DEBUG( fmt, ...) utils::Logger::instance()->write(utils::Logger::DEBUG,  __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)

#endif // UTILS_LOGGER_H