#include "PlayerManager.h"
#include <iostream>

#include "club/ClubManager.h"
#include "person/PersonManager.h"

using namespace std;

PlayerManager::~PlayerManager() {
    removeAllPlayers();
}

Player* PlayerManager::addPlayer(Person* person, Position position) {
    if (!person) return nullptr;

    auto* newPlayer = new Player();
    newPlayer->person = person;
    newPlayer->position = position;

    players.push_back(newPlayer);
    return newPlayer;
}

void PlayerManager::updatePlayerPosition(Player* player, Position newPosition) {
    if (player) {
        player->position = newPosition;
    }
}

vector<Player*> PlayerManager::filterPlayers(const function<bool(Player*)>& predicate) const {
    vector<Player*> results;
    for (Player* player : players) {
        if (player && predicate(player)) {
            results.push_back(player);
        }
    }
    return results;
}

Player* PlayerManager::findPlayerByPersonId(uint32_t personId) const {
    for (Player* player : players) {
        if (player && player->person && player->person->id == personId) {
            return player;
        }
    }
    return nullptr;
}

vector<Player*> PlayerManager::findPlayersByPosition(Position position) const {
    return filterPlayers([position](Player* p) {
        return p->position == position;
    });
}

bool PlayerManager::removePlayerByPersonId(uint32_t personId) {
    for (auto it = players.begin(); it != players.end(); ++it) {
        if ((*it)->person && (*it)->person->id == personId) {

            if ((*it)->person->hiredBy) {
                ClubManager::removePersonFromClub((*it)->person, (*it)->person->hiredBy);
            }

            delete *it;
            players.erase(it);
            return true;
        }
    }
    return false;
}

void PlayerManager::removeAllPlayers() {
    for (Player* player : players) {
        delete player;
    }
    players.clear();
}

void PlayerManager::displayPlayer(const Player* player)
{
    if (!player || !player->person) return;

    cout << "ID: " << player->person->id
         << " | " << player->person->data.name << " " << player->person->data.surname
         << " | Position: " << player->position << endl;
}

void PlayerManager::displayAllPlayers() const {
    cout << "--- Players List ---" << endl;
    for (const auto* player : players) {
        displayPlayer(player);
    }
    cout << "--------------------" << endl;
}