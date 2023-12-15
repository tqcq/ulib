#include "countdown_latch.h"
#include <assert.h>

namespace ulib {

CountDownLatch::CountDownLatch(uint32_t count) : count_(count) {
    assert(count > 0);
}

void
CountDownLatch::CountDown()
{
    MutexGuard guard(count_lock_);
    assert(count_ > 0);
    --count_;
    if (count_ == 0) { count_cond_.NotifyAll(); }
}

void
CountDownLatch::Await()
{
    if (count_ == 0) { return; }
    MutexGuard guard(count_lock_);
    while (count_ > 0) { count_cond_.Wait(guard); }
    assert(count_ == 0);
}

}// namespace ulib
