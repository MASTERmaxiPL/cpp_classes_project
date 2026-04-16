#include <gtest/gtest.h>

#include "../../src/stadium/StadiumManager.h"
#include "../../src/utils/Country.h"

class StadiumManagerTest : public testing::Test
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

    Stadium* s1 = sm.findStadiumByName("Stadion Miejski Legii Warszawa");
    EXPECT_NE(s1, nullptr);
}

TEST(StadiumManagerAdditionTest, AddStadiumToExistingList) {
    StadiumManager sm;
    sm.stadium("Stadion Miejski Legii Warszawa", POLAND, "Warsaw", 80000);
    sm.stadium("Stadion Miejski Pogoni Szczecin", POLAND, "Szczecin", 40000);

    Stadium* s1 = sm.findStadiumByName("Stadion Miejski Pogoni Szczecin");
    Stadium* s2 = sm.findStadiumByName("Stadion Miejski Legii Warszawa");

    EXPECT_NE(s1, nullptr);
    EXPECT_NE(s2, nullptr);
    EXPECT_EQ(s2->next, nullptr);
}

TEST_F(StadiumManagerTest, EditStadiumData) {
    sm.stadium("Stadion Narodowy", POLAND, "Warsaw", 150000);

    Stadium* s1 = sm.findStadiumByName("Stadion Narodowy");
    EXPECT_NE(s1, nullptr);
    EXPECT_EQ(s1->data.numberOfSeats, 150000);
}

TEST(StadiumManagerGetters, GetAllStadiumsWrappedFromEmptyList) {
    StadiumManager sm;

    StadiumListNode* list = sm.getAllStadiumsWrapped();
    EXPECT_EQ(list, nullptr);

    sm.deleteAllWrappedStadiums(list);
}

TEST(StadiumManagerGetters, GetAllStadiumsWrappedFromExistingList) {
    StadiumManager sm;
    sm.stadium("Stadion Miejski Legii Warszawa", POLAND, "Warsaw", 80000);
    sm.stadium("Stadion Miejski Pogoni Szczecin", POLAND, "Szczecin", 40000);

    StadiumListNode* list = sm.getAllStadiumsWrapped();
    EXPECT_NE(list, nullptr);
    EXPECT_NE(list->next, nullptr);
    EXPECT_EQ(list->next->next, nullptr);

    sm.deleteAllWrappedStadiums(list);
}

TEST_F(StadiumManagerTest, TryToFindNotExistingStadiumByName)
{
    auto list = sm.getAllStadiumsWrapped();
    Stadium* s1 = sm.findStadiumByNameInWrapper("UNKNOWN", list);

    EXPECT_EQ(s1, nullptr);

    sm.deleteAllWrappedStadiums(list);
}

TEST_F(StadiumManagerTest, FindStadiumByName)
{
    Stadium* s1 = sm.findStadiumByName("Stadion Narodowy");
    EXPECT_NE(s1, nullptr);
}

TEST_F(StadiumManagerTest, FindWrappedStadiumByName)
{
    auto list = sm.getAllStadiumsWrapped();
    Stadium* s1 = sm.findStadiumByNameInWrapper("Stadion Narodowy", list);
    EXPECT_NE(s1, nullptr);

    sm.deleteAllWrappedStadiums(list);
}

TEST_F(StadiumManagerTest, FindStadiumsByCountry)
{
    auto list = sm.getAllStadiumsWrapped();
    StadiumListNode* s1 = sm.findStadiumsByCountry(POLAND, list);

    EXPECT_NE(s1, nullptr);
    EXPECT_NE(s1->next, nullptr);

    sm.deleteAllWrappedStadiums(list);
}

TEST_F(StadiumManagerTest, FindStadiumsByCity)
{
    auto list = sm.getAllStadiumsWrapped();
    StadiumListNode* s1 = sm.findStadiumsByCity("Warsaw", list);

    EXPECT_NE(s1, nullptr);
    EXPECT_NE(s1->next, nullptr);

    sm.deleteAllWrappedStadiums(list);
}

TEST_F(StadiumManagerTest, FindStadiumsByMinSeats)
{
    auto list = sm.getAllStadiumsWrapped();
    StadiumListNode* s1 = sm.findStadiumsByMinSeats(150000, list);

    EXPECT_NE(s1, nullptr);

    int count = 0;
    for (StadiumListNode* it = s1; it != nullptr; it = it->next)
        count++;
    EXPECT_EQ(count, 1);

    EXPECT_EQ(s1->next, nullptr);

    sm.deleteAllWrappedStadiums(list);
}

