#include <codecvt>
#include <gtest/gtest.h>

#include "../../src/person/PersonManager.h"
#include "../../src/utils/Country.h"

class PersonManagerTest : public testing::Test
{
    protected:
    void SetUp() override
    {
        pm.player("Robert", "Lewandowski", 34, POLAND, FORWARD);
        pm.player("Michał", "Grabara", 26, POLAND, GOALKEEPER);
        pm.player("Lucas", "Hernandez", 30, FRANCE, DEFENDER);
        pm.staff("Pep", "Guardiola", 52, SPAIN, MANAGER);
        pm.staff("Alvarez", "Nunca", 41, SPAIN, PHYSIOTHERAPIST);
        pm.staff("Zinedine", "Zidane", 50, FRANCE, COACH);
    }

    PersonManager pm;
};

TEST(PersonManagerAdditionTest, AddPersonToEmptyList)
{
    PersonManager pm;
    pm.player("Lionel", "Messi", 36, ARGENTINA, FORWARD);

    PersonListNode* list = pm.getAllPeopleWrapped();

    Person* p1 = pm.findPersonById(0, list);
    EXPECT_NE(p1, nullptr);
    EXPECT_EQ(p1->data.name, string("Lionel"));
    EXPECT_EQ(p1->data.surname, string("Messi"));
    EXPECT_EQ(p1->data.age, 36);
    EXPECT_EQ(p1->data.nationality, ARGENTINA);
    EXPECT_EQ(dynamic_cast<Player*>(p1)->position, FORWARD);
}

TEST_F(PersonManagerTest, AddPlayerToExistingList)
{
    pm.player("Lionel", "Messi", 36, ARGENTINA, FORWARD);

    PersonListNode* list = pm.getAllPeopleWrapped();

    Person* p1 = pm.findPersonById(6, list);
    EXPECT_NE(p1, nullptr);
    EXPECT_EQ(p1->data.name, string("Lionel"));
    EXPECT_EQ(p1->data.surname, string("Messi"));
    EXPECT_EQ(p1->data.age, 36);
    EXPECT_EQ(p1->data.nationality, ARGENTINA);
    EXPECT_EQ(dynamic_cast<Player*>(p1)->position, FORWARD);
}

TEST_F(PersonManagerTest, AddPStaffToExistingList)
{
    pm.staff("Lionel", "Messi", 56, ARGENTINA, COACH);

    PersonListNode* list = pm.getAllPeopleWrapped();
    Person* s1 = pm.findPersonById(6, list);
    EXPECT_NE(s1, nullptr);
    EXPECT_EQ(s1->data.name, string("Lionel"));
    EXPECT_EQ(s1->data.surname, string("Messi"));
    EXPECT_EQ(s1->data.age, 56);
    EXPECT_EQ(s1->data.nationality, ARGENTINA);
    EXPECT_EQ(dynamic_cast<Staff*>(s1)->role, COACH);
}

TEST(PersonManagerGetters, GetAllPeopleWrappedFromEmptyList)
{
    PersonManager pm;

    PersonListNode* list = pm.getAllPeopleWrapped();
    EXPECT_EQ(list, nullptr);

    pm.deleteAllWrappedPeople(list);
}

TEST(PersonManagerGetters, GetAllPeopleWrappedFromExisingList)
{
    PersonManager pm;
    pm.player("Lionel", "Messi", 36, ARGENTINA, FORWARD);
    pm.player("Robert", "Lewandowski", 34, POLAND, FORWARD);

    PersonListNode* list = pm.getAllPeopleWrapped();
    EXPECT_NE(list, nullptr);
    EXPECT_NE(list->next, nullptr);
    EXPECT_EQ(list->next->next, nullptr);
}

TEST_F(PersonManagerTest, FindPersonById)
{
    PersonListNode* list = pm.getAllPeopleWrapped();
    Person* p1 = pm.findPersonById(1, list);

    EXPECT_NE(p1, nullptr);
}

TEST_F(PersonManagerTest, FindPeopleByName)
{
    PersonListNode* list = pm.getAllPeopleWrapped();

    PersonListNode* filtered = pm.findPeopleByName("Robert", "Lewandowski", list);

    EXPECT_NE(filtered, nullptr);
    EXPECT_EQ(filtered->next, nullptr);
}

TEST_F(PersonManagerTest, FindPeopleByAge)
{
    PersonListNode* list = pm.getAllPeopleWrapped();

    PersonListNode* filtered = pm.findPeopleByAge(34, list);

    EXPECT_NE(filtered, nullptr);
    EXPECT_EQ(filtered->next, nullptr);
}

TEST_F(PersonManagerTest, FindPeopleYoungerThan)
{
    PersonListNode* list = pm.getAllPeopleWrapped();

    PersonListNode* filtered = pm.findPeopleYoungerThan(31, list);

    EXPECT_NE(filtered, nullptr);
    EXPECT_NE(filtered->next, nullptr);
    EXPECT_EQ(filtered->next->next, nullptr);
}

TEST_F(PersonManagerTest, FindPeopleOlderThan)
{
    PersonListNode* list = pm.getAllPeopleWrapped();

    PersonListNode* filtered = pm.findPeopleOlderThan(45, list);

    EXPECT_NE(filtered, nullptr);
    EXPECT_NE(filtered->next, nullptr);
    EXPECT_EQ(filtered->next->next, nullptr);
}

TEST_F(PersonManagerTest, FindPeopleByNationality)
{
    PersonListNode* list = pm.getAllPeopleWrapped();

    PersonListNode* filtered = pm.findPeopleByNationality(POLAND, list);

    EXPECT_NE(filtered, nullptr);
    EXPECT_NE(filtered->next, nullptr);
    EXPECT_EQ(filtered->next->next, nullptr);
}

