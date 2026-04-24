#include <gtest/gtest.h>
#include <sstream>
#include <string>

#include "../../src/person/PersonManager.h"
#include "../../src/person/player/PlayerManager.h"
#include "../../src/person/staff/StaffManager.h"
#include "../../src/utils/Country.h"
#include "../../src/club/ClubManager.h"

class PersonManagerFullTest : public testing::Test {
protected:
    void SetUp() override {
        pm.person("Robert", "Lewandowski", 34, POLAND);
        p1 = PersonManager::findPeopleByName("Robert", "Lewandowski", pm.getAllPeopleCollection())[0];
        plm.addPlayer(p1, FORWARD);

        pm.person("Michal", "Grabara", 26, POLAND);
        p2 = PersonManager::findPeopleByName("Michal", "Grabara", pm.getAllPeopleCollection())[0];
        plm.addPlayer(p2, GOALKEEPER);

        pm.person("Lucas", "Hernandez", 30, FRANCE);
        p3 = PersonManager::findPeopleByName("Lucas", "Hernandez", pm.getAllPeopleCollection())[0];
        plm.addPlayer(p3, DEFENDER);

        pm.person("Pep", "Guardiola", 52, SPAIN);
        s1 = PersonManager::findPeopleByName("Pep", "Guardiola", pm.getAllPeopleCollection())[0];
        stm.addStaff(s1, MANAGER);

        pm.person("Alvarez", "Nunca", 41, SPAIN);
        s2 = PersonManager::findPeopleByName("Alvarez", "Nunca", pm.getAllPeopleCollection())[0];
        stm.addStaff(s2, PHYSIOTHERAPIST);

        pm.person("Zinedine", "Zidane", 50, FRANCE);
        s3 = PersonManager::findPeopleByName("Zinedine", "Zidane", pm.getAllPeopleCollection())[0];
        stm.addStaff(s3, COACH);
    }

    PersonManager pm;
    PlayerManager plm;
    StaffManager stm;

    Person* p1{};
    Person* p2{};
    Person* p3{};
    Person* s1{};
    Person* s2{};
    Person* s3{};
};

TEST_F(PersonManagerFullTest, AssignmentOperation) {
    PersonManager copy;
    copy = pm;

    auto list = copy.getAllPeopleCollection();
    EXPECT_EQ(list.size(), 6);
    EXPECT_NE(list[0], pm.getAllPeopleCollection()[0]);
    EXPECT_STREQ(list[0]->data.name, pm.getAllPeopleCollection()[0]->data.name);
}

TEST_F(PersonManagerFullTest, CopyConstructor) {
    PersonManager copy(pm);

    auto list = copy.getAllPeopleCollection();
    EXPECT_EQ(list.size(), 6);
    EXPECT_NE(list[0], pm.getAllPeopleCollection()[0]);
    EXPECT_STREQ(list[0]->data.name, pm.getAllPeopleCollection()[0]->data.name);
}

TEST(PersonManagerAdditionTest, AddPersonToEmptyList) {
    PersonManager pm;
    PlayerManager plm;
    
    pm.person("Lionel", "Messi", 36, ARGENTINA);
    Person* p = PersonManager::findPeopleByName("Lionel", "Messi", pm.getAllPeopleCollection())[0];
    Player* pl = plm.addPlayer(p, FORWARD);

    auto list = pm.getAllPeopleCollection();
    EXPECT_EQ(list.size(), 1);
    
    Person* found = pm.findPersonById(p->id);
    EXPECT_NE(found, nullptr);
    EXPECT_STREQ(found->data.name, "Lionel");
    EXPECT_STREQ(found->data.surname, "Messi");
    EXPECT_EQ(found->data.age, 36);
    EXPECT_EQ(found->data.nationality, ARGENTINA);
    EXPECT_EQ(pl->position, FORWARD);
}

