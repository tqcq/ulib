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
