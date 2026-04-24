#include <gtest/gtest.h>

#include "../src/person/staff/StaffManager.h"
#include "../src/person/PersonManager.h"


class StaffManagerTest : public ::testing::Test {
protected:
    PersonManager pm;
    StaffManager sm;
    Person* p1;
    Person* p2;

    void SetUp() override {
        pm.person("Michal", "Probierz", 51, POLAND);
        pm.person("Remigiusz", "Rzepka", 45, POLAND);

        p1 = PersonManager::findPeopleByName("Michal", "Probierz", pm.getAllPeopleCollection())[0];
        p2 = PersonManager::findPeopleByName("Remigiusz", "Rzepka", pm.getAllPeopleCollection())[0];
    }
};

TEST_F(StaffManagerTest, AddStaff) {
    Staff* staff = sm.addStaff(p1, MANAGER);

    ASSERT_NE(staff, nullptr);
    EXPECT_EQ(staff->person->id, p1->id);
    EXPECT_EQ(staff->role, MANAGER);
}

TEST_F(StaffManagerTest, FindStaffByPersonId) {
    sm.addStaff(p1, MANAGER);
    sm.addStaff(p2, PHYSIOTHERAPIST);

    Staff* found = sm.findStaffByPersonId(p2->id);

    ASSERT_NE(found, nullptr);
    EXPECT_EQ(found->role, PHYSIOTHERAPIST);
    EXPECT_STREQ(found->person->data.name, "Remigiusz");
}

TEST_F(StaffManagerTest, FindStaffByRole) {
    sm.addStaff(p1, MANAGER);
    pm.person("Pep", "Guardiola", 53, SPAIN);
    Person* p3 = pm.findPeopleByName("Pep", "Guardiola", pm.getAllPeopleCollection())[0];
    sm.addStaff(p3, MANAGER);

    const auto managers = sm.findStaffByRole(MANAGER);

    EXPECT_EQ(managers.size(), 2);
}

TEST_F(StaffManagerTest, UpdateStaffRole) {
    Staff* staff = sm.addStaff(p2, OTHER);
    sm.updateStaffRole(staff, PHYSIOTHERAPIST);

    EXPECT_EQ(staff->role, PHYSIOTHERAPIST);
}

TEST_F(StaffManagerTest, RemoveStaffByPersonId) {
    sm.addStaff(p1, MANAGER);

    bool removed = sm.removeStaffByPersonId(p1->id);
    EXPECT_TRUE(removed);

    Staff* found = sm.findStaffByPersonId(p1->id);
    EXPECT_EQ(found, nullptr);
}