TEST_F(PersonManagerFullTest, AddPlayerToExistingList) {
    pm.person("Lionel", "Messi", 36, ARGENTINA);
    Person* p = PersonManager::findPeopleByName("Lionel", "Messi", pm.getAllPeopleCollection())[0];
    Player* pl = plm.addPlayer(p, FORWARD);

    auto list = pm.getAllPeopleCollection();
    EXPECT_EQ(list.size(), 7);

    Person* found = pm.findPersonById(p->id);
    EXPECT_NE(found, nullptr);
    EXPECT_STREQ(found->data.name, "Lionel");
    EXPECT_EQ(found->data.age, 36);
    
    Player* foundPlayer = plm.findPlayerByPersonId(p->id);
    EXPECT_NE(foundPlayer, nullptr);
    EXPECT_EQ(foundPlayer->position, FORWARD);
}

TEST_F(PersonManagerFullTest, AddStaffToExistingList) {
    pm.person("Lionel", "Messi", 56, ARGENTINA);
    Person* p = pm.findPersonById(6);
    stm.addStaff(p, COACH);

    auto list = pm.getAllPeopleCollection();
    EXPECT_EQ(list.size(), 7);

    Staff* foundStaff = stm.findStaffByPersonId(p->id);
    EXPECT_NE(foundStaff, nullptr);
    EXPECT_EQ(foundStaff->role, COACH);
    EXPECT_EQ(foundStaff->person->data.age, 56);
}

TEST_F(PersonManagerFullTest, EditPlayer) {
    pm.updatePerson(p1, "Lionel", "Messi", 36, ARGENTINA);
    
    Player* pl = plm.findPlayerByPersonId(p1->id);
    ASSERT_NE(pl, nullptr);
    plm.updatePlayerPosition(pl, FORWARD);

    EXPECT_STREQ(p1->data.name, "Lionel");
    EXPECT_STREQ(p1->data.surname, "Messi");
    EXPECT_EQ(p1->data.age, 36);
    EXPECT_EQ(p1->data.nationality, ARGENTINA);
    EXPECT_EQ(pl->position, FORWARD);
}

TEST_F(PersonManagerFullTest, EditStaff) {
    pm.updatePerson(s1, "Lionel", "Messi", 56, ARGENTINA);
    
    Staff* st = stm.findStaffByPersonId(s1->id);
    ASSERT_NE(st, nullptr);
    stm.updateStaffRole(st, COACH);

    EXPECT_STREQ(s1->data.name, "Lionel");
    EXPECT_STREQ(s1->data.surname, "Messi");
    EXPECT_EQ(s1->data.age, 56);
    EXPECT_EQ(s1->data.nationality, ARGENTINA);
    EXPECT_EQ(st->role, COACH);
}

TEST(PersonManagerGetters, getAllPeopleCollectionFromEmptyList) {
    PersonManager emptyPm;
    auto list = emptyPm.getAllPeopleCollection();
    EXPECT_TRUE(list.empty());
}

TEST(PersonManagerGetters, getAllPeopleCollectionFromExistingList) {
    PersonManager pm;
    pm.person("Lionel", "Messi", 36, ARGENTINA);
    pm.person("Robert", "Lewandowski", 34, POLAND);

    auto list = pm.getAllPeopleCollection();
    EXPECT_EQ(list.size(), 2);
}

TEST_F(PersonManagerFullTest, FindPersonById) {
    Person* found = pm.findPersonById(p1->id);
    EXPECT_NE(found, nullptr);
    EXPECT_STREQ(found->data.name, "Robert");
}

TEST_F(PersonManagerFullTest, FindPeopleByName) {
    auto list = pm.getAllPeopleCollection();
    auto filtered = PersonManager::findPeopleByName("Robert", "Lewandowski", list);
    
    ASSERT_EQ(filtered.size(), 1);
    EXPECT_STREQ(filtered[0]->data.name, "Robert");
}

