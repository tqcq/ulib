#include <ulib/concorrency/condition_variable.h>
#include <ulib/concorrency/mutex.h>
#include <ulib/log/log.h>
#include <gtest/gtest.h>
#include <ulib/concorrency/event.h>
#include <pthread.h>
#include <vector>

class EventTest : public ::testing::Test {
public:
    void SetUp() override
    {
        EXPECT_FALSE(event_.Wait(100));

        consumer_count_ = 0;
    }

    ulib::Event event_;
    ulib::Mutex mutex_;
    ulib::ConditionVariable cond_;

    int32_t consumer_count_;
};

void *
Consumer(void *args)
{
    EventTest *test = (EventTest *) args;
    {
        ulib::MutexGuard guard(test->mutex_);
        ++test->consumer_count_;
        test->cond_.NotifyAll();
    }
    EXPECT_TRUE(test != NULL);
    test->event_.Wait();
    {
        ulib::MutexGuard guard(test->mutex_);
        --test->consumer_count_;
        test->cond_.NotifyAll();
    }

    return NULL;
}

TEST_F(EventTest, multi_thread_wait_event)
{
    std::vector<pthread_t> threads(10);
    for (int i = 0; i < threads.size(); i++) { pthread_create(&threads[i], NULL, Consumer, this); }

    ulib::MutexGuard guard(mutex_);
    while (consumer_count_ < threads.size()) { cond_.Wait(guard); }
    EXPECT_EQ(consumer_count_, threads.size());

    {
        event_.Set();
        while (consumer_count_ > 0) { cond_.Wait(guard); }
    }

    EXPECT_EQ(consumer_count_, 0);
    for (int i = 0; i < threads.size(); i++) { pthread_join(threads[i], NULL); }
}
