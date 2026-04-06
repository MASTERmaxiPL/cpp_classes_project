#include <gtest/gtest.h>

#include "../../src/utils/IdGenerator.h"

TEST(HelloTestIdGenerator, BasicAssertions) {
    EXPECT_STRNE("hello", "world");
    EXPECT_EQ(7 * 6, 42);
}