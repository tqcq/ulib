#include <gtest/gtest.h>
#include <ulib/system/timer.h>
#include <ulib/system/thread.h>
#include <ulib/concorrency/countdown_latch.h>
#include <ulib/log/log.h>

class TimerTest : public ::testing::Test {
protected:
    void SetUp() override { latch_ = nullptr; }
    void TearDown() override { delete latch_; }

protected:
    ulib::CountDownLatch *latch_;
};

TEST_F(TimerTest, OnceTrigger) {
    latch_ = new ulib::CountDownLatch(1);
    ulib::TimerId timer_id = ulib::TimerManager::AddTimer([&]{
        latch_->CountDown();
    }, 1000);
    latch_->Await();
    ulib::TimerManager::CancelTimer(timer_id);
}

TEST_F(TimerTest, PeriodTrigger) {
    latch_ = new ulib::CountDownLatch(3);
    std::atomic_int cnt(3);
    ulib::TimerId timer_id = ulib::TimerManager::AddTimer([&]{
        if (cnt.fetch_sub(1) > 0) {
            ULOG_INFO("timer.unittest", "start count down: {}", cnt);
            latch_->CountDown();
        }
    }, 1000, 3000);
    latch_->Await();
    ulib::TimerManager::CancelTimer(timer_id);
}