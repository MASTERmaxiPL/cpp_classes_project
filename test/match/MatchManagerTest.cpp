#include <gtest/gtest.h>

#include "../../src/match/MatchManager.h"

TEST(HelloTestMatch, BasicAssertions) {
    EXPECT_STRNE("hello", "world");
    EXPECT_EQ(7 * 6, 42);
}