//
// Created by tqcq on 2023/11/16.
//
#include "checks.h"
#include <cstdlib>
#include <cstdarg>


namespace tqcq {

namespace detail {

void CheckLog(const char *expr, const char *file, int line, const char *msg, ...) {
    std::fprintf(stderr, "Check failed: %s:%d \"%s\" ", file, line, expr);
    if (msg) {
        va_list ap;
        va_start(ap, msg);
        std::vfprintf(stderr, msg, ap);
        va_end(ap);
    }
    std::fprintf(stderr, "\n");
    std::abort();
}

}

}
