#include <gtest/gtest.h>
#include "../src/person/player/PlayerManager.h"
#include "../src/person/PersonManager.h"

class PlayerManagerTest : public ::testing::Test {
protected:
    PersonManager pm;
    PlayerManager plm;
    Person* p1;
    Person* p2;

    void SetUp() override {
        pm.person("Wojciech", "Szczesny", 34, POLAND);
        pm.person("Kamil", "Glik", 36, POLAND);

        p1 = PersonManager::findPeopleByName("Wojciech", "Szczesny", pm.getAllPeopleCollection())[0];
        p2 = PersonManager::findPeopleByName("Kamil", "Glik", pm.getAllPeopleCollection())[0];
    }
};

TEST_F(PlayerManagerTest, AddPlayer) {
    Player* player = plm.addPlayer(p1, GOALKEEPER);

    ASSERT_NE(player, nullptr);
    EXPECT_EQ(player->person->id, p1->id);
    EXPECT_EQ(player->position, GOALKEEPER);
}

TEST_F(PlayerManagerTest, FindPlayerByPersonId) {
    plm.addPlayer(p1, GOALKEEPER);
    plm.addPlayer(p2, DEFENDER);

    Player* found = plm.findPlayerByPersonId(p2->id);

    ASSERT_NE(found, nullptr);
    EXPECT_EQ(found->position, DEFENDER);
    EXPECT_STREQ(found->person->data.name, "Kamil");
}

TEST_F(PlayerManagerTest, FindPlayersByPosition) {
    plm.addPlayer(p1, GOALKEEPER);
    plm.addPlayer(p2, DEFENDER);
    pm.person("Matty", "Cash", 26, POLAND);
    Person* p3 = PersonManager::findPeopleByName("Matty", "Cash", pm.getAllPeopleCollection())[0];
    plm.addPlayer(p3, DEFENDER);

    const auto defenders = plm.findPlayersByPosition(DEFENDER);

    EXPECT_EQ(defenders.size(), 2);
}

TEST_F(PlayerManagerTest, UpdatePlayerPosition) {
    Player* player = plm.addPlayer(p2, MIDFIELDER);
    plm.updatePlayerPosition(player, DEFENDER);

    EXPECT_EQ(player->position, DEFENDER);
}

TEST_F(PlayerManagerTest, RemovePlayerByPersonId) {
    plm.addPlayer(p1, GOALKEEPER);

    const bool removed = plm.removePlayerByPersonId(p1->id);
    EXPECT_TRUE(removed);

    Player* found = plm.findPlayerByPersonId(p1->id);
    EXPECT_EQ(found, nullptr);
}