#include <gtest/gtest.h>

#include "../src/stadium/StadiumManager.h"

TEST(HelloTest, BasicAssertions) {
    EXPECT_STRNE("hello", "world");
    EXPECT_EQ(7 * 6, 42);
}