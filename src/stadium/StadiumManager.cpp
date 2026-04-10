#include <iostream>
#include "StadiumManager.h"

#include <cstring>

using namespace std;

// --- CONSTRUCTORS & DESTRUCTORS ---
StadiumManager::StadiumManager() : head(nullptr) {}

StadiumManager::~StadiumManager()
{
    deleteAllStadiums();
}

// --- CREATION ---
void StadiumManager::stadium(const char* name, const Country country, const char* city, const int numberOfSeats)
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

void StadiumManager::stadium(const char* name, const Country country, const char* city)
{
    stadium(name, country, city, 0);
}

// --- GETTERS ---
StadiumListNode* StadiumManager::getAllStadiumsWrapped() const
{
    StadiumListNode* result = nullptr;

    Stadium* curr = this->head;

    while (curr) {
        result = new StadiumListNode{curr, result};
        curr = curr->next;
    }

    return result;
}

// --- FILTERS ---
Stadium* StadiumManager::findStadiumByName(const char* name) const {
    Stadium* curr = head;
    while (curr)
    {
        if (strcmp(curr->data.name, name) == 0)
            return curr;
        curr = curr->next;
    }
    return nullptr;
}

Stadium* StadiumManager::findStadiumByNameInWrapper(const char* name, StadiumListNode* head) {
    if (!head)
        return nullptr;

    StadiumListNode* curr = head;
    while (curr)
    {
        if (strcmp(curr->stadium->data.name, name) == 0)
            return curr->stadium;
        curr = curr->next;
    }
    return nullptr;
}

StadiumListNode* StadiumManager::findStadiumsByCountry(const Country country, StadiumListNode* head)
{
    if (!head)
        return nullptr;

    StadiumListNode* resultHead = nullptr;
    StadiumListNode* curr = head;

    while (curr)
    {
        if (country == curr->stadium->data.country)
        {
            resultHead = new StadiumListNode{curr->stadium, resultHead};
        }
        curr = curr->next;
    }
    return resultHead;
}

StadiumListNode* StadiumManager::findStadiumsByCity(const char* city, StadiumListNode* head)
{
    if (!head)
        return nullptr;

    StadiumListNode* resultHead = nullptr;
    StadiumListNode* curr = head;

    while (curr)
    {
        if (strcmp(curr->stadium->data.city, city) == 0)
        {
            resultHead = new StadiumListNode{curr->stadium, resultHead};
        }
        curr = curr->next;
    }
    return resultHead;
}

StadiumListNode* StadiumManager::findStadiumsByMinSeats(const int minSeats, StadiumListNode* head)
{
    if (!head)
        return nullptr;

    StadiumListNode* resultHead = nullptr;
    StadiumListNode* curr = head;

    while (curr)
    {
        if (curr->stadium->data.numberOfSeats >= minSeats)
        {
            resultHead = new StadiumListNode{curr->stadium, resultHead};
        }
        curr = curr->next;
    }
    return resultHead;
}

StadiumListNode* StadiumManager::findStadiumsByMaxSeats(const int maxSeats, StadiumListNode* head) {
    if (!head)
        return nullptr;

    StadiumListNode* resultHead = nullptr;
    StadiumListNode* curr = head;

    while (curr)
    {
        if (curr->stadium->data.numberOfSeats <= maxSeats)
        {
            resultHead = new StadiumListNode{curr->stadium, resultHead};
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

bool StadiumManager::deleteWrappedStadium(StadiumListNode*& head, Stadium* target)
{
    if (!head || !target)
        return false;

    if (head->stadium == target)
    {
        StadiumListNode* temp = head;
        head = head->next;
        delete temp;
        return true;
    }

    StadiumListNode* prev = head;
    while (prev->next && prev->next->stadium != target)
    {
        prev = prev->next;
    }

    if (!prev->next)
        return false;

    StadiumListNode* temp = prev->next;
    prev->next = temp->next;
    delete temp;

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

void StadiumManager::deleteAllWrappedList(StadiumListNode*& head)
{
    while (head)
    {
        StadiumListNode* next = head->next;
        delete head;
        head = next;
    }
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

void StadiumManager::displayWrappedStadium(const StadiumListNode* wrapped_stadium)
{
    if (!wrapped_stadium)
        return;

    displayStadium(wrapped_stadium->stadium);
}

void StadiumManager::displayWrappedStadiumList(StadiumListNode* wrapped_stadium)
{
    if (!wrapped_stadium)
        return;

    StadiumListNode* curr = wrapped_stadium;

    cout << "Stadiums List" << endl;
    while (curr)
    {
        displayWrappedStadium(curr);
        curr = curr->next;
    }
    cout << "===============" << endl;
}