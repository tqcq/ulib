#include <gtest/gtest.h>
#include <eventbus.h>

TEST(EventBus, BaseTest)
{
    struct Event {
        int x;
    };

    ulib::eventbus bus;
    int calls = 0;
    ulib::scoped_subscription<Event> ss(bus, [&](const Event &e) noexcept {
        EXPECT_EQ(calls, e.x);
        ++calls;
    });
    bus.publish(Event{0});
    EXPECT_EQ(calls, 1);
    bus.publish(Event{1});
    EXPECT_EQ(calls, 2);
}
