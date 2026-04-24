#include <gtest/gtest.h>

#include "../../src/club/ClubManager.h"
#include "../../src/utils/Country.h"
#include "../../src/person/PersonManager.h"
#include "../../src/person/player/PlayerManager.h"
#include "../../src/stadium/stadiumManager.h"
#include "person/staff/StaffManager.h"

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

TEST_F(ClubManagerTest, AssignmentOperation) {
    ClubManager copy;
    copy = sm;

    Club* c1 = copy.findClubByName("Legia Warszawa");
    EXPECT_NE(c1, nullptr);
    EXPECT_EQ(c1->data.country, POLAND);
    EXPECT_EQ(c1->data.founded_year, 1916);
    EXPECT_STREQ(c1->data.city, "Warsaw");
}

TEST_F(ClubManagerTest, CopyConstructor) {
    ClubManager copy(sm);

    Club* c1 = copy.findClubByName("Legia Warszawa");
    EXPECT_NE(c1, nullptr);
    EXPECT_EQ(c1->data.country, POLAND);
    EXPECT_EQ(c1->data.founded_year, 1916);
    EXPECT_STREQ(c1->data.city, "Warsaw");
}

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

TEST_F(ClubManagerTest, EditExistingClub) {
    sm.club("Legia Warszawa", POLAND, "Warszawa", 1912);

    Club* c1 = sm.findClubByName("Legia Warszawa");
    EXPECT_NE(c1, nullptr);
    EXPECT_EQ(c1->data.country, POLAND);
    EXPECT_EQ(c1->data.founded_year, 1912);
    EXPECT_STREQ(c1->data.city, "Warszawa");
}

TEST(ClubManagerGetters, getAllClubsCollectionFromEmptyList) {
    ClubManager sm;

    vector<Club*> list = sm.getAllClubsCollection();
    EXPECT_EQ(list.empty(), true);
}

TEST(ClubManagerGetters, getAllClubsCollectionFromExistingList) {
    ClubManager sm;
    sm.club("Legia Warszawa", POLAND, "Warsaw", 1916);
    sm.club("Lech Poznań", POLAND, "Poznań", 1922);

    const vector<Club*> list = sm.getAllClubsCollection();
    EXPECT_NE(list.empty(), true);
    EXPECT_NE(list.front()->next, nullptr);
    EXPECT_EQ(list.front()->next->next, nullptr);
}

TEST_F(ClubManagerTest, FindClubByName)
{

    Club* c1 = sm.findClubByName("Legia Warszawa");
    EXPECT_NE(c1, nullptr);
}

TEST_F(ClubManagerTest, FindClubsByCountry)
{
    const auto list = sm.getAllClubsCollection();
    const vector<Club*> c1 = sm.findClubsByCountry(POLAND, list);

    EXPECT_NE(c1.front(), nullptr);
    EXPECT_NE(c1.back(), nullptr);
}

TEST_F(ClubManagerTest, FindClubsByCity)
{
    sm.club("Polonia Warszawa", POLAND, "Warsaw", 1927);

    auto list = sm.getAllClubsCollection();

    vector<Club*> c1 = sm.findClubsByCity("Warsaw", list);

    EXPECT_NE(c1.front(), nullptr);
}

TEST_F(ClubManagerTest, FindClubsByFoundedYear)
{
    const auto list = sm.getAllClubsCollection();
    vector<Club*> c1 = sm.findClubsByFoundedYear(1916, list);

    EXPECT_NE(c1.front(), nullptr);
    EXPECT_EQ(c1.front(), c1.back());
}

TEST_F(ClubManagerTest, FindClubsByNewerFoundedYear)
{
    const auto list = sm.getAllClubsCollection();
    vector<Club*> c1 = sm.findClubsByNewerFoundedYear(1910, list);

    EXPECT_NE(c1.front(), nullptr);
}

TEST_F(ClubManagerTest, FindClubsByOlderFoundedYear)
{
    const auto list = sm.getAllClubsCollection();
    vector<Club*> c1 = sm.findClubsByOlderFoundedYear(1910, list);

    EXPECT_NE(c1.front(), nullptr);
    EXPECT_EQ(c1.front(), c1.back());
}

