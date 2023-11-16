//
// Created by tqcq on 2023/11/16.
//

#include "event.h"
#include <pthread.h>
#include <sys/time.h>

namespace tqcq {

const TimeDelta Event::kForever = TimeDelta::PlusInfinity();

Event::Event() : Event(false, false) {}

Event::Event(bool manua_reset, bool initially_signaled)
    : is_manual_reset_(manua_reset), event_status_(initially_signaled) {
  pthread_mutex_init(&mutex_, nullptr);
  pthread_cond_init(&cond_, nullptr);
}

Event::~Event() {
  pthread_mutex_destroy(&mutex_);
  pthread_cond_destroy(&cond_);
}

void Event::Set() {
  pthread_mutex_lock(&mutex_);
  event_status_ = true;
  pthread_cond_broadcast(&cond_);
  pthread_mutex_unlock(&mutex_);
}

void Event::Reset() {
  pthread_mutex_lock(&mutex_);
  event_status_ = false;
  pthread_mutex_unlock(&mutex_);
}

namespace {
timespec GetTimespec(TimeDelta duration_from_now) {
  timespec ts;
  // use clock ?
  if (false) {
    // clock_gettime(CLOCK_MONOTONIC, &ts);
  } else {
    timeval tv;
    gettimeofday(&tv, nullptr);
    ts.tv_sec = tv.tv_sec;
    ts.tv_nsec = tv.tv_usec * 1000;
  }

  int64_t microsecs_from_now = duration_from_now.us();
  ts.tv_sec += microsecs_from_now / 1000000;
  ts.tv_nsec += (microsecs_from_now % 1000000) * 1000;

  if (ts.tv_nsec >= 1000000000) {
    ts.tv_sec++;
    ts.tv_nsec -= 1000000000;
  }

  return ts;
}
}

bool Event::Wait(TimeDelta give_up_after, TimeDelta warn_after) {
  timespec ts;

  int error = 0;
  pthread_mutex_lock(&mutex_);
  if (give_up_after.IsPlusInfinity()) {
    error = pthread_cond_wait(&cond_, &mutex_);
  } else {
    ts = GetTimespec(give_up_after);
    error = pthread_cond_timedwait(&cond_, &mutex_, &ts);
  }

  return error;
}

} // namespace tqcq
