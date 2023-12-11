//
// Created by tqcq on 2023/12/5.
//

#include "logger.h"
#include <fmt/format.h>
#include <fmt/time.h>
#include <sstream>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <algorithm>

namespace tqcq {

Logger &
Logger::GetInstance()
{
    static Logger instance;
    return instance;
}

Logger::Logger() {
    const char* env_level = getenv("ULIB_LOG_LEVEL");
    if (!env_level) {
        SetLogLevel(Level::kALL);
    } else {
        char buffer[1024] =  "invalid ULIB_LOG_LEVEL=";
        for (const char* ptr = env_level; ptr && *ptr; ++ptr) {
            if (!isdigit(*ptr)) {
                strncat(buffer + strlen(buffer), env_level, 1000);
                Log(Level::kError, "logger.cpp", __FUNCTION__, __LINE__, "ulib.log", buffer);
                return;
            }
        }

        int level = atoi(env_level);
        level = std::max(static_cast<int>(Level::kALL), std::min(static_cast<int>(Level::kOFF), level));
        SetLogLevel(static_cast<Level::LevelEnum>(level));
    }
}

Logger::~Logger() {}

void
Logger::Log(int32_t level, const char *file, const char *func, int32_t line, const char* tag, const char *msg)
{
    if (level < level_) {
        return;
    }
    const char *level_name = Level::ToString(level);
    /**
     * @brief time file:line@func tag level_name msg
     */
    std::string pattern = "{} {}:{}@{} {} [{:<5}]: {}";
    std::string log_time;
    {
        std::time_t now = time(NULL);
        std::tm* timeinfo = std::localtime(&now);

        struct timeval timeval_now;
        gettimeofday(&timeval_now, NULL);

        log_time = fmt::format("{:02}-{:02} {:02}:{:02}:{:02}.{:06}", timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, timeval_now.tv_usec);
    }

    // auto add CR
    bool need_append_line_break = !msg || *msg == '\0' || msg[strlen(msg) - 1] != '\n';
    if (need_append_line_break) {
        pattern.append(1, '\n');
    }
    fmt::print(pattern, log_time, file + stripped_prefix_len_, line, func, tag, level_name, msg);
}

void
Logger::SetStrippedPrefixLen(size_t len)
{
    stripped_prefix_len_ = len;
}

void
Logger::SetLogLevel(Level::LevelEnum min_level)
{
    level_ = min_level;
}

}// namespace tqcq
