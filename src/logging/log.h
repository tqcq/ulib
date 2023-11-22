//
// Created by tqcq on 2023/11/22.
//

#ifndef ULIB_SRC_LOGGING_LOG_H_
#define ULIB_SRC_LOGGING_LOG_H_

#include <string>
#include "log_common.h"

namespace tqcq {

class Log {
private:
    Log();
    ~Log();

public:
    Log(const Log &) = delete;
    Log(Log &&) = delete;
    Log &operator=(const Log &) = delete;
    Log &operator=(Log &&) = delete;

public:
    static Log *instance();

    void Close();
    bool ShouldLog(const std::string &type, int level);
    bool SetLogLevel(const std::string &type, int level);

    template<typename... Args>
    void OutMessage(const std::string &filter, LogLevel const level, const char *fmt, Args... args)
    {
        OutMessageImpl(filter, level, fmt, args...);
    }
};

}// namespace tqcq

#endif//ULIB_SRC_LOGGING_LOG_H_
