#include <gtest/gtest.h>
#include <ulib/concorrency/barrier.h>
#include <pthread.h>

void *
BarrierThread(void *arg)
{
    ulib::Barrier *barrier = (ulib::Barrier *) arg;
    barrier->Wait();
    return NULL;
}

TEST(Barrier, Barrier)
{
    ulib::Barrier barrier(1);
    barrier.Wait();
}

TEST(Barrier, Barrier2)
{
    ulib::Barrier barrier(2);
    pthread_t thread;
    pthread_create(&thread, NULL, BarrierThread, &barrier);
    barrier.Wait();
    pthread_join(thread, NULL);
}

TEST(Barrier, MultiThreadBarrier)
{
#define THREAD_NUM 30
    pthread_t threads[THREAD_NUM];
    ulib::Barrier barrier(THREAD_NUM);
    for (int i = 0; i < THREAD_NUM - 1; ++i) { pthread_create(&threads[i], NULL, BarrierThread, &barrier); }
    barrier.Wait();
    for (int i = 0; i < THREAD_NUM - 1; ++i) { pthread_join(threads[i], NULL); }
}
