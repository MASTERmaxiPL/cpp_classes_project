#include <gtest/gtest.h>

#include "../../src/club/ClubManager.h"
#include "../../src/utils/Country.h"
#include "../../src/person/PersonManager.h"
#include "../../src/stadium/stadiumManager.h"
#include "../../src/utils/Filter.h"

class ClubManagerTest : public testing::Test
{
protected:
    void SetUp() override
    {
        cm.club("PSG", FRANCE, "Paris", 1970);
        cm.club("FC Bayern Munich", GERMANY, "Munich", 1900);
        cm.club("Legia Warszawa", POLAND, "Warsaw", 1916);
        cm.club("Lech Poznań", POLAND, "Poznań", 1922);
    }

    ClubManager cm;
};

TEST_F(ClubManagerTest, AssignmentOperation) {
    ClubManager copy;
    copy = cm;

    Club* c1 = copy.findClubByName("Legia Warszawa");
    EXPECT_NE(c1, nullptr);
    EXPECT_EQ(c1->data.country, POLAND);
    EXPECT_EQ(c1->data.founded_year, 1916);
    EXPECT_STREQ(c1->data.city, "Warsaw");
}

TEST_F(ClubManagerTest, CopyConstructor) {
    ClubManager copy(cm);

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
    cm.club("Legia Warszawa", POLAND, "Warszawa", 1912);

    Club* c1 = cm.findClubByName("Legia Warszawa");
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

    Club* c1 = cm.findClubByName("Legia Warszawa");
    EXPECT_NE(c1, nullptr);
}

TEST_F(ClubManagerTest, FindClubsByCountry)
{
    const auto list = cm.getAllClubsCollection();
    const vector<Club*> c1 = cm.findClubsByCountry(POLAND, list);

    EXPECT_NE(c1.front(), nullptr);
    EXPECT_NE(c1.back(), nullptr);
}

TEST_F(ClubManagerTest, FindClubsByCity)
{
    cm.club("Polonia Warszawa", POLAND, "Warsaw", 1927);

    auto list = cm.getAllClubsCollection();

    vector<Club*> c1 = cm.findClubsByCity("Warsaw", list);

    EXPECT_NE(c1.front(), nullptr);
}

TEST_F(ClubManagerTest, FindClubsByFoundedYear)
{
    const auto list = cm.getAllClubsCollection();
    vector<Club*> c1 = cm.findClubsByFoundedYear(1916, list);

    EXPECT_NE(c1.front(), nullptr);
    EXPECT_EQ(c1.front(), c1.back());
}

TEST_F(ClubManagerTest, FindClubsByNewerFoundedYear)
{
    const auto list = cm.getAllClubsCollection();
    vector<Club*> c1 = cm.findClubsByNewerFoundedYear(1910, list);

    EXPECT_NE(c1.front(), nullptr);
}

TEST_F(ClubManagerTest, FindClubsByOlderFoundedYear)
{
    const auto list = cm.getAllClubsCollection();
    vector<Club*> c1 = cm.findClubsByOlderFoundedYear(1910, list);

    EXPECT_NE(c1.front(), nullptr);
    EXPECT_EQ(c1.front(), c1.back());
}

TEST_F(ClubManagerTest, ChainFilters)
{
    const auto list = cm.getAllClubsCollection();

    const auto poland = cm.findClubsByCountry(POLAND, list);
    const auto warsaw = cm.findClubsByCity("Warsaw", poland);

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
    const bool success = cm.deleteClub(&c);
    ASSERT_FALSE(success);
}

TEST_F(ClubManagerTest, DeleteClub)
{

    Club* c1 = cm.findClubByName("Legia Warszawa");

    const bool success = cm.deleteClub(c1);
    ASSERT_TRUE(success);

    c1 = cm.findClubByName("Legia Warszawa");
    EXPECT_EQ(c1, nullptr);
}

TEST_F(ClubManagerTest, DeleteAllClubs)
{
    cm.deleteAllClubs();

    Club* c1 = cm.findClubByName("Legia Warszawa");
    Club* c2 = cm.findClubByName("Lech Poznań");

    EXPECT_EQ(c1, nullptr);
    EXPECT_EQ(c2, nullptr);
}

TEST_F(ClubManagerTest, DisplayClub)
{
    auto list = cm.getAllClubsCollection();

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    cm.displayClub(list.front());

    std::cout.rdbuf(old);
    std::string output = buffer.str();

    ASSERT_NE(output.find(list.front()->data.name), std::string::npos);
}

