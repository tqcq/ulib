//
// Created by tqcq on 2023/11/16.
//

#ifndef ULIB_SRC_BASE_TASK_QUEUE_BASE_H_
#define ULIB_SRC_BASE_TASK_QUEUE_BASE_H_

#include <functional>
#include "location.h"
#include "units/time_delta.h"

namespace tqcq {

class TaskQueueBase {
 public:
  enum class DelayPrecision {
    kLow,
    kHigh,
  };

  // 销毁队列
  virtual void Delete() = 0;
  void PostTask(std::function<void()> task,
                const Location &location = Location::Current()) {
    PostTaskImpl(std::move(task), {}, location);
  };

  void PostDelayedTask(std::function<void()> task,
                       TimeDelta delay,
                       const Location &location = Location::Current()) {
    PostDelayedTaskImpl(std::move(task), delay, {}, location);
  };
  void PostDelayedHighPrecisionTask(
      std::function<void()> task,
      TimeDelta delay,
      const Location &location = Location::Current()) {
    PostDelayedTaskTraits traits;
    traits.high_precision = true;
    PostDelayedTaskImpl(std::move(task), delay, traits, location);
  }

  void PostDelayedTaskWithPrecision(
      DelayPrecision precision,
      std::function<void()> task,
      TimeDelta delay,
      const Location &location = Location::Current()) {
    switch (precision) {
      case DelayPrecision::kLow:PostDelayedTask(std::move(task), delay, location);
        break;
      case DelayPrecision::kHigh:PostDelayedHighPrecisionTask(std::move(task), delay, location);
        break;
    }
  };

  static TaskQueueBase *Current();
  bool IsCurrent() const { return Current() == this; }

 protected:
  struct PostTaskTraits {};
  struct PostDelayedTaskTraits {
    bool high_precision = false;
  };

  virtual void PostTaskImpl(std::function<void()> task,
                            const PostTaskTraits &traits,
                            const Location &location) = 0;
  virtual void PostDelayedTaskImpl(std::function<void()> task,
                                   TimeDelta delay,
                                   const PostDelayedTaskTraits &traits,
                                   const Location &location) = 0;

  class CurrentTaskQueueSetter {
   public:
    explicit CurrentTaskQueueSetter(TaskQueueBase *task_queue);
    CurrentTaskQueueSetter(const CurrentTaskQueueSetter &) = delete;
    CurrentTaskQueueSetter &operator=(const CurrentTaskQueueSetter &) = delete;
    ~CurrentTaskQueueSetter();
   private:
    TaskQueueBase *const previous_;
  };
};

struct TaskQueueDeleter {
  void operator()(TaskQueueBase *task_queue) const {
    task_queue->Delete();
  }
};

} // namespace tqcq

#endif //ULIB_SRC_BASE_TASK_QUEUE_BASE_H_
