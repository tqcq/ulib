//
// Created by tqcq on 2023/11/16.
//

#include "task_queue_base.h"

namespace tqcq {

namespace {
thread_local TaskQueueBase *current_task_queue = nullptr;
}

TaskQueueBase::CurrentTaskQueueSetter::CurrentTaskQueueSetter(TaskQueueBase *task_queue)
    : previous_(current_task_queue) {
  current_task_queue = task_queue;
}

TaskQueueBase::CurrentTaskQueueSetter::~CurrentTaskQueueSetter() {
    current_task_queue = previous_;
}

} // namespace tqcq
