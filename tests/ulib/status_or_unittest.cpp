#include <gtest/gtest.h>
#include <ulib/status_or.h>

TEST(StatusOr, EmptyValue)
{
    ulib::StatusOr<int> s;
    EXPECT_FALSE(s.ok());
    s = 1;
    EXPECT_TRUE(s.ok());
    EXPECT_EQ(s.value(), 1);
}
