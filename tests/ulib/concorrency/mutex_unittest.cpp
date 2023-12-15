#include <gtest/gtest.h>
#include <ulib/concorrency/mutex.h>

TEST(MutexTest, MutexTest)
{
    ulib::Mutex mutex;
    EXPECT_TRUE(mutex.TryLock());
    EXPECT_FALSE(mutex.TryLock());
    mutex.Unlock();
}
