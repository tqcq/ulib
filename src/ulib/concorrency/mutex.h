#ifndef ULIB_SRC_ULIB_CONCORRENCY_MUTEX_H_
#define ULIB_SRC_ULIB_CONCORRENCY_MUTEX_H_

namespace ulib {

namespace detail {
class MutexImpl;
}

class Mutex {
public:
    Mutex();
    ~Mutex();
    void Lock();
    void Unlock();
    bool TryLock();

private:
    Mutex(const Mutex &);
    Mutex &operator=(const Mutex &);

    friend class ConditionVariable;
    detail::MutexImpl *impl_;
};

class MutexGuard {
public:
    MutexGuard(Mutex &);
    ~MutexGuard();

private:
    MutexGuard(const MutexGuard &);
    MutexGuard &operator=(const MutexGuard &);

    friend class ConditionVariable;
    Mutex &mutex_;
};

}// namespace ulib

#endif//ULIB_SRC_ULIB_CONCORRENCY_MUTEX_H_
