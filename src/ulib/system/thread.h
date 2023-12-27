#ifndef ULIB_SRC_ULIB_SYSTEM_THREAD_H_
#define ULIB_SRC_ULIB_SYSTEM_THREAD_H_
#include "ulib/base/types.h"
#include <functional>
#include <string>

namespace ulib {
class Thread {
public:
    using ThreadFunc = std::function<void()>;
    Thread(const ThreadFunc &func,
           const std::string &thread_name = std::string());
    // Thread(ThreadFunc &&func, const std::string &thread_name = std::string());
    Thread(Thread &&);
    ~Thread();

    void Start();
    int Join();

    bool Started() const;
    pid_t Tid() const;
    const std::string &name() const;

private:
    class Impl;
    Impl *impl_;
};

}// namespace ulib
#endif// ULIB_SRC_ULIB_SYSTEM_THREAD_H_
