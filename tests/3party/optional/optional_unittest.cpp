#include <gtest/gtest.h>
#include <optional.h>

TEST(optional, has_value)
{
    ulib::optional<int> int_opt = 1;
    EXPECT_TRUE(int_opt.has_value());
    EXPECT_EQ(*int_opt, 1);
    EXPECT_EQ(int_opt.value(), 1);
    EXPECT_EQ(int_opt.value_or(2), 1);
}

TEST(optional, not_has_value)
{
    ulib::optional<int> int_opt;
    EXPECT_FALSE(int_opt.has_value());
    EXPECT_EQ(int_opt.value_or(1), 1);
    EXPECT_EQ(int_opt.value_or(2), 2);
}