TEST_F(ClubManagerTest, DisplayClubList)
{
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    cm.displayClubList();

    std::cout.rdbuf(old);
    std::string output = buffer.str();

    ASSERT_NE(output.find("PSG"), std::string::npos);
    ASSERT_NE(output.find("FC Bayern Munich"), std::string::npos);
    ASSERT_NE(output.find("Legia Warszawa"), std::string::npos);
    ASSERT_NE(output.find("Lech Poznań"), std::string::npos);

    cm.deleteAllClubs();
}

TEST_F(ClubManagerTest, AddStadiumToClub)
{
    Club* club = cm.findClubByName("Legia Warszawa");

    StadiumData data = {"Stadion Wojska Polskiego", POLAND, "Warsaw", 31000};
    Stadium* stadium = new Stadium{data, nullptr};

    ClubManager::addStadiumToClub(stadium, club);

    int count = ClubManager::getClubStadiumsCount(club);
    EXPECT_EQ(count, 1);

    delete stadium;
}

TEST_F(ClubManagerTest, AddPlayerToClub)
{
    Club* club = cm.findClubByName("Legia Warszawa");
    ASSERT_NE(club, nullptr);

    PersonManager pm;

    pm.person("Jan", "Kowalski", 25, POLAND);
    Person* p = pm.findPersonById(0);
    ASSERT_NE(p, nullptr);

    cm.addPlayerToClub(p, FORWARD, club);

    const int count = cm.getClubPlayersCount(club);
    EXPECT_EQ(count, 1);
}

TEST_F(ClubManagerTest, AddStaffToClub)
{
    Club* club = cm.findClubByName("Legia Warszawa");
    ASSERT_NE(club, nullptr);

    PersonManager pm;

    pm.person("Adam", "Trener", 45, POLAND);

    Person* p = pm.findPersonById(0);
    ASSERT_NE(p, nullptr);

    cm.addStaffToClub(p, COACH, club);

    const int count = cm.getClubStaffCount(club);
    EXPECT_EQ(count, 1);
}

TEST_F(ClubManagerTest, GetClubStadiumsCountWithEmptyList)
{
    Club* club = cm.findClubByName("Legia Warszawa");

    int count = ClubManager::getClubStadiumsCount(club);
    EXPECT_EQ(count, 0);
}

TEST_F(ClubManagerTest, GetClubPlayersCountWithEmptyList)
{
    Club* club = cm.findClubByName("Legia Warszawa");

    int count = ClubManager::getClubPlayersCount(club);
    EXPECT_EQ(count, 0);
}

TEST_F(ClubManagerTest, GetClubStaffCountWithEmptyList)
{
    Club* club = cm.findClubByName("Legia Warszawa");

    const int count = ClubManager::getClubStaffCount(club);
    EXPECT_EQ(count, 0);
}

TEST_F(ClubManagerTest, GetClubStadiumsCountWithExistingList)
{
    Club* club = cm.findClubByName("Legia Warszawa");

    StadiumData data1 = {"Stadion Wojska Polskiego", POLAND, "Warsaw", 31000};
    auto* stadium1 = new Stadium{data1, nullptr};

    StadiumData data2 = {"Stadion Legii", POLAND, "Warsaw", 20000};
    auto* stadium2 = new Stadium{data2, nullptr};

    ClubManager::addStadiumToClub(stadium1, club);
    ClubManager::addStadiumToClub(stadium2, club);

    const int count = ClubManager::getClubStadiumsCount(club);
    EXPECT_EQ(count, 2);

    delete stadium1;
    delete stadium2;
}

TEST_F(ClubManagerTest, GetClubPlayersCountWithExistingList)
{
    Club* club = cm.findClubByName("Legia Warszawa");

    PersonManager pm;

    pm.person("Jan", "Kowalski", 25, POLAND);
    pm.person("Adam", "Nowak", 30, POLAND);

    auto* p1 = PersonManager::findPeopleByName("Jan", "Kowalski", pm.getAllPeopleCollection())[0];
    auto* p2  = PersonManager::findPeopleByName("Adam", "Nowak", pm.getAllPeopleCollection())[0];


    cm.addPlayerToClub(p1, FORWARD, club);
    cm.addPlayerToClub(p2, MIDFIELDER, club);

    const int count = cm.getClubPlayersCount(club);
    EXPECT_EQ(count, 2);
}

