//
// Created by tqcq on 2023/11/16.
//

#include "location.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

namespace tqcq {
#define WhereAmI() (FROM_HERE)

TEST(LocationTest, CurrentYieldsCorrectValue) {
  int previous_line = __LINE__;
  Location here = WhereAmI();
  EXPECT_NE(here.program_counter(), WhereAmI().program_counter());
  EXPECT_THAT(here.file_name(), ::testing::EndsWith("location_unittest.cpp"));
  EXPECT_EQ(here.line_number(), previous_line + 1);
  EXPECT_STREQ("TestBody", here.function_name());
}

}