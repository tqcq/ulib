//
// Created by tqcq on 2023/12/5.
//

#ifndef ULIB_SRC_LOG_LOG_H_
#define ULIB_SRC_LOG_LOG_H_

#include "ulib/base/location.h"
#include "logger.h"
#include "level.h"
#include <fmt/format.h>

namespace tqcq {

#define _ULOG(level, tag, fmt_str, ...)                                        \
    tqcq::Logger::GetInstance().Log(                                           \
        level, __FILE__, __FUNCTION__, __LINE__, tag,                          \
        fmt::format(fmt_str, ##__VA_ARGS__).c_str())

#define ULOG_SET_STRIPPED_PREFIX_LEN(len)                                      \
    ::tqcq::Logger::GetInstance().SetStrippedPrefixLen(len)

#if ULOG_LEVEL <= ULOG_LEVEL_TRACE
#define ULOG_TRACE(tag, ...) _ULOG(ULOG_LEVEL_TRACE, tag, __VA_ARGS__)
#else
#define ULOG_TRACE(...) ((void) 0)
#endif

#if ULOG_LEVEL <= ULOG_LEVEL_DEBUG
#define ULOG_DEBUG(tag, ...) _ULOG(ULOG_LEVEL_DEBUG, tag, __VA_ARGS__)
#else
#define ULOG_DEBUG(...) ((void) 0)
#endif

#if ULOG_LEVEL <= ULOG_LEVEL_INFO
#define ULOG_INFO(tag, ...) _ULOG(ULOG_LEVEL_INFO, tag, __VA_ARGS__)
#else
#define ULOG_INFO(...) ((void) 0)
#endif

#if ULOG_LEVEL <= ULOG_LEVEL_WARN
#define ULOG_WARN(tag, ...) _ULOG(ULOG_LEVEL_WARN, tag, __VA_ARGS__)
#else
#define ULOG_WARN(...) ((void) 0)
#endif

#if ULOG_LEVEL <= ULOG_LEVEL_ERROR
#define ULOG_ERROR(tag, ...) _ULOG(ULOG_LEVEL_ERROR, tag, __VA_ARGS__)
#else
#define ULOG_ERROR(...) ((void) 0)
#endif

#if ULOG_LEVEL <= ULOG_LEVEL_FATAL
#define ULOG_FATAL(tag, ...) _ULOG(ULOG_LEVEL_FATAL, tag, __VA_ARGS__)
#else
#define ULOG_FATAL(...) ((void) 0)
#endif

#define ULOG_ASSERT_WITH_TAG(expr, tag, ...)                                   \
    do {                                                                       \
        if (!(expr)) {                                                         \
            _ULOG(ULOG_LEVEL_FATAL, tag, ##__VA_ARGS__);                       \
            abort();                                                           \
        }                                                                      \
    } while (0)

#define ULOG_ASSERT(expr, ...)                                                 \
    ULOG_ASSERT_WITH_TAG(expr, "ulib_assert", ##__VA_ARGS__)

}// namespace tqcq

#endif//ULIB_SRC_LOG_LOG_H_
