#include "ClubManager.h"
#include <iostream>
#include <cstring>
#include <vector>
#include <functional>

#include "../stadium/StadiumManager.h"
#include "../match/MatchManager.h"

using namespace std;

// --- CONSTRUCTORS & DESTRUCTORS ---
ClubManager::ClubManager(MatchManager* matchManager) : head(nullptr), matchManager(matchManager) {}

ClubManager::~ClubManager() {
    deleteAllClubs();
}

ClubManager::ClubManager(const ClubManager& other) : head(nullptr), matchManager(other.matchManager) {
    *this = other;
}

Club* ClubManager::copyClub(const Club* other) {
    if (!other) return nullptr;

    const auto newClub = new Club();
    newClub->data.country = other->data.country;
    newClub->data.founded_year = other->data.founded_year;
    newClub->next = nullptr;

    if (other->data.name) {
        newClub->data.name = new char[strlen(other->data.name) + 1];
        strcpy(newClub->data.name, other->data.name);
    } else {
        newClub->data.name = nullptr;
    }

    if (other->data.city) {
        newClub->data.city = new char[strlen(other->data.city) + 1];
        strcpy(newClub->data.city, other->data.city);
    } else {
        newClub->data.city = nullptr;
    }

    newClub->data.stadiums = other->data.stadiums;
    newClub->data.players = other->data.players;
    newClub->data.staff = other->data.staff;

    return newClub;
}

ClubManager& ClubManager::operator=(const ClubManager& other) {
    if (this == &other) return *this;

    deleteAllClubs();

    this->matchManager = other.matchManager;

    const Club* currentOther = other.head;
    Club** target = &this->head;

    while (currentOther) {
        *target = copyClub(currentOther);

        if (*target) {
            target = &((*target)->next);
        }

        currentOther = currentOther->next;
    }

    *target = nullptr;
    return *this;
}

// --- CREATION ---
void ClubManager::club(const char* name, const Country country) {
    club(name, country, nullptr, -1);
}

void ClubManager::club(const char* name, const Country country, const char* city) {
    club(name, country, city, -1);
}

void ClubManager::club(const char* name, const Country country, const int founded_year) {
    club(name, country, nullptr, founded_year);
}

void ClubManager::club(const char* name, const Country country, const char* city, const int founded_year) {
    if (!name) return;

    Club* curr = head;
    while (curr) {
        if (strcmp(curr->data.name, name) == 0) {
            cout << "Club with name " << name << " already exists. Club data will be edited" << endl;
            curr->data.country = country;
            if (founded_year != -1) curr->data.founded_year = founded_year;
            if (city) {
                delete [] curr->data.city;
                curr->data.city = new char[strlen(city) + 1];
                strcpy(curr->data.city, city);
            }
            return;
        }
        curr = curr->next;
    }

    auto newClub = new Club;
    newClub->data.name = new char[strlen(name) + 1];
    strcpy(newClub->data.name, name);
    newClub->data.country = country;

    if (!city) {
        newClub->data.city = nullptr;
    } else {
        newClub->data.city = new char[strlen(city) + 1];
        strcpy(newClub->data.city, city);
    }

    newClub->data.founded_year = founded_year;

    newClub->next = head;
    head = newClub;
}

// --- GETTERS & SETTERS ---
vector<Club*> ClubManager::getAllClubsCollection() const {
    vector<Club*> results;
    Club* curr = head;
    while (curr) {
        results.push_back(curr);
        curr = curr->next;
    }
    return results;
}

void ClubManager::setMatchManager(MatchManager* mgr) {
    this->matchManager = mgr;
}

// --- FILTERS ---
Club* ClubManager::findClubByName(const char* name) const {
    Club* curr = head;
    while (curr) {
        if (!strcmp(curr->data.name, name)) return curr;
        curr = curr->next;
    }
    return nullptr;
}

vector<Club*> ClubManager::filterClubs(const vector<Club*>& clubs, const function<bool(Club*)>& predicate) {
    vector<Club*> results;
    for (Club* club : clubs) {
        if (club && predicate(club)) results.push_back(club);
    }
    return results;
}

vector<Club*> ClubManager::findClubsByCountry(const Country country, const vector<Club*>& clubs) {
    return filterClubs(clubs, [country](Club* c) { return c->data.country == country; });
}

vector<Club*> ClubManager::findClubsByCity(const char* city, const vector<Club*>& clubs) {
    if (!city) return {};
    return filterClubs(clubs, [city](Club* c) { return strcmp(c->data.city, city) == 0; });
}

vector<Club*> ClubManager::findClubsByFoundedYear(const int founded_year, const vector<Club*>& clubs) {
    return filterClubs(clubs, [founded_year](Club* c) { return c->data.founded_year == founded_year; });
}

vector<Club*> ClubManager::findClubsByNewerFoundedYear(const int founded_year, const vector<Club*>& clubs) {
    return filterClubs(clubs, [founded_year](Club* c) { return c->data.founded_year > founded_year; });
}