TEST_F(ClubManagerTest, GetClubStaffCountWithExistingList)
{
    Club* club = cm.findClubByName("Legia Warszawa");

    PersonManager pm;

    pm.person("Adam", "Trener", 45, POLAND);
    pm.person("Ewa", "Asystent", 35, POLAND);

    auto* p1 = PersonManager::findPeopleByName("Adam", "Trener", pm.getAllPeopleCollection())[0];
    auto* p2  = PersonManager::findPeopleByName("Ewa", "Asystent", pm.getAllPeopleCollection())[0];

    cm.addStaffToClub(p1, COACH, club);
    cm.addStaffToClub(p2, PHYSIOTHERAPIST, club);

    const int count = cm.getClubStaffCount(club);
    EXPECT_EQ(count, 2);
}

TEST_F(ClubManagerTest, RemovePersonFromClub)
{
    Club* club = cm.findClubByName("Legia Warszawa");

    PersonManager pm;

    pm.person("Adam", "Trener", 45, POLAND);
    auto* p1 = PersonManager::findPeopleByName("Adam", "Trener", pm.getAllPeopleCollection())[0];

    cm.addStaffToClub(p1, COACH, club);

    bool removed = ClubManager::removePersonFromClub(p1, club);
    EXPECT_TRUE(removed);

    const int count = cm.getClubStaffCount(club);
    EXPECT_EQ(count, 0);
}

TEST_F(ClubManagerTest, RemoveStadiumFromClub)
{
    Club* club = cm.findClubByName("Legia Warszawa");

    StadiumData data = {"Stadion Wojska Polskiego", POLAND, "Warsaw", 31000};
    auto* stadium = new Stadium{data, nullptr};

    ClubManager::addStadiumToClub(stadium, club);

    bool removed = ClubManager::removeStadiumFromClub(stadium, club);
    EXPECT_TRUE(removed);

    const int count = ClubManager::getClubStadiumsCount(club);
    EXPECT_EQ(count, 0);

    delete stadium;
}

TEST_F(ClubManagerTest, CopyConstructorPerformsDeepCopy) {
    Club* originalClub = cm.findClubByName("Legia Warszawa");

    PersonManager pm;
    pm.person("Test", "Player", 25, POLAND);
    Person* p = PersonManager::findPeopleByName("Test", "Player", pm.getAllPeopleCollection())[0];

    cm.addPlayerToClub(p, FORWARD, originalClub);

    ClubManager copy(cm);
    Club* copiedClub = copy.findClubByName("Legia Warszawa");

    ASSERT_NE(copiedClub, nullptr);
    ASSERT_EQ(copiedClub->data.players.size(), 1);


    EXPECT_NE(copiedClub->data.players[0], originalClub->data.players[0]);

    EXPECT_EQ(copiedClub->data.players[0]->person, originalClub->data.players[0]->person);
}

TEST_F(ClubManagerTest, PipelineFiltersVeteranForwards) {
    Club* club = cm.findClubByName("Legia Warszawa");
    PersonManager pm;

    // Create 3 people
    pm.person("Veteran", "Forward", 35, POLAND);
    pm.person("Young", "Forward", 20, POLAND);
    pm.person("Veteran", "Defender", 35, POLAND);

    auto p1 = PersonManager::findPeopleByName("Veteran", "Forward", pm.getAllPeopleCollection())[0];
    auto p2 = PersonManager::findPeopleByName("Young", "Forward", pm.getAllPeopleCollection())[0];
    auto p3 = PersonManager::findPeopleByName("Veteran", "Defender", pm.getAllPeopleCollection())[0];

    cm.addPlayerToClub(p1, FORWARD, club);
    cm.addPlayerToClub(p2, FORWARD, club);
    cm.addPlayerToClub(p3, DEFENDER, club);

    vector<Player*> forwards = ClubManager::getPlayersByPosition(FORWARD, club);
    EXPECT_EQ(forwards.size(), 2);

    vector<Person*> forwardPeople = ClubManager::extractPeopleFromPlayers(forwards);
    EXPECT_EQ(forwardPeople.size(), 2);

    vector<Person*> veteranForwards = PersonManager::findPeopleOlderThan(29, forwardPeople);

    ASSERT_EQ(veteranForwards.size(), 1);
    EXPECT_STREQ(veteranForwards[0]->data.name, "Veteran");
    EXPECT_STREQ(veteranForwards[0]->data.surname, "Forward");
}
