//
// Created by tqcq on 2023/12/5.
//

#include "logger.h"
#include "fmt/format.h"
#include <sstream>
#include <stdarg.h>
#include <string.h>

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
Logger::Log(int32_t level, const char *file, const char *func, int32_t line, const char *fmt, ...)
{
    const char *level_name = Level::ToString(level);
    char buffer[8192];
    {
        va_list args;
        va_start(args, fmt);
        vsnprintf(buffer, sizeof(buffer), fmt, args);
        va_end(args);
    }
    fmt::print("[{}] {}:{}@{} msg: {}", level_name, file + stripped_prefix_len_, line, func, buffer);
}

void
Logger::SetStrippedPrefixLen(size_t len)
{
    stripped_prefix_len_ = len;
}

}// namespace tqcq
