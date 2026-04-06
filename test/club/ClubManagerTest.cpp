#include <gtest/gtest.h>

#include "../../src/club/ClubManager.h"

TEST(HelloTestClub, BasicAssertions) {
    EXPECT_STRNE("hello", "world");
    EXPECT_EQ(7 * 6, 42);
}