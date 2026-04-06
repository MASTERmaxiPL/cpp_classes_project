#include <gtest/gtest.h>

#include "../../src/person/PersonManager.h"

TEST(HelloTestPerson, BasicAssertions) {
    EXPECT_STRNE("hello", "world");
    EXPECT_EQ(7 * 6, 42);
}