vector<Club*> ClubManager::findClubsByOlderFoundedYear(const int founded_year, const vector<Club*>& clubs) {
    return filterClubs(clubs, [founded_year](Club* c) { return c->data.founded_year < founded_year; });
}

// --- DELETION ---
void ClubManager::clearClubMemory(Club* club) const {
    if (club) {
        if (this->matchManager) matchManager->removeClubFromMatchData(club);

        for (Stadium* s : club->data.stadiums) {
            if (s) s->ownedBy = nullptr;
        }

        for (Player* p : club->data.players) {
            if (p && p->person) p->person->hiredBy = nullptr;
        }

        for (Staff* st : club->data.staff) {
            if (st && st->person) st->person->hiredBy = nullptr;
        }

        delete[] club->data.name;
        delete[] club->data.city;

        club->data.stadiums.clear();
        club->data.players.clear();
        club->data.staff.clear();

        delete club;
    }
}

bool ClubManager::deleteClub(Club* club) {
    if (!club || !head) return false;

    if (club == head) {
        Club* temp = head;
        head = club->next;
        clearClubMemory(temp);
        return true;
    }

    Club* prev = head;
    while (prev->next && prev->next != club) {
        prev = prev->next;
    }
    if (prev->next == club) {
        prev->next = club->next;
        clearClubMemory(club);
        return true;
    }
    return false;
}

void ClubManager::deleteAllClubs() {
    while(head) {
        Club* next = head->next;
        clearClubMemory(head);
        head = next;
    }
    this->head = nullptr;
}

// --- RELATIONSHIP MANAGEMENT ---
void ClubManager::addStadiumToClub(Stadium* stadium, Club* club) {
    if (!stadium || !club) return;

    for (Stadium* s : club->data.stadiums) {
        if (s == stadium) return;
    }

    club->data.stadiums.push_back(stadium);
    stadium->ownedBy = club;
}

void ClubManager::addPlayerToClub(Player* player, Club* club) {
    if (!player || !club || !player->person) return;

    for (Player* p : club->data.players) {
        if (p->person && p->person->id == player->person->id) return;
    }

    club->data.players.push_back(player);
    player->person->hiredBy = club;
}

void ClubManager::addStaffToClub(Staff* staff, Club* club) {
    if (!staff || !club || !staff->person) return;

    for (Staff* s : club->data.staff) {
        if (s->person && s->person->id == staff->person->id) return;
    }

    club->data.staff.push_back(staff);
    staff->person->hiredBy = club;
}

bool ClubManager::removeStadiumFromClub(Stadium* stadium, Club* club) {
    if (!stadium || !club) return false;
    bool removed = false;

    for (auto it = club->data.stadiums.begin(); it != club->data.stadiums.end(); ) {
        if (*it == stadium) {
            it = club->data.stadiums.erase(it);
            stadium->ownedBy = nullptr;
            removed = true;
        } else {
            ++it;
        }
    }
    return removed;
}

bool ClubManager::removePersonFromClub(Person* person, Club* club) {
    if (!person || !club) return false;
    bool removed = false;

    for (auto it = club->data.players.begin(); it != club->data.players.end(); ) {
        if ((*it)->person && (*it)->person->id == person->id) {
            it = club->data.players.erase(it);
            person->hiredBy = nullptr;
            removed = true;
        } else {
            ++it;
        }
    }

    for (auto it = club->data.staff.begin(); it != club->data.staff.end(); ) {
        if ((*it)->person && (*it)->person->id == person->id) {
            it = club->data.staff.erase(it);
            person->hiredBy = nullptr;
            removed = true;
        } else {
            ++it;
        }
    }

    return removed;
}

// --- COUNTS ---
int ClubManager::getClubStadiumsCount(const Club* club) {
    return club ? club->data.stadiums.size() : 0;
}

int ClubManager::getClubPlayersCount(const Club* club) {
    return club ? club->data.players.size() : 0;
}

int ClubManager::getClubStaffCount(const Club* club) {
    return club ? club->data.staff.size() : 0;
}

// --- DISPLAY ---
void ClubManager::displayClub(const Club *club) {
    if (!club) return;

    cout << club->data.name << ": " << club->data.country << ", "
         << (club->data.city ? club->data.city : "Unknown")
         << ", founded in " << club->data.founded_year << endl;
    cout << "Number of stadiums: " << getClubStadiumsCount(club) <<  endl;
    cout << "Number of players: " << getClubPlayersCount(club) << endl;
    cout << "Number of staff: " << getClubStaffCount(club) << endl;
}

void ClubManager::displayClubList() const {
    Club* curr = head;
    cout << "--- Club List ---" << endl;
    while (curr) {
        displayClub(curr);
        cout << "-" << endl;
        curr = curr->next;
    }
    cout << "=================" << endl;
}