TEST_F(ClubManagerTest, ChainFilters)
{
    const auto list = sm.getAllClubsCollection();

    const auto poland = sm.findClubsByCountry(POLAND, list);
    const auto warsaw = sm.findClubsByCity("Warsaw", poland);

    int count = 0;
    for (Club* club : warsaw) {
        count++;
    }

    EXPECT_EQ(count, 1);
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

TEST_F(ClubManagerTest, DisplayClub)
{
    auto list = sm.getAllClubsCollection();

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    sm.displayClub(list.front());

    std::cout.rdbuf(old);
    std::string output = buffer.str();

    ASSERT_NE(output.find(list.front()->data.name), std::string::npos);
}

TEST_F(ClubManagerTest, DisplayClubList)
{
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
    ASSERT_NE(club, nullptr);

    PersonManager pm;
    PlayerManager plm;

    pm.person("Jan", "Kowalski", 25, POLAND);
    Person* p = pm.findPersonById(0);

    Player* player = plm.addPlayer(p, FORWARD);

    sm.addPlayerToClub(player, club);

    const int count = sm.getClubPlayersCount(club);
    EXPECT_EQ(count, 1);
}

TEST_F(ClubManagerTest, AddStaffToClub)
{
    Club* club = sm.findClubByName("Legia Warszawa");
    ASSERT_NE(club, nullptr);

    PersonManager pm;
    StaffManager stm;

    pm.person("Adam", "Trener", 45, POLAND);

    Person* p = pm.findPersonById(0);
    ASSERT_NE(p, nullptr);

    Staff* staff = stm.addStaff(p, COACH);
    ASSERT_NE(staff, nullptr);

    sm.addStaffToClub(staff, club);

    const int count = sm.getClubStaffCount(club);
    EXPECT_EQ(count, 1);
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

    PersonManager pm;
    PlayerManager plm;

    pm.person("Jan", "Kowalski", 25, POLAND);
    pm.person("Adam", "Nowak", 30, POLAND);

    Player* player1 = plm.addPlayer(pm.findPersonById(0), FORWARD);
    Player* player2 = plm.addPlayer(pm.findPersonById(1), MIDFIELDER);

    sm.addPlayerToClub(player1, club);
    sm.addPlayerToClub(player2, club);

    int count = sm.getClubPlayersCount(club);
    EXPECT_EQ(count, 2);
}

TEST_F(ClubManagerTest, GetClubStaffCountWithExistingList)
{
    Club* club = sm.findClubByName("Legia Warszawa");

    PersonManager pm;
    StaffManager stm;

    pm.person("Adam", "Trener", 45, POLAND);
    pm.person("Ewa", "Asystent", 35, POLAND);

    Staff* staff1 = stm.addStaff(pm.findPersonById(0), COACH);
    Staff* staff2 = stm.addStaff(pm.findPersonById(1), SCOUT);

    sm.addStaffToClub(staff1, club);
    sm.addStaffToClub(staff2, club);

    int count = sm.getClubStaffCount(club);
    EXPECT_EQ(count, 2);
}

TEST_F(ClubManagerTest, RemovePersonAndStadiumFromClub)
{
    PersonManager pm;
    PlayerManager plm;
    StadiumManager stm;

    Club* club = sm.findClubByName("Legia Warszawa");
    ASSERT_NE(club, nullptr);

    pm.person("CClub", "Member", 25, POLAND);
    Person* p = pm.findPersonById(0);
    ASSERT_NE(p, nullptr);

    Player* player = plm.addPlayer(p, FORWARD);
    ASSERT_NE(player, nullptr);

    stm.stadium("ClubStadium", POLAND, "Warsaw", 20000);
    auto allStadiums = stm.getAllStadiumsCollection();
    Stadium* s = stm.findStadiumByName("ClubStadium", allStadiums);
    ASSERT_NE(s, nullptr);

    sm.addPlayerToClub(player, club);
    sm.addStadiumToClub(s, club);

    EXPECT_EQ(sm.getClubPlayersCount(club), 1);
    EXPECT_EQ(sm.getClubStadiumsCount(club), 1);

    const bool pr = sm.removePersonFromClub(p, club);
    const bool sr = sm.removeStadiumFromClub(s, club);

    EXPECT_TRUE(pr);
    EXPECT_TRUE(sr);

    EXPECT_EQ(sm.getClubPlayersCount(club), 0);
    EXPECT_EQ(sm.getClubStadiumsCount(club), 0);

    EXPECT_EQ(p->hiredBy, nullptr);
    EXPECT_EQ(s->ownedBy, nullptr);
}
