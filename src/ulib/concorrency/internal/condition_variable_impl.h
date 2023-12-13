//
// Created by Feng Zhang on 2023/12/12.
//

#ifndef ULIB_SRC_ULIB_CONCORRENCY_INTERNAL_CONDITION_VARIABLE_IMPL_H_
#define ULIB_SRC_ULIB_CONCORRENCY_INTERNAL_CONDITION_VARIABLE_IMPL_H_

#include "ulib/base/types.h"
#include "mutex_impl.h"
#include "ulib/concorrency/mutex.h"
#include <algorithm>
#include <pthread.h>
#include <errno.h>

namespace ulib {
namespace detail {

class ConditionVariableImpl {
public:
    ConditionVariableImpl() { pthread_cond_init(&cond_, NULL); }

    ~ConditionVariableImpl() { pthread_cond_destroy(&cond_); }

    void NotifyOne() { pthread_cond_signal(&cond_); }

    void NotifyAll() { pthread_cond_broadcast(&cond_); }

    void Wait(MutexImpl &mutex_impl) { pthread_cond_wait(&cond_, &mutex_impl.mutex_); }

    template<class Predicate>
    void Wait(MutexImpl &mutex_impl, Predicate p)
    {
        while (!p()) { Wait(mutex_impl); }
    }

    bool WaitForMilliseconds(MutexImpl &mutex_impl, uint32_t wait_time)
    {
        struct timespec ts;
        ts.tv_sec = 0;
        ts.tv_nsec = wait_time * 1000000;
        return ETIMEDOUT != pthread_cond_timedwait(&cond_, &mutex_impl.mutex_, &ts);
    }

    template<class Predicate>
    bool WaitForMilliseconds(MutexImpl &mutex_impl, uint32_t wait_time, Predicate p)
    {
        if (!p()) { WaitForMilliseconds(mutex_impl, wait_time); }
        return p();
    }

private:
    pthread_cond_t cond_;
};

} // namespace detail
}// namespace ulib

#endif//ULIB_SRC_ULIB_CONCORRENCY_INTERNAL_CONDITION_VARIABLE_IMPL_H_
