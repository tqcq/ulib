#include <gtest/gtest.h>
#include <ulib/concorrency/countdown_latch.h>
#include <pthread.h>

TEST(CountdownLatch, CountDownLatch)
{
    ulib::CountDownLatch latch(1);
    latch.CountDown();
    latch.Await();
}

TEST(CountdownLatch, CountDownLatch2)
{
    ulib::CountDownLatch latch(2);
    latch.CountDown();
    latch.CountDown();
    latch.Await();
}

void *
CountDownThread(void *arg)
{
    ulib::CountDownLatch *latch = (ulib::CountDownLatch *) arg;
    latch->CountDown();
    return NULL;
}

TEST(CountdownLatch, DoubleCountDownLatch)
{
    ulib::CountDownLatch latch(2);
    pthread_t thread;
    pthread_create(&thread, NULL, CountDownThread, &latch);
    latch.CountDown();
    latch.Await();
    pthread_join(thread, NULL);
}

TEST(CountdownLatch, MultiThreadCountDownLatch)
{
#define THREAD_NUM 10
    ulib::CountDownLatch latch(THREAD_NUM);
    pthread_t thread[THREAD_NUM];
    for (int i = 0; i < THREAD_NUM; ++i) { pthread_create(&thread[i], NULL, CountDownThread, &latch); }
    latch.Await();
    for (int i = 0; i < THREAD_NUM; ++i) { pthread_join(thread[i], NULL); }
}

TEST(CoundownLatch, Assert)
{
    ulib::CountDownLatch latch(1);
    latch.CountDown();
    // NOTE https://github.com/google/googletest/blob/main/docs/advanced.md#death-tests-and-threads
    GTEST_FLAG_SET(death_test_style, "threadsafe");
    EXPECT_DEATH(latch.CountDown(), ".*failed.*");
}
