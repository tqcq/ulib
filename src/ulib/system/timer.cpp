//
// Created by Feng Zhang on 2023/12/27.
//

#include "timer.h"
#include <unistd.h>
#include <mutex>
#include <sys/time.h>
#include "ulib/log/log.h"

namespace ulib {

uint64_t
TimeNowInMicroSeconds()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000ULL + tv.tv_usec;
}

std::atomic_uint64_t Timer::next_timer_id_(1);

Timer::Timer(uint64_t when, uint64_t interval)
    : timer_id_(next_timer_id_.fetch_add(1)),
      when_(when),
      interval_(interval),
      auto_reset_(interval_ > 0),
      expire_callback_()
{}

TimerManager::TimerManager()
    : stopped_(false),
      thread_(std::bind(&TimerManager::Loop, this), "timer_manager_thread"),
      timers_lock_(),
      timers_cond_(),
      timers_(),
      timer_inserting_set_lock_(),
      timer_inserting_set_(),
      timer_canceling_set_lock_(),
      timer_canceling_set_()
{}

TimerId
TimerManager::AddTimerImpl(const Timer::ExpireCallback &cb,
                       uint64_t run_after_ms,
                       uint64_t interval)
{
    MutexGuard guard(timer_inserting_set_lock_);
    auto *timer =
        new Timer(TimeNowInMicroSeconds() + run_after_ms * 1000, interval * 1000);
    timer->on_expire() = cb;
    if (!timer_inserting_set_.insert({timer->when(), timer}).second) {
        ULOG_ERROR("system.timer_manager", "Add timer failed, already exists timer_id={}", timer->id());
        delete timer;
        return 0;
    }

    if (timers_lock_.TryLock()) {
        timers_cond_.NotifyAll();
        timers_lock_.Unlock();
    }
    return timer->id();
}

void
TimerManager::CancelTimerImpl(TimerId timer_id)
{
    MutexGuard guard(timer_canceling_set_lock_);
    timer_canceling_set_.insert(timer_id);
    if (timers_lock_.TryLock()) {
        timers_cond_.NotifyAll();
        timers_lock_.Unlock();
    }
}

void
TimerManager::Loop()
{
    while (!stopped_) {
        // ULOG_TRACE("system.timer_manager", "now: {}", TimeNowInMicroSeconds());
        // Thread::Sleep(3000000);
        MutexGuard timers_guard(timers_lock_);
        // add timer
        {
            std::set<Entry> timer_inserting_set;
            {
                MutexGuard insert_guard(timer_inserting_set_lock_);
                timer_inserting_set.swap(timer_inserting_set_);
            }
            timers_.insert(timer_inserting_set.begin(),
                           timer_inserting_set.end());
        }
        // cancel timer
        {
            // TODO imporve
            MutexGuard cancel_guard(timer_canceling_set_lock_);
            for (auto iter = timers_.begin(); !timer_canceling_set_.empty() && iter != timers_.end();) {
                if (timer_canceling_set_.find(iter->second->id())
                    != timer_canceling_set_.end()) {
                    delete iter->second;
                    iter = timers_.erase(iter);
                } else {
                    ++iter;
                }
            }
            timer_canceling_set_.clear();
        }

        uint64_t now = TimeNowInMicroSeconds();
        // check timers
        // ULOG_TRACE("system.timer_manager","wait for {}-{}={} micro seconds",  timers_.begin()->first, now, timers_.begin()->first - now);
        if (timers_.empty()) {
            timers_cond_.WaitForMilliseconds(timers_guard, 1000);
        } else if (timers_.begin()->first > now) {
            do {
                uint64_t wait_time_ms = (timers_.begin()->first - now) / 1000;
                timers_cond_.WaitForMilliseconds(timers_guard, wait_time_ms);
                now = TimeNowInMicroSeconds();
            } while(timers_.begin()->first > now);
        } else {
            ULOG_TRACE("system.timer_manager", "need trigger set size: {}", timers_.size());
            std::set<Entry> expired_timers;
            while (!timers_.empty() && timers_.begin()->first <= now) {
                expired_timers.insert(*timers_.begin());
                timers_.erase(timers_.begin());
            }

            for (auto iter = expired_timers.begin();
                 iter != expired_timers.end(); ++iter) {
                uint64_t when = iter->first;
                Timer* timer = iter->second;
                try {
                    timer->on_expire()();
                    if (timer->auto_reset()) {
                        timers_.insert({when + timer->interval(), timer});
                    } else {
                        delete timer;
                    }
                } catch (const std::exception &e) {
                    ULOG_ERROR("system.timer",
                               "timer crash: timer_id={}, reason: {}",
                               iter->second->id(), e.what());
                } catch (...) {
                    // do nothing
                }
            }
        }
    }
}

bool
TimerManager::Initialize()
{
    ULOG_ASSERT(!thread_.Started(), "double initialize");
    thread_.Start();
    return thread_.Started();
}

TimerManager::~TimerManager()
{
    stopped_ = true;
    thread_.Join();
}

TimerManager &
TimerManager::Instance()
{
    static TimerManager timer_manager;
    static std::once_flag init_flag;
    std::call_once(init_flag, [&]{
        timer_manager.Initialize();
    });
    return timer_manager;
}

TimerId
TimerManager::AddTimer(const Timer::ExpireCallback &cb,
                  uint64_t run_after_ms,
                  uint64_t interval)
{
    return Instance().AddTimerImpl(cb, run_after_ms, interval);
}

void
TimerManager::CancelTimer(TimerId timer_id)
{
    Instance().CancelTimerImpl(timer_id);
}
}// namespace ulib
