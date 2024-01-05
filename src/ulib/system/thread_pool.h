#ifndef ULIB_SRC_ULIB_SYSTEM_THREAD_POOL_H_
#define ULIB_SRC_ULIB_SYSTEM_THREAD_POOL_H_

#include "thread.h"
#include <fmt/format.h>
#include "ulib/log/log.h"
#include "ulib/concorrency/condition_variable.h"
#include "ulib/concorrency/mutex.h"
#include <future>
#include <vector>
#include <queue>

namespace ulib {
class ThreadPool {
public:
    ThreadPool(int max_thread_num,
               int init_thread_num = 1,
               const std::string &thread_pool_name = std::string());
    ThreadPool(const ThreadPool &) = delete;
    ThreadPool(ThreadPool &&) = delete;
    ThreadPool &operator=(const ThreadPool &) = delete;
    ~ThreadPool();

    void Stop();
    void Join();

    template<typename F, typename... Args>
    auto Submit(F &&f, Args &&...args)
        -> std::future<typename std::result_of<F(Args...)>::type>
    {
        using return_type = typename std::result_of<F(Args...)>::type;
        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...));

        std::future<return_type> res = task->get_future();
        {
            MutexGuard guard(tasks_lock_);
            ULOG_ASSERT(!stopped_, "thread pool is stopped");

            tasks_.emplace([task]() { (*task)(); });
            if (tasks_.size() > idle_thread_num_
                && workers_.size() < max_thread_num_) {
                AddThread(thread_pool_name_
                          + fmt::format("{}", workers_.size()));
            }
            if (tasks_.size() > 1) {
                tasks_cond_.NotifyAll();
            } else {
                tasks_cond_.NotifyOne();
            }
        }
        return res;
    }

private:
    void AddThread(const std::string &&thread_name);

    int max_thread_num_;
    int idle_thread_num_;
    std::string thread_pool_name_;
    bool stopped_;
    bool joined_;
    std::vector<Thread> workers_;
    std::queue<Thread::ThreadFunc> tasks_;
    Mutex tasks_lock_;
    ConditionVariable tasks_cond_;
};
}// namespace ulib
#endif// ULIB_SRC_ULIB_SYSTEM_THREAD_POOL_H_
