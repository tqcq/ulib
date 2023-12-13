//
// Created by Feng Zhang on 2023/12/13.
//

#include "event.h"

ulib::Event::Event() : manual_reset_(false), event_status_(false) {}

ulib::Event::Event(bool manual_reset, bool initially_signaled)
    : manual_reset_(manual_reset),
      event_status_(initially_signaled)
{}

ulib::Event::~Event() {}

void
ulib::Event::Set()
{
    MutexGuard guard(mutex_);
    event_status_ = true;
    cond_.NotifyAll();
}

void
ulib::Event::Reset()
{
    MutexGuard guard(mutex_);
    event_status_ = false;
}

bool
ulib::Event::Wait(int give_up_after_ms)
{
    MutexGuard guard(mutex_);
    if (event_status_) { return true; }

    IsEventSetChecker checker(*this);
    if (give_up_after_ms <= 0) {
        cond_.Wait(guard, checker);
    } else {
        cond_.WaitForMilliseconds(guard, give_up_after_ms, checker);
    }

    if (event_status_) {
        if (manual_reset_) { event_status_ = false; }
        return true;
    } else {
        return false;
    }
}

ulib::Event::IsEventSetChecker::IsEventSetChecker(const ulib::Event &event) : event_(event) {}

bool
ulib::Event::IsEventSetChecker::operator()() const
{
    return event_.event_status_;
}
