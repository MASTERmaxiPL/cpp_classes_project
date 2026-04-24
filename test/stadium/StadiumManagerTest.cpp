#include <gtest/gtest.h>

#include "../../src/stadium/StadiumManager.h"
#include "../../src/utils/Country.h"
#include "club/ClubManager.h"

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

TEST_F(StadiumManagerTest, AssignmentOperation) {
    StadiumManager copy;
    copy = sm;

    const auto list = sm.getAllStadiumsCollection();

    Stadium* s1 = copy.findStadiumByName("Stadion Narodowy", list);
    EXPECT_NE(s1, nullptr);
    EXPECT_EQ(s1->data.country, POLAND);
    EXPECT_EQ(s1->data.numberOfSeats, 100000);
    EXPECT_STREQ(s1->data.city, "Warsaw");
}

TEST_F(StadiumManagerTest, CopyConstructor) {
    StadiumManager copy(sm);

    const auto list = sm.getAllStadiumsCollection();

    Stadium* s1 = copy.findStadiumByName("Stadion Narodowy", list);
    EXPECT_NE(s1, nullptr);
    EXPECT_EQ(s1->data.country, POLAND);
    EXPECT_EQ(s1->data.numberOfSeats, 100000);
    EXPECT_STREQ(s1->data.city, "Warsaw");
}

TEST(StadiumManagerAdditionTest, AddStadiumToEmptyList) {
    StadiumManager sm;
    sm.stadium("Stadion Miejski Legii Warszawa", POLAND, "Warsaw", 80000);

    const auto list = sm.getAllStadiumsCollection();

    Stadium* s1 = sm.findStadiumByName("Stadion Miejski Legii Warszawa", list);
    EXPECT_NE(s1, nullptr);
}

TEST(StadiumManagerAdditionTest, AddStadiumToExistingList) {
    StadiumManager sm;
    sm.stadium("Stadion Miejski Legii Warszawa", POLAND, "Warsaw", 80000);
    sm.stadium("Stadion Miejski Pogoni Szczecin", POLAND, "Szczecin", 40000);

    const auto list = sm.getAllStadiumsCollection();

    Stadium* s1 = sm.findStadiumByName("Stadion Miejski Pogoni Szczecin", list);
    Stadium* s2 = sm.findStadiumByName("Stadion Miejski Legii Warszawa", list);

    EXPECT_NE(s1, nullptr);
    EXPECT_NE(s2, nullptr);
    EXPECT_EQ(s2->next, nullptr);
}

TEST_F(StadiumManagerTest, EditStadiumData) {
    sm.stadium("Stadion Narodowy", POLAND, "Warsaw", 150000);

    const auto list = sm.getAllStadiumsCollection();

    Stadium* s1 = sm.findStadiumByName("Stadion Narodowy", list);
    EXPECT_NE(s1, nullptr);
    EXPECT_EQ(s1->data.numberOfSeats, 150000);
}

TEST(StadiumManagerGetters, GetAllStadiumsWrappedFromEmptyList) {
    StadiumManager sm;

    const auto list = sm.getAllStadiumsCollection();
    EXPECT_EQ(list.empty(), true);
}

TEST(StadiumManagerGetters, GetAllStadiumsWrappedFromExistingList) {
    StadiumManager sm;
    sm.stadium("Stadion Miejski Legii Warszawa", POLAND, "Warsaw", 80000);
    sm.stadium("Stadion Miejski Pogoni Szczecin", POLAND, "Szczecin", 40000);

    const auto list = sm.getAllStadiumsCollection();
    EXPECT_NE(list.empty(), true);
    EXPECT_NE(list.front()->next, nullptr);
    EXPECT_EQ(list.front()->next->next, nullptr);
}

TEST_F(StadiumManagerTest, TryToFindNotExistingStadiumByName)
{
    const auto list = sm.getAllStadiumsCollection();
    Stadium* s1 = sm.findStadiumByName("UNKNOWN", list);

    EXPECT_EQ(s1, nullptr);
}