TEST_F(StadiumManagerTest, FindStadiumsByMaxSeats)
{
    auto list = sm.getAllStadiumsWrapped();
    StadiumListNode* s1 = sm.findStadiumsByMaxSeats(80000, list);

    EXPECT_NE(s1, nullptr);
    EXPECT_NE(s1->next, nullptr);

    sm.deleteAllWrappedStadiums(list);
}

TEST_F(StadiumManagerTest, ChainFilters)
{
    auto list = sm.getAllStadiumsWrapped();

    auto poland = sm.findStadiumsByCountry(POLAND, list);
    auto warsaw = sm.findStadiumsByCity("Warsaw", poland);

    int count = 0;
    for (auto* it = warsaw; it != nullptr; it = it->next)
        count++;

    EXPECT_EQ(count, 2);

    sm.deleteAllWrappedStadiums(list);
    sm.deleteAllWrappedStadiums(poland);
    sm.deleteAllWrappedStadiums(warsaw);
}

TEST(StadiumManagerTestEdgeCase, DeleteStadiumFromEmptyList)
{
    StadiumManager sm;
    Stadium s;
    const bool success = sm.deleteStadium(&s);
    ASSERT_FALSE(success);
}

TEST_F(StadiumManagerTest, DeleteStadium)
{
    Stadium* s1 = sm.findStadiumByName("Stadion Narodowy");

    const bool success = sm.deleteStadium(s1);
    ASSERT_TRUE(success);

    s1 = sm.findStadiumByName("Stadion Narodowy");
    EXPECT_EQ(s1, nullptr);
}

TEST_F(StadiumManagerTest, DeleteAllStadiums)
{
    sm.deleteAllStadiums();

    Stadium* s1 = sm.findStadiumByName("Stadion Miejski Legii Warszawa");
    Stadium* s2 = sm.findStadiumByName("Allianz Arena");

    EXPECT_EQ(s1, nullptr);
    EXPECT_EQ(s2, nullptr);
}

TEST_F(StadiumManagerTest, DeleteWrapperStadium)
{
    auto list = sm.getAllStadiumsWrapped();

    Stadium* s = sm.findStadiumByNameInWrapper("Stadion Narodowy", list);
    ASSERT_NE(s, nullptr);

    bool removed = sm.deleteWrappedStadium(list, s);
    EXPECT_TRUE(removed);

    Stadium* again = sm.findStadiumByNameInWrapper("Stadion Narodowy", list);
    EXPECT_EQ(again, nullptr);

    sm.deleteAllWrappedStadiums(list);
}

TEST_F(StadiumManagerTest, DeleteAllWrapperList)
{
    auto list = sm.getAllStadiumsWrapped();

    sm.deleteAllWrappedStadiums(list);

    StadiumListNode* s1 = sm.findStadiumsByCountry(POLAND, list);
    StadiumListNode* s2 = sm.findStadiumsByCity("Warsaw", list);

    EXPECT_EQ(s1, nullptr);
    EXPECT_EQ(s2, nullptr);
}

TEST_F(StadiumManagerTest, DisplayStadium)
{
    Stadium* s1 = sm.findStadiumByName("Stadion Narodowy");

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    sm.displayStadium(s1);

    std::cout.rdbuf(old);
    std::string output = buffer.str();

    ASSERT_NE(output.find("Stadion Narodowy"), std::string::npos);
}

TEST_F(StadiumManagerTest, DisplayStadiumList)
{
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    sm.displayStadiumList();

    std::cout.rdbuf(old);
    std::string output = buffer.str();

    ASSERT_NE(output.find("Stade de l'Independance"), std::string::npos);
    ASSERT_NE(output.find("Allianz Arena"), std::string::npos);
    ASSERT_NE(output.find("Stadion Narodowy"), std::string::npos);
    ASSERT_NE(output.find("Stadion Miejski Legii Warszawa"), std::string::npos);
}

TEST_F(StadiumManagerTest, DisplayWrappedStadium)
{
    auto list = sm.getAllStadiumsWrapped();

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    sm.displayWrappedStadium(list);

    std::cout.rdbuf(old);
    std::string output = buffer.str();

    ASSERT_NE(output.find(list->stadium->data.name), std::string::npos);

    sm.deleteAllWrappedStadiums(list);
}

TEST_F(StadiumManagerTest, DisplayWrappedStadiumList)
{
    auto list = sm.getAllStadiumsWrapped();

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    sm.displayWrappedStadiumList(list);

    std::cout.rdbuf(old);
    std::string output = buffer.str();

    ASSERT_NE(output.find("Stade de l'Independance"), std::string::npos);
    ASSERT_NE(output.find("Allianz Arena"), std::string::npos);
    ASSERT_NE(output.find("Stadion Narodowy"), std::string::npos);
    ASSERT_NE(output.find("Stadion Miejski Legii Warszawa"), std::string::npos);

    sm.deleteAllWrappedStadiums(list);
}