//
// Created by tqcq on 2023/11/16.
//

#ifndef ULIB_SRC_SYNCHRONIZATION_EVENT_H_
#define ULIB_SRC_SYNCHRONIZATION_EVENT_H_

#include "units/time_delta.h"
#include <pthread.h>

namespace tqcq {

class Event {
 public:
  static const TimeDelta kForever; // = TimeDelta::PlusInfinity();
  Event();
  Event(bool manua__reset, bool initially_signaled);
  Event(const Event &) = delete;
  ~Event();

  void Set();
  void Reset();

  bool Wait(TimeDelta give_up_after, TimeDelta warn_after);
  bool Wait(TimeDelta give_up_after) {
    // 如果是无限等待，那么超过3秒就可能是死锁了
    // 如果使用者有明确的等待时间，那么不用警告
    return Wait(give_up_after, give_up_after.IsPlusInfinity()
                               ? TimeDelta::Seconds(3)
                               : kForever);
  }
 private:
  const bool is_manual_reset_;
  bool event_status_;
  pthread_mutex_t mutex_;
    pthread_cond_t cond_;
};

} // namespace tqcq

#endif //ULIB_SRC_SYNCHRONIZATION_EVENT_H_