TEST_F(PersonManagerFullTest, FindPeopleByAge) {
    auto list = pm.getAllPeopleCollection();
    auto filtered = PersonManager::findPeopleByAge(34, list);
    
    ASSERT_EQ(filtered.size(), 1);
    EXPECT_EQ(filtered[0]->id, p1->id);
}

TEST_F(PersonManagerFullTest, FindPeopleYoungerThan) {
    auto list = pm.getAllPeopleCollection();
    auto filtered = PersonManager::findPeopleYoungerThan(31, list);
    
    EXPECT_EQ(filtered.size(), 2);
}

TEST_F(PersonManagerFullTest, FindPeopleOlderThan) {
    auto list = pm.getAllPeopleCollection();
    auto filtered = PersonManager::findPeopleOlderThan(45, list);
    
    EXPECT_EQ(filtered.size(), 2);
}

TEST_F(PersonManagerFullTest, FindPeopleByNationality) {
    auto list = pm.getAllPeopleCollection();
    auto filtered = PersonManager::findPeopleByNationality(POLAND, list);
    
    EXPECT_EQ(filtered.size(), 2);
}

TEST_F(PersonManagerFullTest, FindPlayersByPosition) {
    auto filtered = plm.findPlayersByPosition(GOALKEEPER);
    
    ASSERT_EQ(filtered.size(), 1);
    EXPECT_STREQ(filtered[0]->person->data.name, "Michal");
}

TEST_F(PersonManagerFullTest, FindStaffByRole) {
    auto filtered = stm.findStaffByRole(MANAGER);
    
    ASSERT_EQ(filtered.size(), 1);
    EXPECT_STREQ(filtered[0]->person->data.surname, "Guardiola");
}

TEST(PersonManagerDeletionTest, DeletePersonFromEmptyList) {
    PersonManager emptyPm;
    const bool success = emptyPm.deletePerson(1);
    ASSERT_FALSE(success);
}

TEST_F(PersonManagerFullTest, DeleteExistingPerson) {
    uint32_t targetId = p1->id;
    
    plm.removePlayerByPersonId(targetId);
    const bool success = pm.deletePerson(targetId);
    
    ASSERT_TRUE(success);
    EXPECT_EQ(pm.findPersonById(targetId), nullptr);
}

TEST_F(PersonManagerFullTest, DisplayPerson) {
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    pm.displayPerson(p2);

    std::cout.rdbuf(old);
    std::string output = buffer.str();

    ASSERT_NE(output.find("Michal"), std::string::npos);
    ASSERT_NE(output.find("Grabara"), std::string::npos);
}

TEST_F(PersonManagerFullTest, DisplayPeopleList) {
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    pm.displayPeopleList();

    std::cout.rdbuf(old);
    std::string output = buffer.str();

    ASSERT_NE(output.find("Robert"), std::string::npos);
    ASSERT_NE(output.find("Lewandowski"), std::string::npos);
    ASSERT_NE(output.find("Pep"), std::string::npos);
    ASSERT_NE(output.find("Zidane"), std::string::npos);
}

TEST_F(PersonManagerFullTest, DeletePersonRemovesFromClub) {
    ClubManager cm;
    cm.club("TestClub", POLAND, "City", 1900);
    Club* club = cm.findClubByName("TestClub");
    ASSERT_NE(club, nullptr);

    pm.person("Test", "Player", 28, POLAND);
    Person* testP = PersonManager::findPeopleByName("Test", "Player", pm.getAllPeopleCollection())[0];
    Player* testPl = plm.addPlayer(testP, MIDFIELDER);

    EXPECT_EQ(cm.getClubPlayersCount(club), 0);

    cm.addPlayerToClub(testPl, club);
    EXPECT_EQ(cm.getClubPlayersCount(club), 1);

    const bool success = pm.deletePerson(testP->id);
    EXPECT_TRUE(success);
    plm.removePlayerByPersonId(testP->id);

    EXPECT_EQ(cm.getClubPlayersCount(club), 0);
}