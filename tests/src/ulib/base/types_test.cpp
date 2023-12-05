//
// Created by tqcq on 2023/12/5.
//

#include <ulib/base/types.h>
#include <gtest/gtest.h>

TEST(ULIB_BASE_TYPES, SIZE_OF)
{
    EXPECT_EQ(sizeof(ulib::int8_t), sizeof(char));
    EXPECT_EQ(sizeof(ulib::int16_t), sizeof(short));
    EXPECT_EQ(sizeof(ulib::int32_t), sizeof(int));
    EXPECT_EQ(sizeof(ulib::int64_t), sizeof(long long));
    EXPECT_EQ(sizeof(ulib::uint8_t), sizeof(unsigned char));
    EXPECT_EQ(sizeof(ulib::uint16_t), sizeof(unsigned short));
    EXPECT_EQ(sizeof(ulib::uint32_t), sizeof(unsigned int));
    EXPECT_EQ(sizeof(ulib::uint64_t), sizeof(unsigned long long));

    EXPECT_EQ(sizeof(ulib::int8_t), 1);
    EXPECT_EQ(sizeof(ulib::int16_t), 2);
    EXPECT_EQ(sizeof(ulib::int32_t), 4);
    EXPECT_EQ(sizeof(ulib::int64_t), 8);
    EXPECT_EQ(sizeof(ulib::uint8_t), 1);
    EXPECT_EQ(sizeof(ulib::uint16_t), 2);
    EXPECT_EQ(sizeof(ulib::uint32_t), 4);
    EXPECT_EQ(sizeof(ulib::uint64_t), 8);
}