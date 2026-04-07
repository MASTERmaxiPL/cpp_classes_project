#include <codecvt>
#include <gtest/gtest.h>

#include "../../src/stadium/StadiumManager.h"
#include "../../src/utils/Country.h"

class StadiumManagerTest : public ::testing::Test
{
    protected:
    void SetUp() override
    {
        sm.stadium("Stade de l'Independance", FRANCE, "Paris", 80000);
        sm.stadium("Allianz Arena", GERMANY, "Munich", 200000);
        sm.stadium("Stadion Narodowy", POLAND, "Warsaw", 100000);
        sm.stadium("Stadion Miejski Legii Warszawa", POLAND, "Warsaw", 80000);
    }

    StadiumManager sm;
};

TEST(StadiumManagerAdditionTest, AddStadiumToEmptyList) {
    StadiumManager sm;
    sm.stadium("Stadion Miejski Legii Warszawa", POLAND, "Warsaw", 80000);
    Stadium* s1 = sm.findStadiumByName("Stadion Miejski Legii Warszawa", nullptr);
    EXPECT_NE(s1, nullptr);
}

TEST(StadiumManagerAdditionTest, AddStadiumToExistingList) {
    StadiumManager sm;
    sm.stadium("Stadion Miejski Legii Warszawa", POLAND, "Warsaw", 80000);
    sm.stadium("Stadion Miejski Pogoni Szczecin", POLAND, "Szczecin", 40000);

    Stadium* s1 = sm.findStadiumByName("Stadion Miejski Pogoni Szczecin", nullptr);
    Stadium* s2 = sm.findStadiumByName("Stadion Miejski Legii Warszawa", nullptr);

    EXPECT_NE(s1, nullptr);
    EXPECT_NE(s2, nullptr);
    EXPECT_EQ(s2->next, nullptr);
}

TEST_F(StadiumManagerTest, FindStadiumByName)
{
    Stadium* s1 = sm.findStadiumByName("Stadion Narodowy", nullptr);
    EXPECT_NE(s1, nullptr);
}

TEST_F(StadiumManagerTest, FindStadiumsByCountry)
{
    StadiumListNode* s1 = sm.findStadiumsByCountry(POLAND, nullptr);
    EXPECT_NE(s1, nullptr);
    EXPECT_NE(s1->next, nullptr);
}

TEST_F(StadiumManagerTest, FindStadiumsByCity)
{
    StadiumListNode* s1 = sm.findStadiumsByCity("Warsaw", nullptr);
    EXPECT_NE(s1, nullptr);
    EXPECT_NE(s1->next, nullptr);
}

TEST_F(StadiumManagerTest, FindStadiumsByMinSeats)
{
    StadiumListNode* s1 = sm.findStadiumsByMinSeats(150000, nullptr);
    EXPECT_NE(s1, nullptr);
    int count = 0;
    for (StadiumListNode* it = s1; it != nullptr; it = it->next)
        count++;
    EXPECT_EQ(count, 1);

    EXPECT_EQ(s1->next, nullptr);
}

TEST_F(StadiumManagerTest, FindStadiumsByMaxSeats)
{
    StadiumListNode* s1 = sm.findStadiumsByMaxSeats(80000, nullptr);
    EXPECT_NE(s1, nullptr);
    EXPECT_NE(s1->next, nullptr);
}

TEST(StadiumManagerTestEdgeCase, DeleteStadiumFromEmptyList)
{
    StadiumManager sm;
    auto* s = new Stadium();
    const bool success = sm.deleteStadium(s);
    ASSERT_FALSE(success);
}

TEST_F(StadiumManagerTest, DeleteStadium)
{
    Stadium* s1 = sm.findStadiumByName("Stadion Narodowy", nullptr);
    const bool success = sm.deleteStadium(s1);
    ASSERT_TRUE(success);
    s1 = sm.findStadiumByName("Stadion Narodowy", nullptr);
    EXPECT_EQ(s1, nullptr);
}

TEST_F(StadiumManagerTest, DeleteAllStadiums)
{
    sm.deleteAllStadiums(nullptr);
    Stadium* s1 = sm.findStadiumByName("Stadion Miejski Legii Warszawa", nullptr);
    Stadium* s2 = sm.findStadiumByName("Allianz Arena", nullptr);
    EXPECT_EQ(s1, nullptr);
    EXPECT_EQ(s2, nullptr);
}

TEST(StadiumManagerWrapperTest, DeleteAllWrapperList)
{
    StadiumManager sm;

    Stadium* s1 = new Stadium();
    auto* el1 = new StadiumListNode();
    el1->stadium = s1;
    el1->stadium->data.name = "TEST_1";
    Stadium* s2 = new Stadium();
    auto* el2 = new StadiumListNode();
    el2->stadium = s2;
    el2->stadium->data.name = "TEST_2";
    el2->next = el1;

    sm.deleteAllWrapperList(el2);

    EXPECT_EQ(el2, nullptr);
}

TEST_F(StadiumManagerTest, DisplayStadium)
{
    Stadium* s1 = sm.findStadiumByName("Stadion Narodowy", nullptr);

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    sm.displayStadium(s1);

    std::cout.rdbuf(old);
    std::string output = buffer.str();

    ASSERT_NE(output.find("Stadion Narodowy"), std::string::npos);
}