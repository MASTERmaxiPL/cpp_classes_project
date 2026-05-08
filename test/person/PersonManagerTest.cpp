#include <gtest/gtest.h>
#include <sstream>
#include <string>

#include "../../src/person/PersonManager.h"
#include "../../src/utils/Country.h"
#include "../../src/club/ClubManager.h"

class PersonManagerFullTest : public testing::Test {
protected:
    void SetUp() override {
        pm.person("Robert", "Lewandowski", 34, POLAND);
        p1 = PersonManager::findPeopleByName("Robert", "Lewandowski", pm.getAllPeopleCollection())[0];

        pm.person("Michal", "Grabara", 26, POLAND);
        p2 = PersonManager::findPeopleByName("Michal", "Grabara", pm.getAllPeopleCollection())[0];

        pm.person("Lucas", "Hernandez", 30, FRANCE);
        p3 = PersonManager::findPeopleByName("Lucas", "Hernandez", pm.getAllPeopleCollection())[0];

        pm.person("Pep", "Guardiola", 52, SPAIN);
        s1 = PersonManager::findPeopleByName("Pep", "Guardiola", pm.getAllPeopleCollection())[0];

        pm.person("Alvarez", "Nunca", 41, SPAIN);
        s2 = PersonManager::findPeopleByName("Alvarez", "Nunca", pm.getAllPeopleCollection())[0];

        pm.person("Zinedine", "Zidane", 50, FRANCE);
        s3 = PersonManager::findPeopleByName("Zinedine", "Zidane", pm.getAllPeopleCollection())[0];
    }

    PersonManager pm;

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
    
    pm.person("Lionel", "Messi", 36, ARGENTINA);
    Person* p = PersonManager::findPeopleByName("Lionel", "Messi", pm.getAllPeopleCollection())[0];

    auto list = pm.getAllPeopleCollection();
    EXPECT_EQ(list.size(), 1);
    
    Person* found = pm.findPersonById(p->id);
    EXPECT_NE(found, nullptr);
    EXPECT_STREQ(found->data.name, "Lionel");
    EXPECT_STREQ(found->data.surname, "Messi");
    EXPECT_EQ(found->data.age, 36);
    EXPECT_EQ(found->data.nationality, ARGENTINA);
}

TEST_F(PersonManagerFullTest, AddPersonToExistingList)
{
    pm.person("Lionel", "Messi", 36, ARGENTINA);
    Person* p = PersonManager::findPeopleByName("Lionel", "Messi", pm.getAllPeopleCollection())[0];

    auto list = pm.getAllPeopleCollection();
    EXPECT_EQ(list.size(), 7);

    Person* found = pm.findPersonById(p->id);

    EXPECT_NE(found, nullptr);
    EXPECT_STREQ(found->data.name, "Lionel");
    EXPECT_STREQ(found->data.surname, "Messi");
    EXPECT_EQ(found->data.age, 36);
    EXPECT_EQ(found->data.nationality, ARGENTINA);
}

TEST_F(PersonManagerFullTest, UpdatePerson)
{
    Person* target = p1;
    PersonManager::updatePerson(target, "UpdatedName", "UpdatedSurname", 40, SPAIN);

    Person* found = pm.findPersonById(target->id);

    EXPECT_NE(found, nullptr);
    EXPECT_STREQ(found->data.name, "UpdatedName");
    EXPECT_STREQ(found->data.surname, "UpdatedSurname");
    EXPECT_EQ(found->data.age, 40);
    EXPECT_EQ(found->data.nationality, SPAIN);
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


TEST(PersonManagerDeletionTest, DeletePersonFromEmptyList) {
    PersonManager emptyPm;
    const bool success = emptyPm.deletePerson(1);
    ASSERT_FALSE(success);
}

TEST_F(PersonManagerFullTest, DeleteExistingPerson) {
    uint32_t targetId = p1->id;

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

    EXPECT_EQ(cm.getClubPlayersCount(club), 0);

    cm.addPlayerToClub(testP, FORWARD, club);

    EXPECT_EQ(cm.getClubPlayersCount(club), 1);

    const bool success = pm.deletePerson(testP->id);
    EXPECT_TRUE(success);

    EXPECT_EQ(cm.getClubPlayersCount(club), 0);
}