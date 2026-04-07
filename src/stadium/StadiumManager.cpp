#include <iostream>
#include "StadiumManager.h"

#include <cstring>

using namespace std;

// --- CONSTRUCTORS & DESTRUCTORS ---
StadiumManager::StadiumManager() : head(nullptr) {}

StadiumManager::~StadiumManager()
{
    deleteAllStadiums(head);
}

// --- CREATION ---
void StadiumManager::stadium(char* name, Country country, char* city, int numberOfSeats)
{
    auto* newStadium = new Stadium;

    newStadium->data.name = new char[strlen(name) + 1];
    strcpy(newStadium->data.name, name);

    newStadium->data.country = country;

    newStadium->data.city = new char[strlen(city) + 1];
    strcpy(newStadium->data.city, city);

    newStadium->data.numberOfSeats = numberOfSeats;

    Stadium* curr = head;
    head = newStadium;
    head->next = curr;
}

void StadiumManager::stadium(char* name, Country country, char* city)
{
    stadium(name, country, city, 0);
}

// --- FILTERS ---

Stadium* StadiumManager::findStadiumByName(char* name, Stadium* head)
{
    if (!head)
        head = this->head;

    Stadium* curr = head;
    while (curr)
    {
        if (strcmp(curr->data.name, name) == 0)
            return curr;
        curr = curr->next;
    }
    return nullptr;
}

StadiumListNode* StadiumManager::findStadiumsByCountry(Country country, Stadium* head)
{
    if (!head)
        head = this->head;

    StadiumListNode* resultHead = nullptr;
    Stadium* curr = head;

    while (curr)
    {
        if (country == curr->data.country)
        {
            resultHead = new StadiumListNode{curr, resultHead};
        }
        curr = curr->next;
    }
    return resultHead;
}

StadiumListNode* StadiumManager::findStadiumsByCity(char* city, Stadium* head)
{
    if (!head)
        head = this->head;

    StadiumListNode* resultHead = nullptr;
    Stadium* curr = head;

    while (curr)
    {
        if (strcmp(curr->data.city, city) == 0)
        {
            resultHead = new StadiumListNode{curr, resultHead};
        }
        curr = curr->next;
    }
    return resultHead;
}

StadiumListNode* StadiumManager::findStadiumsByMinSeats(int minSeats, Stadium* head)
{
    if (!head)
        head = this->head;

    StadiumListNode* resultHead = nullptr;
    Stadium* curr = head;

    while (curr)
    {
        if (curr->data.numberOfSeats >= minSeats)
        {
            resultHead = new StadiumListNode{curr, resultHead};
        }
        curr = curr->next;
    }
    return resultHead;
}

StadiumListNode* StadiumManager::findStadiumsByMaxSeats(int maxSeats, Stadium* head)
{
    if (!head)
        head = this->head;

    StadiumListNode* resultHead = nullptr;
    Stadium* curr = head;

    while (curr)
    {
        if (curr->data.numberOfSeats <= maxSeats)
        {
            resultHead = new StadiumListNode{curr, resultHead};
        }
        curr = curr->next;
    }
    return resultHead;
}

// --- DELETION ---
void StadiumManager::clearStadiumMemory(Stadium* stadium)
{
    if (stadium)
    {
        delete[] stadium->data.name;
        delete[] stadium->data.city;
        delete stadium;
    }
}

bool StadiumManager::deleteStadium(Stadium* stadium)
{
    if (!stadium) return false;

    if (stadium == head)
    {
        Stadium* temp = head;
        head = stadium->next;
        clearStadiumMemory(temp);
        return true;
    }

    Stadium* prev = head;
    while (prev && prev->next != stadium)
        prev = prev->next;
    if (prev)
    {
        prev->next = stadium->next;
        clearStadiumMemory(stadium);
        return true;
    }

    return false;
}


void StadiumManager::deleteAllStadiums(Stadium* head = nullptr)
{
    if (!head)
        head = this->head;
    while(head)
    {
        Stadium* next = head->next;
        clearStadiumMemory(head);
        head = next;
    }
    this->head = nullptr;
}

void StadiumManager::deleteAllWrapperList(StadiumListNode*& head)
{
    while (head)
    {
        StadiumListNode* next = head->next;
        delete head;
        head = next;
    }
}

// --- DISPLAY ---
void StadiumManager::displayStadium(Stadium* head) const
{
    if (!head)
        head = this->head;

    Stadium* curr = head;

    cout << "Stadiums List" << endl;
    while (curr)
    {
        cout << curr->data.name << ": " << curr->data.country << ", " << curr->data.city << "; number of seats" << curr->data.numberOfSeats << endl;
        curr = curr->next;
    }
    cout << "===============" << endl;
}
