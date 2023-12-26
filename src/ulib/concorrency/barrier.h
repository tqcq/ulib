#ifndef ULIB_SRC_ULIB_CONCORRENCY_BARRIER_H_
#define ULIB_SRC_ULIB_CONCORRENCY_BARRIER_H_

#include "ulib/concorrency/mutex.h"
#include "ulib/concorrency/condition_variable.h"

namespace ulib {
class Barrier {
public:
    Barrier(uint32_t num);
    void Wait();

private:
    uint32_t num_;
    Mutex mutex_;
    ConditionVariable cond_;
};

}// namespace ulib

#endif//ULIB_SRC_ULIB_CONCORRENCY_BARRIER_H_
