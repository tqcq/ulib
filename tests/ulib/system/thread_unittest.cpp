#include <ulib/system/thread.h>
#include <gtest/gtest.h>
#include <ulib/concorrency/countdown_latch.h>

TEST(Thread, Start)
{
    volatile int i = 0;
    ulib::Thread thread([&]() {
        EXPECT_EQ(i, 0);
        i = 1;
        EXPECT_EQ(i, 1);
    });
    EXPECT_EQ(i, 0);
    thread.Start();
    thread.Join();
    EXPECT_EQ(i, 1);
}
