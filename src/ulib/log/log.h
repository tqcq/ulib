//
// Created by tqcq on 2023/12/5.
//

#ifndef ULIB_SRC_LOG_LOG_H_
#define ULIB_SRC_LOG_LOG_H_

#include "logger.h"
#include <fmt/format.h>

namespace tqcq {

#define _ULOG(level, tag, fmt_str, ...)                                                                                     \
    tqcq::Logger::GetInstance().Log(level, __FILE__, __FUNCTION__, __LINE__, tag,                                           \
                                    fmt::format(fmt_str, ##__VA_ARGS__).c_str())

#define ULOG_SET_STRIPPED_PREFIX_LEN(len) ::tqcq::Logger::GetInstance().SetStrippedPrefixLen(len)

#if ULOG_LEVEL <= ULOG_LEVEL_TRACE
#define ULOG_TRACE(...) _ULOG(ULOG_LEVEL_TRACE, __VA_ARGS__)
#else
#define ULOG_TRACE(...) ((void) 0)
#endif

#if ULOG_LEVEL <= ULOG_LEVEL_DEBUG
#define ULOG_DEBUG(...) _ULOG(ULOG_LEVEL_DEBUG, __VA_ARGS__)
#else
#define ULOG_DEBUG(...) ((void) 0)
#endif

#if ULOG_LEVEL <= ULOG_LEVEL_INFO
#define ULOG_INFO(...) _ULOG(ULOG_LEVEL_INFO, __VA_ARGS__)
#else
#define ULOG_INFO(...) ((void) 0)
#endif

#if ULOG_LEVEL <= ULOG_LEVEL_WARN
#define ULOG_WARN(...) _ULOG(ULOG_LEVEL_WARN, __VA_ARGS__)
#else
#define ULOG_WARN(...) ((void) 0)
#endif

#if ULOG_LEVEL <= ULOG_LEVEL_ERROR
#define ULOG_ERROR(...) _ULOG(ULOG_LEVEL_ERROR, __VA_ARGS__)
#else
#define ULOG_ERROR(...) ((void) 0)
#endif

#if ULOG_LEVEL <= ULOG_LEVEL_FATAL
#define ULOG_FATAL(...) _ULOG(ULOG_LEVEL_FATAL, __VA_ARGS__)
#else
#define ULOG_FATAL(...) ((void) 0)
#endif

// class Log {};

}// namespace tqcq

#endif//ULIB_SRC_LOG_LOG_H_
