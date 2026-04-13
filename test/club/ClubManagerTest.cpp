#include <codecvt>
#include <gtest/gtest.h>

#include "../../src/club/ClubManager.h"
#include "../../src/utils/Country.h"

class ClubManagerTest : public testing::Test
{
protected:
    void SetUp() override
    {
        sm.club("PSG", FRANCE, "Paris", 1970);
        sm.club("FC Bayern Munich", GERMANY, "Munich", 1900);
        sm.club("Legia Warszawa", POLAND, "Warsaw", 1916);
        sm.club("Lech Poznań", POLAND, "Poznań", 1922);
    }

    ClubManager sm;
};

TEST(ClubManagerAdditionTest, AddClubToEmptyList) {
    ClubManager sm;
    sm.club("Legia Warszawa", POLAND, "Warsaw", 1916);


    Club* c1 = sm.findClubByName("Legia Warszawa");
    EXPECT_NE(c1, nullptr);
}

TEST(ClubManagerAdditionTest, AddClubToExistingList) {
    ClubManager sm;
    sm.club("Legia Warszawa", POLAND, "Warsaw", 1916);
    sm.club("Lech Poznań", POLAND, "Poznań", 1922);

    Club* c2 = sm.findClubByName("Legia Warszawa");
    Club* c1 = sm.findClubByName("Lech Poznań");

    EXPECT_NE(c1, nullptr);
    EXPECT_NE(c2, nullptr);
    EXPECT_EQ(c2->next, nullptr);
}

TEST(ClubManagerGetters, GetAllClubsWrappedFromEmptyList) {
    ClubManager sm;

    ClubListNode* list = sm.getAllClubsWrapped();
    EXPECT_EQ(list, nullptr);

    sm.deleteAllWrappedList(list);
}

TEST(ClubManagerGetters, GetAllClubsWrappedFromExistingList) {
    ClubManager sm;
    sm.club("Legia Warszawa", POLAND, "Warsaw", 1916);
    sm.club("Lech Poznań", POLAND, "Poznań", 1922);

    ClubListNode* list = sm.getAllClubsWrapped();
    EXPECT_NE(list, nullptr);
    EXPECT_NE(list->next, nullptr);
    EXPECT_EQ(list->next->next, nullptr);

    sm.deleteAllWrappedList(list);
}

TEST_F(ClubManagerTest, TryToFindNotExistingClubByName)
{
    auto list = sm.getAllClubsWrapped();
    Club* c1 = sm.findClubByNameInWrapper("UNKNOWN", list);

    EXPECT_EQ(c1, nullptr);

    sm.deleteAllWrappedList(list);
}

TEST_F(ClubManagerTest, FindClubByName)
{

    Club* c1 = sm.findClubByName("Legia Warszawa");
    EXPECT_NE(c1, nullptr);
}

TEST_F(ClubManagerTest, FindWrappedClubByName)
{
    auto list = sm.getAllClubsWrapped();
    Club* c1 = sm.findClubByNameInWrapper("Legia Warszawa", list);
    EXPECT_NE(c1, nullptr);

    sm.deleteAllWrappedList(list);
}

TEST_F(ClubManagerTest, FindClubsByCountry)
{
    auto list = sm.getAllClubsWrapped();
    ClubListNode* c1 = sm.findClubsByCountry(POLAND, list);

    EXPECT_NE(c1, nullptr);
    EXPECT_NE(c1->next, nullptr);

    sm.deleteAllWrappedList(list);
}

TEST_F(ClubManagerTest, FindClubsByCity)
{
    sm.club("Polonia Warszawa", POLAND, "Warsaw", 1927);

    auto list = sm.getAllClubsWrapped();

    ClubListNode* c1 = sm.findClubsByCity("Warsaw", list);

    EXPECT_NE(c1, nullptr);
    EXPECT_NE(c1->next, nullptr);

    sm.deleteAllWrappedList(list);
}

TEST_F(ClubManagerTest, FindClubsByFoundedYear)
{
    auto list = sm.getAllClubsWrapped();
    ClubListNode* c1 = sm.findClubsByFoundedYear(1916, list);

    EXPECT_NE(c1, nullptr);
    EXPECT_EQ(c1->next, nullptr);

    sm.deleteAllWrappedList(list);
}

TEST_F(ClubManagerTest, FindClubsByNewerFoundedYear)
{
    auto list = sm.getAllClubsWrapped();
    ClubListNode* c1 = sm.findClubsByNewerFoundedYear(1910, list);

    EXPECT_NE(c1, nullptr);
    EXPECT_NE(c1->next, nullptr);

    sm.deleteAllWrappedList(list);
}

