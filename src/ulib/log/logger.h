//
// Created by tqcq on 2023/12/5.
//

#ifndef ULIB_SRC_LOG_LOGGER_H_
#define ULIB_SRC_LOG_LOGGER_H_

#include "level.h"
#include "ulib/base/types.h"
#include <string>

namespace tqcq {

class Logger {
public:
    Logger();
    ~Logger();
    static Logger &GetInstance();
    void SetLogLevel(Level::LevelEnum min_level);
    void Log(int32_t level, const char *file, const char *func, int32_t line, const char* tag,const char *msg);
    void SetStrippedPrefixLen(size_t len);

private:
    Logger(const Logger &);
    Logger &operator=(const Logger &);

    size_t stripped_prefix_len_;
    Level::LevelEnum level_;
};

}// namespace tqcq

#endif//ULIB_SRC_LOG_LOGGER_H_
