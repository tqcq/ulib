#include "ulib/base/types.h"
#include "ulib/concorrency/mutex.h"
#include "ulib/concorrency/event.h"

namespace ulib {
class CountDownLatch {
public:
    CountDownLatch(uint32_t count);
    void CountDown();
    void Await();

private:
    ulib::int64_t count_;
    Mutex count_lock_;
    ConditionVariable count_cond_;
};
}// namespace ulib
