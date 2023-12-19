#include "barrier.h"
#include <assert.h>

namespace ulib {
Barrier::Barrier(uint32_t num) : num_(num) {}

void
Barrier::Wait()
{
    MutexGuard guard(mutex_);
    assert(num_ > 0);

    if (num_ == 0) {
        cond_.NotifyAll();
    } else {
        while (num_ > 0) { cond_.Wait(guard); }
    }

    assert(num_ == 0);
}

}// namespace ulib
