//
// Created by tqcq on 2023/11/22.
//

#ifndef ULIB_SRC_LOGGING_LOG_COMMON_H_
#define ULIB_SRC_LOGGING_LOG_COMMON_H_

#include "common/define.h"

namespace tqcq {

enum LogLevel : uint8 {
    LOG_LEVEL_DISABLED      = 0,
    LOG_LEVEL_TRACE         = 1,
    LOG_LEVEL_DEBUG         = 2,
    LOG_LEVEL_INFO          = 3,
    LOG_LEVEL_WARN          = 4,
    LOG_LEVEL_ERROR         = 5,
    LOG_LEVEL_FATAL         = 6,

    NUM_ENABLED_LOG_LEVELS = LOG_LEVEL_FATAL,
    LOG_LEVEL_INVALID = 0xFF
};

class LogCommon {};

}// namespace tqcq

#endif//ULIB_SRC_LOGGING_LOG_COMMON_H_
