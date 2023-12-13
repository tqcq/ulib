//
// Created by Feng Zhang on 2023/12/12.
//

#ifndef ULIB_SRC_ULIB_CONCORRENCY_INTERNAL_MUTEX_IMPL_H_
#define ULIB_SRC_ULIB_CONCORRENCY_INTERNAL_MUTEX_IMPL_H_

#include <pthread.h>

namespace ulib {
namespace detail {
class MutexImpl {
public:
    MutexImpl() { pthread_mutex_init(&mutex_, NULL); }

    ~MutexImpl() { pthread_mutex_destroy(&mutex_); }

    void Lock() { pthread_mutex_lock(&mutex_); }

    void Unlock() { pthread_mutex_unlock(&mutex_); }

    bool TryLock() { return pthread_mutex_trylock(&mutex_) == 0; }

private:
    friend class ConditionVariableImpl;
    pthread_mutex_t mutex_;
};
} // namespace detail
} // namespace ulib

#endif//ULIB_SRC_ULIB_CONCORRENCY_INTERNAL_MUTEX_IMPL_H_
