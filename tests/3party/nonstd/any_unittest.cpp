
#include <gtest/gtest.h>
#include <any.h>

TEST(any, shared_ptr)
{
    std::shared_ptr<int> ptr = std::make_shared<int>(1);
    EXPECT_EQ(ptr.use_count(), 1);
    {
        nonstd::any int_any = ptr;
        EXPECT_EQ(ptr.use_count(), 2);
    }
    EXPECT_EQ(ptr.use_count(), 1);
    {
        nonstd::any int_any = ptr;
        EXPECT_EQ(ptr.use_count(), 2);
        std::shared_ptr<int> ptr2 =
            nonstd::any_cast<std::shared_ptr<int>>(int_any);
        EXPECT_EQ(ptr2.use_count(), 3);
    }
}
