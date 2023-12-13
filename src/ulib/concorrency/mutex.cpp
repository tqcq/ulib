//
// Created by Feng Zhang on 2023/12/12.
//

#include "mutex.h"
#include "internal/mutex_impl.h"

namespace ulib {

Mutex::Mutex() : impl_(new detail::MutexImpl) {}

Mutex::~Mutex() { delete impl_; }

void
Mutex::Lock()
{
    impl_->Lock();
}

void
Mutex::Unlock()
{
    impl_->Unlock();
}

bool
Mutex::TryLock()
{
    return impl_->TryLock();
}

MutexGuard::MutexGuard(Mutex &mutex) : mutex_(mutex) { mutex_.Lock(); }

MutexGuard::~MutexGuard() { mutex_.Unlock(); }

}// namespace ulib
