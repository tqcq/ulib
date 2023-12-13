//
// Created by Feng Zhang on 2023/12/12.
//

#include "condition_variable.h"
#include "internal/condition_variable_impl.h"

namespace ulib {

ConditionVariable::ConditionVariable() : impl_(new detail::ConditionVariableImpl) {}

ConditionVariable::~ConditionVariable() { delete impl_; }

void
ConditionVariable::NotifyOne()
{
    impl_->NotifyOne();
}

void
ConditionVariable::NotifyAll()
{
    impl_->NotifyAll();
}

void
ConditionVariable::Wait(MutexGuard &guard)
{
    impl_->Wait(*guard.mutex_.impl_);
}

bool
ConditionVariable::WaitForMilliseconds(MutexGuard &guard, uint32_t wait_time)
{
    return impl_->WaitForMilliseconds(*guard.mutex_.impl_, wait_time);
}

}