TEST_F(StadiumManagerTest, FindStadiumByName)
{
    const auto list = sm.getAllStadiumsCollection();

    Stadium* s1 = sm.findStadiumByName("Stadion Narodowy", list);
    EXPECT_NE(s1, nullptr);
}

TEST_F(StadiumManagerTest, FindStadiumsByCountry)
{
    const auto list = sm.getAllStadiumsCollection();
    const auto s1 = sm.findStadiumsByCountry(POLAND, list);

    EXPECT_NE(s1.empty(), true);
    EXPECT_NE(s1.front()->next, nullptr);

}

TEST_F(StadiumManagerTest, FindStadiumsByCity)
{
    const auto list = sm.getAllStadiumsCollection();
    const auto s1 = sm.findStadiumsByCity("Warsaw", list);

    EXPECT_NE(s1.empty(), true);
    EXPECT_NE(s1.front()->next, nullptr);

}

TEST_F(StadiumManagerTest, FindStadiumsByMinSeats)
{
    auto list = sm.getAllStadiumsCollection();
    const auto s1 = sm.findStadiumsByMinSeats(150000, list);

    EXPECT_NE(s1.empty(), true);

    int count = 0;
    for (Stadium* stadium : s1)
    {
        count++;
    }
    EXPECT_EQ(count, 1);
}

TEST_F(StadiumManagerTest, FindStadiumsByMaxSeats)
{
    const auto list = sm.getAllStadiumsCollection();
    const auto s1 = sm.findStadiumsByMaxSeats(80000, list);

    EXPECT_NE(s1.empty(), true);
    EXPECT_NE(s1.front()->next, nullptr);

}

TEST_F(StadiumManagerTest, ChainFilters)
{
    const auto list = sm.getAllStadiumsCollection();

    const auto poland = sm.findStadiumsByCountry(POLAND, list);
    const auto warsaw = sm.findStadiumsByCity("Warsaw", poland);

    int count = 0;
    for (Stadium* stadium : warsaw)
    {
        count++;
    }

    EXPECT_EQ(count, 2);
}

TEST_F(StadiumManagerTest, DeleteStadium)
{
    const auto list = sm.getAllStadiumsCollection();

    Stadium* s1 = sm.findStadiumByName("Stadion Narodowy", list);

    const bool success = sm.deleteStadium(s1);
    ASSERT_TRUE(success);

    s1 = sm.findStadiumByName("Stadion Narodowy", list);
    EXPECT_EQ(s1, nullptr);
}

TEST_F(StadiumManagerTest, DeleteAllStadiums)
{
    sm.deleteAllStadiums();

    const auto list = sm.getAllStadiumsCollection();

    Stadium* s1 = sm.findStadiumByName("Stadion Miejski Legii Warszawa", list);
    Stadium* s2 = sm.findStadiumByName("Allianz Arena", list);

    EXPECT_EQ(s1, nullptr);
    EXPECT_EQ(s2, nullptr);
}

TEST_F(StadiumManagerTest, DisplayStadium)
{
    const auto list = sm.getAllStadiumsCollection();

    Stadium* s1 = sm.findStadiumByName("Stadion Narodowy", list);

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

TEST(StadiumManagerClubIntegrationTest, DeleteStadiumRemovesFromClub)
{
    ClubManager cm;
    cm.club("ClubS", POLAND, "CityS", 1950);
    Club* club = cm.findClubByName("ClubS");
    ASSERT_NE(club, nullptr);

    StadiumManager sm;
    sm.stadium("TEST1", POLAND, "CityS", 10000);

    const auto list = sm.getAllStadiumsCollection();
    Stadium* s = sm.findStadiumByName("TEST1", list);
    ASSERT_NE(s, nullptr);

    EXPECT_EQ(cm.getClubStadiumsCount(club), 0);

    cm.addStadiumToClub(s, club);
    EXPECT_EQ(cm.getClubStadiumsCount(club), 1);
    EXPECT_EQ(s->ownedBy, club);

    const bool success = sm.deleteStadium(s);
    EXPECT_TRUE(success);

    EXPECT_EQ(cm.getClubStadiumsCount(club), 0);
}