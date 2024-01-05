#include <gtest/gtest.h>
#include <ulib/utils/defer.h>

TEST(Defer, Defer)
{
    int i = 0;
    {
        defer([&i]() { i++; });
        EXPECT_EQ(i, 0);
    }
    EXPECT_EQ(i, 1);
}

TEST(Defer, DeferFunc)
{
    int i = 0;
    {
        defer([&i]() { i++; });
        EXPECT_EQ(i, 0);
    }
    EXPECT_EQ(i, 1);
}

TEST(Defer, DeferMove)
{
    int i = 0;
    {
        defer([&i]() { i++; });
        EXPECT_EQ(i, 0);
        defer([&i]() { i++; });
        EXPECT_EQ(i, 0);
    }
    EXPECT_EQ(i, 2);
}

TEST(Defer, Add)
{
    int i = 0;
    {
        ulib::Defer defer;
        defer.Add([&i]() { i++; });
        EXPECT_EQ(i, 0);
        defer.Add([&i]() { i++; });
        EXPECT_EQ(i, 0);
    }
    EXPECT_EQ(i, 2);
}

TEST(Defer, Clear)
{
    int i = 0;
    {
        ulib::Defer defer;
        defer.Add([&i]() { i++; });
        EXPECT_EQ(i, 0);
        defer.Clear();
        EXPECT_EQ(i, 0);
    }
    EXPECT_EQ(i, 0);
}
