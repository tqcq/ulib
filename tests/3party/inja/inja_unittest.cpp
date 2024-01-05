#include <gtest/gtest.h>
#include <inja.hpp>

TEST(inja, base)
{
    inja::json data;
    data["name"] = "world";

    std::string str = inja::render("Hello {{ name }}", data);
    EXPECT_EQ(str, "Hello world");
}