TEST_F(ClubManagerTest, FindClubsByOlderFoundedYear)
{
    auto list = sm.getAllClubsWrapped();
    ClubListNode* c1 = sm.findClubsByOlderFoundedYear(1910, list);

    EXPECT_NE(c1, nullptr);
    EXPECT_EQ(c1->next, nullptr);

    sm.deleteAllWrappedList(list);
}

TEST_F(ClubManagerTest, ChainFilters)
{
    auto list = sm.getAllClubsWrapped();

    auto poland = sm.findClubsByCountry(POLAND, list);
    auto warsaw = sm.findClubsByCity("Warsaw", poland);

    int count = 0;
    for (auto* it = warsaw; it != nullptr; it = it->next)
        count++;

    EXPECT_EQ(count, 1);

    sm.deleteAllWrappedList(list);
    sm.deleteAllWrappedList(poland);
    sm.deleteAllWrappedList(warsaw);
}

TEST(ClubManagerDeletionTest, DeleteClubFromEmptyList)
{
    ClubManager sm;
    Club c;
    const bool success = sm.deleteClub(&c);
    ASSERT_FALSE(success);
}


TEST_F(ClubManagerTest, DeleteNotExistingClub)
{
    Club c;
    const bool success = sm.deleteClub(&c);
    ASSERT_FALSE(success);
}

TEST_F(ClubManagerTest, DeleteClub)
{

    Club* c1 = sm.findClubByName("Legia Warszawa");

    const bool success = sm.deleteClub(c1);
    ASSERT_TRUE(success);

    c1 = sm.findClubByName("Legia Warszawa");
    EXPECT_EQ(c1, nullptr);
}

TEST_F(ClubManagerTest, DeleteAllClubs)
{
    sm.deleteAllClubs();

    Club* c1 = sm.findClubByName("Legia Warszawa");
    Club* c2 = sm.findClubByName("Lech Poznań");

    EXPECT_EQ(c1, nullptr);
    EXPECT_EQ(c2, nullptr);
}

TEST_F(ClubManagerTest, DeleteWrappedClub)
{
    auto list = sm.getAllClubsWrapped();

    Club* c = sm.findClubByNameInWrapper("Legia Warszawa", list);
    ASSERT_NE(c, nullptr);

    bool removed = sm.deleteWrappedClub(list, c);
    EXPECT_TRUE(removed);

    Club* again = sm.findClubByNameInWrapper("Legia Warszawa", list);
    EXPECT_EQ(again, nullptr);

    sm.deleteAllWrappedList(list);
}

TEST_F(ClubManagerTest, DeleteAllWrapperList)
{
    auto list = sm.getAllClubsWrapped();

    sm.deleteAllWrappedList(list);

    ClubListNode* c1 = sm.findClubsByCountry(POLAND, list);
    ClubListNode* c2 = sm.findClubsByCity("Warsaw", list);

    EXPECT_EQ(c1, nullptr);
    EXPECT_EQ(c2, nullptr);
}

TEST_F(ClubManagerTest, DisplayClub)
{
    auto list = sm.getAllClubsWrapped();

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    sm.displayWrappedClub(list);

    std::cout.rdbuf(old);
    std::string output = buffer.str();

    ASSERT_NE(output.find(list->club->data.name), std::string::npos);

    sm.deleteAllWrappedList(list);
}

TEST_F(ClubManagerTest, DisplayClubList)
{
    ClubData data1 = {"Test Club", POLAND, "Test City", 2000};
    Club* club1 = new Club;
    club1->data = data1;

    ClubData data2 = {"Test Club 2", POLAND, "Test City 2", 2001};
    Club* club2 = new Club;
    club2->data = data2;

    club1->next = club2;

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    sm.displayClubList();

    std::cout.rdbuf(old);
    std::string output = buffer.str();

    ASSERT_NE(output.find("PSG"), std::string::npos);
    ASSERT_NE(output.find("FC Bayern Munich"), std::string::npos);
    ASSERT_NE(output.find("Legia Warszawa"), std::string::npos);
    ASSERT_NE(output.find("Lech Poznań"), std::string::npos);

    sm.deleteAllClubs();
}

TEST_F(ClubManagerTest, DisplayWrappedClub)
{
    auto list = sm.getAllClubsWrapped();

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    sm.displayWrappedClub(list);

    std::cout.rdbuf(old);
    std::string output = buffer.str();

    ASSERT_NE(output.find(list->club->data.name), std::string::npos);

    sm.deleteAllWrappedList(list);
}

