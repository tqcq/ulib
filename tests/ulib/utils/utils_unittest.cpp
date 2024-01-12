#include <gtest/gtest.h>
#include <ulib/utils/utils.h>

TEST(Utils, Clamp)
{
    EXPECT_EQ(ulib::Clamp(1, 2, 3), 2);
    EXPECT_EQ(ulib::Clamp(2, 2, 3), 2);
    EXPECT_EQ(ulib::Clamp(3, 2, 3), 3);
    EXPECT_EQ(ulib::Clamp(4, 2, 3), 3);
}

TEST(Utils, StrJoin)
{
    std::vector<nonstd::string_view> vec{"a", "b", "c"};
    EXPECT_EQ(ulib::StrJoin(vec, ","), "a,b,c");
    EXPECT_EQ(ulib::StrJoin(vec, ",", false), "a,b,c");
    EXPECT_EQ(ulib::StrJoin(vec, ",", true), "a,b,c");
    vec = {"a", "", "c"};
    EXPECT_EQ(ulib::StrJoin(vec, ","), "a,c");
    EXPECT_EQ(ulib::StrJoin(vec, ",", false), "a,,c");
    EXPECT_EQ(ulib::StrJoin(vec, ",", true), "a,c");
    vec = {"", "", ""};
    EXPECT_EQ(ulib::StrJoin(vec, ","), "");
    EXPECT_EQ(ulib::StrJoin(vec, ",", false), ",,");
    EXPECT_EQ(ulib::StrJoin(vec, ",", true), "");
    vec = {"a", "b", ""};
    EXPECT_EQ(ulib::StrJoin(vec, ","), "a,b");
    EXPECT_EQ(ulib::StrJoin(vec, ",", false), "a,b,");
    EXPECT_EQ(ulib::StrJoin(vec, ",", true), "a,b");
    vec = {"", "b", "c"};
    EXPECT_EQ(ulib::StrJoin(vec, ","), "b,c");
    EXPECT_EQ(ulib::StrJoin(vec, ",", false), ",b,c");
    EXPECT_EQ(ulib::StrJoin(vec, ",", true), "b,c");
}
