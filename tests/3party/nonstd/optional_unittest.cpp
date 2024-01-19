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

TEST(optional, shared_ptr)
{
    std::shared_ptr<int> ptr = std::make_shared<int>(1);
    EXPECT_EQ(ptr.use_count(), 1);
    ulib::optional<std::shared_ptr<int>> int_opt = ptr;
    EXPECT_EQ(ptr.use_count(), 2);
}
