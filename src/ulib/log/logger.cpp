//
// Created by tqcq on 2023/12/5.
//

#include "logger.h"
#include <fmt/format.h>
#include <fmt/time.h>
#include <sstream>
#include <stdarg.h>
#include <string.h>
#include <sys/time.h>

namespace tqcq {

Logger &
Logger::GetInstance()
{
    static Logger instance;
    return instance;
}

Logger::Logger() {}

Logger::~Logger() {}

void
Logger::Log(int32_t level, const char *file, const char *func, int32_t line, const char* tag, const char *msg)
{
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

}// namespace tqcq
