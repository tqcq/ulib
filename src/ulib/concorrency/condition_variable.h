#ifndef ULIB_SRC_ULIB_CONCORRENCY_CONDITION_VARIABLE_H_
#define ULIB_SRC_ULIB_CONCORRENCY_CONDITION_VARIABLE_H_

#include "mutex.h"
#include "ulib/base/types.h"

namespace ulib {

namespace detail {
class ConditionVariableImpl;
}

class ConditionVariable {
public:
    ConditionVariable();
    ~ConditionVariable();

    void NotifyOne();
    void NotifyAll();

    void Wait(MutexGuard &guard);
    bool WaitForMilliseconds(MutexGuard &guard, uint32_t wait_time);

    template<class Predicate>
    void Wait(MutexGuard &guard, Predicate p)
    {
        while (!p()) { Wait(guard); }
    }

    template<class Predicate>
    bool WaitForMilliseconds(MutexGuard &guard, uint32_t wait_time, Predicate p)
    {
        if (!p()) { WaitForMilliseconds(guard, wait_time); }
        return p();
    }

private:
    detail::ConditionVariableImpl *impl_;
};

}// namespace ulib

#endif//ULIB_SRC_ULIB_CONCORRENCY_CONDITION_VARIABLE_H_
