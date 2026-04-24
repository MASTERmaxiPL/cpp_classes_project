#ifndef FOOTBALL_MANAGEMENT_SYSTEM_PLAYER_MANAGER_H
#define FOOTBALL_MANAGEMENT_SYSTEM_PLAYER_MANAGER_H
#pragma once

#include <vector>
#include <functional>
#include "Player.h"

class PlayerManager {
    vector<Player*> players;

public:
    PlayerManager() = default;
    ~PlayerManager();

    Player* addPlayer(Person* person, Position position);
    static void updatePlayerPosition(Player* player, Position newPosition);

    vector<Player*> filterPlayers(const function<bool(Player*)>& predicate) const;
    Player* findPlayerByPersonId(uint32_t personId) const;
    vector<Player*> findPlayersByPosition(Position position) const;

    bool removePlayerByPersonId(uint32_t personId);
    void removeAllPlayers();

    static void displayPlayer(const Player* player);
    void displayAllPlayers() const;
};

#endif