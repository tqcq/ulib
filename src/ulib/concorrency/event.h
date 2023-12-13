//
// Created by Feng Zhang on 2023/12/13.
//

#ifndef ULIB_SRC_ULIB_CONCORRENCY_EVENT_H_
#define ULIB_SRC_ULIB_CONCORRENCY_EVENT_H_

#include "mutex.h"
#include "condition_variable.h"

namespace ulib {
class Event {
public:
    Event();
    Event(bool manual_reset, bool initially_signaled);
    ~Event();

    void Set();
    void Reset();
    /**
     * @brief
     * @param give_up_after_ms -1, always wait, > 0 set timeout
     * @return
     */
    bool Wait(int give_up_after_ms = -1);

private:
    class IsEventSetChecker {
    public:
        IsEventSetChecker(const Event &);
        bool operator()() const;

    private:
        const Event &event_;
    };

    Event(const Event &);
    Event &operator=(const Event &);

    const bool manual_reset_;
    Mutex mutex_;
    ConditionVariable cond_;
    bool event_status_;

    friend class IsEventSetChecker;
};
}// namespace ulib

#endif//ULIB_SRC_ULIB_CONCORRENCY_EVENT_H_
