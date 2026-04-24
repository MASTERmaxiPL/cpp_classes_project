#include <iostream>
#include "StadiumManager.h"

#include <cstring>
#include "club/ClubManager.h"
#include "../match/MatchManager.h"

using namespace std;

// --- CONSTRUCTORS & DESTRUCTORS ---
StadiumManager::StadiumManager(MatchManager* matchManager) : head(nullptr), matchManager(matchManager) {}

StadiumManager::~StadiumManager()
{
    deleteAllStadiums();
}

StadiumManager::StadiumManager(const StadiumManager& other)
{
    head = nullptr;
    matchManager = other.matchManager;
    *this = other;
}

StadiumManager& StadiumManager::operator=(const StadiumManager& other)
{
    if (this == &other)
        return *this;

    deleteAllStadiums();

    this->matchManager = other.matchManager;

    Stadium* curr = other.head;
    while (curr)
    {
        this->stadium(curr->data.name, curr->data.country, curr->data.city, curr->data.numberOfSeats);
        curr = curr->next;
    }
    return *this;
}

// --- CREATION ---
void StadiumManager::stadium(const char* name, const Country country, const char* city, const int numberOfSeats)
{
    if (!name)
        return;

    Stadium* curr = head;
    while (curr)
    {
        if (strcmp(curr->data.name, name) == 0)
        {
            cout << "Stadium with name " << name << " already exists. Stadium data will be edited" << endl;
            curr->data.country = country;
            if (numberOfSeats != -1)
                curr->data.numberOfSeats = numberOfSeats;
            if (city)
            {
                delete [] curr->data.city;
                curr->data.city = new char[strlen(city) +1];
                strcpy(curr->data.city, city);
            }
            return;
        }
        curr = curr->next;
    }

    auto* newStadium = new Stadium;

    newStadium->data.name = new char[strlen(name) + 1];
    strcpy(newStadium->data.name, name);

    newStadium->data.country = country;

    newStadium->data.city = new char[strlen(city) + 1];
    strcpy(newStadium->data.city, city);

    newStadium->data.numberOfSeats = numberOfSeats;

    curr = head;
    head = newStadium;
    head->next = curr;
}

void StadiumManager::stadium(const char* name, const Country country, const char* city)
{
    stadium(name, country, city, -1);
}

// --- GETTERS ---
vector<Stadium*> StadiumManager::getAllStadiumsCollection() const
{
    vector<Stadium*> results;
    Stadium* curr = head;

    while (curr)
    {
        results.push_back(curr);
        curr = curr->next;
    }

    return results;
}

// -- SETTERS ---
void StadiumManager::setMatchManager(MatchManager* mgr)
{
    this->matchManager = mgr;
}

// --- FILTERS ---
vector<Stadium*> StadiumManager::filterStadiums(const vector<Stadium*>& stadiums, function<bool(Stadium*)> predicate)
{
    vector<Stadium*> results;
    for (Stadium* stadium : stadiums) {
        if (stadium && predicate(stadium)) {
            results.push_back(stadium);
        }
    }
    return results;
}

Stadium* StadiumManager::findStadiumByName(const char* name, const vector<Stadium*>& stadiums) {
    if (!name) return nullptr;

    for (Stadium* stadium : stadiums) {
        if (stadium && strcmp(stadium->data.name, name) == 0) {
            return stadium;
        }
    }
    return nullptr;
}

vector<Stadium*> StadiumManager::findStadiumsByCountry(const Country country, const vector<Stadium*>& stadiums) {
    return filterStadiums(stadiums, [country](Stadium* s) {
        return s->data.country == country;
    });
}

vector<Stadium*> StadiumManager::findStadiumsByCity(const char* city, const vector<Stadium*>& stadiums) {
    if (!city) return {};
    return filterStadiums(stadiums, [city](Stadium* s) {
        return strcmp(s->data.city, city) == 0;
    });
}

vector<Stadium*> StadiumManager::findStadiumsByMinSeats(const int minSeats, const vector<Stadium*>& stadiums) {
    return filterStadiums(stadiums, [minSeats](Stadium* s) {
        return s->data.numberOfSeats >= minSeats;
    });
}

vector<Stadium*> StadiumManager::findStadiumsByMaxSeats(const int maxSeats, const vector<Stadium*>& stadiums) {
    return filterStadiums(stadiums, [maxSeats](Stadium* s) {
        return s->data.numberOfSeats <= maxSeats;
    });
}

// --- DELETION ---
void StadiumManager::clearStadiumMemory(const Stadium* stadium) const
{
    if (stadium)
    {
        if (this->matchManager)
            this->matchManager->removeStadiumFromMatchData(stadium);

        delete[] stadium->data.name;
        delete[] stadium->data.city;
        delete stadium;
    }
}

bool StadiumManager::deleteStadium(Stadium* stadium) {
    if (!stadium) return false;

    if (stadium->ownedBy) {
        auto& clubStadiums = stadium->ownedBy->data.stadiums;
        for (auto it = clubStadiums.begin(); it != clubStadiums.end(); ++it) {
            if (*it == stadium) {
                clubStadiums.erase(it);
                break;
            }
        }
    }

    if (this->matchManager) {
        this->matchManager->removeStadiumFromMatchData(stadium);
    }

    if (head == stadium) {
        head = head->next;
    } else {
        Stadium* prev = head;
        while (prev && prev->next != stadium) prev = prev->next;
        if (prev) prev->next = stadium->next;
    }

    clearStadiumMemory(stadium);
    return true;
}

void StadiumManager::deleteAllStadiums()
{
    while(head)
    {
        Stadium* next = head->next;
        clearStadiumMemory(head);
        head = next;
    }
    this->head = nullptr;
}

// --- DISPLAY ---
void StadiumManager::displayStadium(const Stadium* stadium)
{
    if (!stadium)
        return;

    cout << stadium->data.name << ": " << stadium->data.country << ", " << stadium->data.city <<
        "; number of seats" << stadium->data.numberOfSeats << endl;
}

void StadiumManager::displayStadiumList() const
{
    Stadium* curr = head;

    cout << "Stadiums List" << endl;
    while (curr)
    {
        displayStadium(curr);
        curr = curr->next;
    }
    cout << "===============" << endl;
}