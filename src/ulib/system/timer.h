#ifndef ULIB_SRC_ULIB_SYSTEM_TIMER_H_
#define ULIB_SRC_ULIB_SYSTEM_TIMER_H_

#include "ulib/base/noncopyable.h"
#include "ulib/base/types.h"
#include "ulib/concorrency/mutex.h"
#include "ulib/concorrency/condition_variable.h"
#include "ulib/system/thread.h"
#include <functional>
#include <atomic>
#include <set>

namespace ulib {

using TimerId = uint64_t;

class Timer {
public:
    const uint64_t INVALID_TIMER_ID = 0;
    using ExpireCallback = std::function<void()>;
    Timer(uint64_t when, uint64_t intarval);
    // void Start();
    // void Stop();
    ExpireCallback & on_expire() { return expire_callback_; }
    const TimerId id() const { return timer_id_; }
    uint64_t when() const { return when_; }
    uint64_t interval() const { return interval_; }
    bool auto_reset() const { return auto_reset_; }
    bool operator < (const Timer& rhs) const { return timer_id_ < rhs.timer_id_; }
    bool operator == (const Timer& rhs) const { return timer_id_ == rhs.timer_id_; }

private:
    static uint64_t NextTimerId();
    const TimerId timer_id_;
    // micro seconds
    uint64_t when_;
    // micro seconds
    uint64_t interval_;
    bool auto_reset_;
    ExpireCallback expire_callback_;
    static std::atomic<uint64_t> next_timer_id_;
};



class TimerManager : NonCopyable {
public:
    using Entry = std::pair<uint64_t, Timer*>;
    TimerManager();
    ~TimerManager();
    static TimerManager& Instance();
    bool Initialize();
    static TimerId AddTimer(const Timer::ExpireCallback& cb, uint64_t run_after_ms, uint64_t interval = 0);
    static void CancelTimer(TimerId timer_id);

    void Loop();
private:
    TimerId AddTimerImpl(const Timer::ExpireCallback& cb, uint64_t run_after_ms, uint64_t interval = 0);
    void CancelTimerImpl(TimerId timer_id);
    friend class Timer;
    bool stopped_;
    Thread thread_;

    Mutex timers_lock_;
    ConditionVariable timers_cond_;
    std::set<Entry> timers_;

    Mutex timer_inserting_set_lock_;
    std::set<Entry> timer_inserting_set_;
    Mutex timer_canceling_set_lock_;
    std::set<TimerId> timer_canceling_set_;
};

} // namespace ulib

#endif//ULIB_SRC_ULIB_SYSTEM_TIMER_H_
