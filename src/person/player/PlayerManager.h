#ifndef FOOTBALL_MANAGEMENT_SYSTEM_PLAYER_MANAGER_H
#define FOOTBALL_MANAGEMENT_SYSTEM_PLAYER_MANAGER_H
#pragma once

#include <vector>
#include <functional>
#include "Player.h"

class PlayerManager {
public:
    vector<Player*> players;

    PlayerManager() = default;
    ~PlayerManager();

    Player* addPlayer(Person* person, Position position);
    static void updatePlayerPosition(Player* player, Position newPosition);

    static vector<Player*> filterPlayers(const vector<Player*>& players, const function<bool(Player*)>& predicate);
    Player* findPlayerByPersonId(uint32_t personId) const;

    static vector<Player*> findPlayersByPosition(Position position, const vector<Player*>& players);

    bool removePlayerByPersonId(uint32_t personId);
    void removeAllPlayers();

    static void displayPlayer(const Player* player);
    void displayAllPlayers() const;
};

#endif