TEST_F(PersonManagerTest, FindPlayersByPosition)
{
    PersonListNode* list = pm.getAllPeopleWrapped();

    PersonListNode* filtered = pm.findPlayersByPosition(GOALKEEPER, list);

    EXPECT_NE(filtered, nullptr);

    EXPECT_EQ(strcmp(filtered->person->data.name, "Michał"), 0);
}

TEST_F(PersonManagerTest, FindStaffByRole)
{
    PersonListNode* list = pm.getAllPeopleWrapped();

    PersonListNode* filtered = pm.findStaffByRole(MANAGER, list);

    EXPECT_NE(filtered, nullptr);
    EXPECT_EQ(strcmp(filtered->person->data.surname, "Guardiola"), 0);
}

TEST(PersonManagerDeletionTest, DeletePersonFromEmptyList)
{
    PersonManager pm;

    const bool success = pm.deletePerson(1);
    ASSERT_FALSE(success);
}

TEST_F(PersonManagerTest, DeleteExistingPerson)
{
    PersonListNode* list = pm.getAllPeopleWrapped();
    Person* person = pm.findPersonById(1, list);
    ASSERT_NE(person, nullptr);

    const bool success = pm.deletePerson(1);
    ASSERT_TRUE(success);

    pm.deleteAllWrappedPeople(list);
}

TEST_F(PersonManagerTest, DeleteWrappedPerson)
{
    PersonListNode* list = pm.getAllPeopleWrapped();
    Person* person = pm.findPersonById(2, list);
    ASSERT_NE(person, nullptr);

    bool removed = pm.deleteWrappedPerson(list, 2);
    EXPECT_TRUE(removed);

    Person* again = pm.findPersonById(2, list);
    EXPECT_EQ(again, nullptr);

    pm.deleteAllWrappedPeople(list);
}

TEST_F(PersonManagerTest, DeleteAllWrappedPeople)
{
    PersonListNode* list = pm.getAllPeopleWrapped();

    pm.deleteAllWrappedPeople(list);

    Person* p1 = pm.findPersonById(1, list);
    Person* p2 = pm.findPersonById(2, list);

    EXPECT_EQ(p1, nullptr);
    EXPECT_EQ(p2, nullptr);
}

TEST_F(PersonManagerTest, DisplayPerson)
{
    PersonListNode* list = pm.getAllPeopleWrapped();
    Person* person = pm.findPersonById(1, list);
    ASSERT_NE(person, nullptr);

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    pm.displayPerson(person);

    std::cout.rdbuf(old);
    std::string output = buffer.str();

    ASSERT_NE(output.find("Michał"), std::string::npos);
    ASSERT_NE(output.find("Grabara"), std::string::npos);
}

TEST_F(PersonManagerTest, DisplayPeopleList)
{
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    pm.displayPeopleList();

    std::cout.rdbuf(old);
    std::string output = buffer.str();

    ASSERT_NE(output.find("Robert"), std::string::npos);
    ASSERT_NE(output.find("Lewandowski"), std::string::npos);
    ASSERT_NE(output.find("Michał"), std::string::npos);
    ASSERT_NE(output.find("Grabara"), std::string::npos);
    ASSERT_NE(output.find("Lucas"), std::string::npos);
    ASSERT_NE(output.find("Hernandez"), std::string::npos);
    ASSERT_NE(output.find("Pep"), std::string::npos);
    ASSERT_NE(output.find("Guardiola"), std::string::npos);
    ASSERT_NE(output.find("Alvarez"), std::string::npos);
    ASSERT_NE(output.find("Nunca"), std::string::npos);
    ASSERT_NE(output.find("Zinedine"), std::string::npos);
    ASSERT_NE(output.find("Zidane"), std::string::npos);
}

TEST_F(PersonManagerTest, DisplayWrappedPerson)
{
    PersonListNode* list = pm.getAllPeopleWrapped();
    ASSERT_NE(list, nullptr);

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    pm.displayWrappedPerson(list->next->next->next);

    std::cout.rdbuf(old);
    std::string output = buffer.str();

    ASSERT_NE(output.find("Guardiola"), std::string::npos);
    ASSERT_NE(output.find("52"), std::string::npos);

    pm.deleteAllWrappedPeople(list);
}

TEST_F(PersonManagerTest, DisplayWrappedPeopleList)
{
    PersonListNode* list = pm.getAllPeopleWrapped();

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    pm.displayWrappedPeopleList(list);

    std::cout.rdbuf(old);
    std::string output = buffer.str();

    ASSERT_NE(output.find("Robert"), std::string::npos);
    ASSERT_NE(output.find("Lewandowski"), std::string::npos);
    ASSERT_NE(output.find("Michał"), std::string::npos);
    ASSERT_NE(output.find("Grabara"), std::string::npos);
    ASSERT_NE(output.find("Lucas"), std::string::npos);
    ASSERT_NE(output.find("Hernandez"), std::string::npos);
    ASSERT_NE(output.find("Pep"), std::string::npos);
    ASSERT_NE(output.find("Guardiola"), std::string::npos);
    ASSERT_NE(output.find("Alvarez"), std::string::npos);
    ASSERT_NE(output.find("Nunca"), std::string::npos);
    ASSERT_NE(output.find("Zinedine"), std::string::npos);
    ASSERT_NE(output.find("Zidane"), std::string::npos);

    pm.deleteAllWrappedPeople(list);
}