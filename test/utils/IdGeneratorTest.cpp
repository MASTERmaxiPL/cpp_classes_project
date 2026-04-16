#include <gtest/gtest.h>

#include "../../src/utils/IdGenerator.h"

TEST(IdGeneratorTest, GenerateUniqueIds)
{
    IdGenerator idGen;

    const int32_t id1 = idGen.generateId();
    const int32_t id2 = idGen.generateId();
    const int32_t id3 = idGen.generateId();

    EXPECT_NE(id1, id2);
    EXPECT_NE(id1, id3);
    EXPECT_NE(id2, id3);
}