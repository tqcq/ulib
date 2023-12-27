#include "thread.h"
#include <climits>
#include <pthread.h>
#include "ulib/log/log.h"
#include "ulib/concorrency/countdown_latch.h"
#include <unistd.h>
#include <sys/syscall.h>
#include <exception>

namespace ulib {

pid_t
GetTid()
{
    // TODO cache tid
    return static_cast<pid_t>(::syscall(SYS_gettid));
}

class Thread::Impl {
public:
    Impl(const ThreadFunc &func, const std::string &thread_name)
        : thread_(0),
          started_(false),
          joined_(false),
          thread_name_(thread_name),
          func_(func),
          tid_(new pid_t(0)),
          latch_(1)
    {}

    ~Impl()
    {
        if (started_ && !joined_) { pthread_detach(thread_); }
    }

    static void *ThreadRun(void *obj)
    {
        Impl *impl = static_cast<Impl *>(obj);
        *impl->tid_ = GetTid();
        pthread_setname_np(impl->thread_name_.c_str());
        impl->latch_.CountDown();
        try {
            impl->func_();
        } catch (std::exception &ex) {
            // do nothing
            ULOG_ERROR("system.thread",
                       "exception caught in Thread %s, reson: %s",
                       impl->thread_name_.c_str(), ex.what());
        } catch (...) {
            // do nothing
        }
        return NULL;
    }

    void Start()
    {
        ULOG_ASSERT(!started_, "thread already started");
        started_ = true;
        if (pthread_create(&thread_, NULL, Impl::ThreadRun, this)) {
            started_ = false;
        } else {
            latch_.Await();
            ULOG_ASSERT(tid_.get(), "tid_ is null");
        }
    }

    int Join()
    {
        ULOG_ASSERT(started_, "thread not started");
        ULOG_ASSERT(!joined_, "thread already joined");
        joined_ = true;
        return pthread_join(thread_, NULL);
    }

    bool Started() const { return started_; }

    bool Tid() const { return *tid_; }

private:
    pthread_t thread_;
    bool started_;
    bool joined_;

    std::string thread_name_;
    ThreadFunc func_;
    std::shared_ptr<pid_t> tid_;
    CountDownLatch latch_;
};

Thread::Thread(const ThreadFunc &func, const std::string &thread_name)
    : impl_(new Impl(func, thread_name))
{}

Thread::Thread(Thread &&other) : impl_(other.impl_) { other.impl_ = nullptr; }

// Thread(ThreadFunc &&func, const std::string &thread_name = std::string());
Thread::~Thread() { delete impl_; }

void
Thread::Start()
{
    ULOG_ASSERT(impl_, "impl_ is null");
    impl_->Start();
}

int
Thread::Join()
{
    ULOG_ASSERT(impl_, "impl_ is null");
    return impl_->Join();
}

bool
Thread::Started() const
{
    ULOG_ASSERT(impl_, "impl_ is null");
    return impl_->Started();
}

pid_t
Thread::Tid() const
{
    ULOG_ASSERT(impl_, "impl_ is null");
    return impl_->Tid();
}
}// namespace ulib
