#include <gtest/gtest.h>
#include <sqlpp11/select.h>
#include <sqlpp11/alias_provider.h>

TEST(sqlpp11, select) { select(sqlpp::value(false).as(sqlpp::alias::a)); }
