#include <ulib/system/thread_pool.h>
#include <gtest/gtest.h>
#include <memory>

class ThreadPoolTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        thread_pool_ = std::make_unique<ulib::ThreadPool>(10);
    }

    std::unique_ptr<ulib::ThreadPool> thread_pool_;
};

TEST_F(ThreadPoolTest, Submit)
{
    auto future = thread_pool_->Submit([]() { return 1; });
    ASSERT_EQ(future.get(), 1);
}

TEST_F(ThreadPoolTest, SubmitWithArgs)
{
    auto future =
        thread_pool_->Submit([](int a, int b) { return a + b; }, 1, 2);
    ASSERT_EQ(future.get(), 3);
}

TEST_F(ThreadPoolTest, MultiTask)
{
    std::vector<std::future<int>> futures;
    for (int i = 0; i < 1000; ++i) {
        futures.emplace_back(
            thread_pool_->Submit([](int a, int b) { return a + b; }, 1, 2));
    }

    for (auto &future : futures) { ASSERT_EQ(future.get(), 3); }
}

TEST_F(ThreadPoolTest, NoArgs)
{
    std::vector<std::future<int>> futures;
    for (int i = 0; i < 1000; ++i) {
        futures.emplace_back(thread_pool_->Submit([]() {
            ulib::Thread::Sleep(1000);
            return 1;
        }));
    }
    for (auto &future : futures) { future.get(); }
}
