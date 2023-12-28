#include "thread_pool.h"
#include <fmt/format.h>

namespace ulib {
ThreadPool::ThreadPool(int max_thread_num,
                       int init_thread_num,
                       const std::string &thread_pool_name)
    : max_thread_num_(max_thread_num),
      idle_thread_num_(0),
      thread_pool_name_(thread_pool_name),
      stopped_(false),
      joined_(false)
{
    ULOG_ASSERT(max_thread_num_ > 0, "max_thread_num_ must be greater than 0");
    workers_.reserve(max_thread_num_);
    for (int i = 0; i < init_thread_num; ++i) {
        AddThread(thread_pool_name_ + fmt::format("{}", i));
    }
}

void
ThreadPool::AddThread(const std::string &&thread_name)
{
    workers_.emplace_back(
        [this]() {
            while (true) {
                Thread::ThreadFunc task;
                {
                    MutexGuard guard(tasks_lock_);
                    ++idle_thread_num_;
                    while (tasks_.empty() && !stopped_) {
                        tasks_cond_.Wait(guard);
                    }
                    --idle_thread_num_;
                    if (tasks_.empty() && stopped_) { break; }
                    task = tasks_.front();
                    tasks_.pop();
                }
                task();
            }
        },
        thread_name);
    workers_.back().Start();
}

ThreadPool::~ThreadPool()
{
    if (!stopped_) { Stop(); }
    if (!joined_) { Join(); }
}

void
ThreadPool::Stop()
{
    MutexGuard guard(tasks_lock_);
    stopped_ = true;
    tasks_cond_.NotifyAll();
}

void
ThreadPool::Join()
{
    joined_ = true;
    for (auto &worker : workers_) { worker.Join(); }
}

}// namespace ulib