TEST_F(ClubManagerTest, DisplayWrappedClubListWithEmptyList)
{
    auto list = sm.getAllClubsWrapped();

    sm.deleteAllWrappedList(list);

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    sm.displayWrappedClubList(list);

    std::cout.rdbuf(old);
    std::string output = buffer.str();

    ASSERT_EQ(output, "");
}

TEST_F(ClubManagerTest, DisplayWrappedClubList)
{
    auto list = sm.getAllClubsWrapped();

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    sm.displayWrappedClubList(list);

    std::cout.rdbuf(old);
    std::string output = buffer.str();

    ASSERT_NE(output.find("PSG"), std::string::npos);
    ASSERT_NE(output.find("FC Bayern Munich"), std::string::npos);
    ASSERT_NE(output.find("Legia Warszawa"), std::string::npos);
    ASSERT_NE(output.find("Lech Poznań"), std::string::npos);

    sm.deleteAllWrappedList(list);
}

TEST_F(ClubManagerTest, AddStadiumToClub)
{
    Club* club = sm.findClubByName("Legia Warszawa");

    StadiumData data = {"Stadion Wojska Polskiego", POLAND, "Warsaw", 31000};
    Stadium* stadium = new Stadium{data, nullptr};

    ClubManager::addStadiumToClub(stadium, club);

    int count = ClubManager::getClubStadiumsCount(club);
    EXPECT_EQ(count, 1);

    delete stadium;
}

TEST_F(ClubManagerTest, AddPlayerToClub)
{
    Club* club = sm.findClubByName("Legia Warszawa");

    Player* player = new Player;

    ClubManager::addPlayerToClub(player, club);

    int count = ClubManager::getClubPlayersCount(club);
    EXPECT_EQ(count, 1);

    delete player;
}

TEST_F(ClubManagerTest, AddStaffToClub)
{
    Club* club = sm.findClubByName("Legia Warszawa");
    Staff* staff = new Staff;

    ClubManager::addStaffToClub(staff, club);

    int count = ClubManager::getClubStaffCount(club);
    EXPECT_EQ(count, 1);

    delete staff;
}

TEST_F(ClubManagerTest, GetClubStadiumsCountWithEmptyList)
{
    Club* club = sm.findClubByName("Legia Warszawa");

    int count = ClubManager::getClubStadiumsCount(club);
    EXPECT_EQ(count, 0);
}

TEST_F(ClubManagerTest, GetClubPlayersCountWithEmptyList)
{
    Club* club = sm.findClubByName("Legia Warszawa");

    int count = ClubManager::getClubPlayersCount(club);
    EXPECT_EQ(count, 0);
}

TEST_F(ClubManagerTest, GetClubStaffCountWithEmptyList)
{
    Club* club = sm.findClubByName("Legia Warszawa");

    int count = ClubManager::getClubStaffCount(club);
    EXPECT_EQ(count, 0);
}

TEST_F(ClubManagerTest, GetClubStadiumsCountWithExistingList)
{
    Club* club = sm.findClubByName("Legia Warszawa");

    StadiumData data1 = {"Stadion Wojska Polskiego", POLAND, "Warsaw", 31000};
    Stadium* stadium1 = new Stadium{data1, nullptr};

    StadiumData data2 = {"Stadion Legii", POLAND, "Warsaw", 20000};
    Stadium* stadium2 = new Stadium{data2, nullptr};

    ClubManager::addStadiumToClub(stadium1, club);
    ClubManager::addStadiumToClub(stadium2, club);

    int count = ClubManager::getClubStadiumsCount(club);
    EXPECT_EQ(count, 2);

    delete stadium1;
    delete stadium2;
}

TEST_F(ClubManagerTest, GetClubPlayersCountWithExistingList)
{
    Club* club = sm.findClubByName("Legia Warszawa");

    Player* player1 = new Player;
    Player* player2 = new Player;

    ClubManager::addPlayerToClub(player1, club);
    ClubManager::addPlayerToClub(player2, club);

    int count = ClubManager::getClubPlayersCount(club);
    EXPECT_EQ(count, 2);

    delete player1;
    delete player2;
}

TEST_F(ClubManagerTest, GetClubStaffCountWithExistingList)
{
    Club* club = sm.findClubByName("Legia Warszawa");

    Staff* staff1 = new Staff;
    Staff* staff2 = new Staff;

    ClubManager::addStaffToClub(staff1, club);
    ClubManager::addStaffToClub(staff2, club);

    int count = ClubManager::getClubStaffCount(club);
    EXPECT_EQ(count, 2);

    delete staff1;
    delete staff2;
}