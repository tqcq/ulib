//
// Created by tqcq on 2023/12/5.
//

#ifndef ULIB_SRC_LOG_LEVEL_H_
#define ULIB_SRC_LOG_LEVEL_H_

namespace tqcq {

#define ULOG_LEVEL_TRACE 0
#define ULOG_LEVEL_DEBUG 1
#define ULOG_LEVEL_INFO 2
#define ULOG_LEVEL_WARN 3
#define ULOG_LEVEL_ERROR 4
#define ULOG_LEVEL_FATAL 5

#define ULOG_LEVEL_ALL ULOG_LEVEL_TRACE
#define ULOG_LEVEL_OFF (ULOG_LEVEL_FATAL + 1)
#define ULOG_LEVEL_LEVEL ULOG_LEVEL_TRACE

class Level {
public:
    enum LevelEnum {
        kTrace = ULOG_LEVEL_TRACE,
        kDebug= ULOG_LEVEL_DEBUG,
        kInfo = ULOG_LEVEL_INFO,
        kWarn = ULOG_LEVEL_WARN,
        kError = ULOG_LEVEL_ERROR,
        kFatal = ULOG_LEVEL_FATAL,

        TRACE = ULOG_LEVEL_TRACE,
        DEBUG= ULOG_LEVEL_DEBUG,
        INFO = ULOG_LEVEL_INFO,
        WARN = ULOG_LEVEL_WARN,
        ERROR = ULOG_LEVEL_ERROR,
        FATAL = ULOG_LEVEL_FATAL
    };

    static const char* ToString(int level) {
        switch (level) {
            case kTrace:
                return "TRACE";
            case kDebug:
                return "DEBUG";
            case kInfo:
                return "INFO";
            case kWarn:
                return "WARN";
            case kError:
                return "ERROR";
            case kFatal:
                return "FATAL";
            default:
                return "UNKNOWN";
        }
    }
};

}// namespace tqcq

#endif//ULIB_SRC_LOG_LEVEL_H_