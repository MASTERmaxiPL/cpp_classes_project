#include "ClubManager.h"

#include <iostream>
#include <cstring>

#include "../person/PersonManager.h"
#include "../stadium/StadiumManager.h"
#include "../match/MatchManager.h"

using namespace std;

// --- CONSTRUCTORS & DESTRUCTORS ---
ClubManager::ClubManager(MatchManager* matchManager) : head(nullptr), matchManager(matchManager) {}

ClubManager::~ClubManager()
{
    deleteAllClubs();
}

ClubManager::ClubManager(const ClubManager& other)
{
    head = nullptr;
    matchManager = other.matchManager;
    *this = other;
}

ClubManager& ClubManager::operator=(const ClubManager& other)
{
    if (this == &other)
        return *this;

    deleteAllClubs();

    this->matchManager = other.matchManager;

    Club* curr = other.head;
    while (curr)
    {
        this->club(curr->data.name, curr->data.country, curr->data.city, curr->data.founded_year);
        curr = curr->next;
    }
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
    if (!name)
        return;

    Club* curr = head;
    while (curr)
    {
        if (strcmp(curr->data.name, name) == 0)
        {
            cout << "Club with name " << name << " already exists. Club data will be edited" << endl;
            curr->data.country = country;
            if (founded_year != -1)
                curr->data.founded_year = founded_year;
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

    auto newClub = new Club;

    newClub->data.name = new char[strlen(name) +1];
    strcpy(newClub->data.name, name);

    newClub->data.country = country;

    if (!city)
        newClub->data.city = nullptr;
    else
    {
        newClub->data.city = new char[strlen(name) +1];
        strcpy(newClub->data.city, city);
    }

    newClub->data.founded_year = founded_year;

    curr = head;
    head = newClub;
    head->next = curr;
}

// --- GETTERS ---
ClubListNode* ClubManager::getAllClubsWrapped() const
{
    ClubListNode* result = nullptr;

    Club* curr = head;

    while (curr) {
        result = new ClubListNode{curr, result};
        curr = curr->next;
    }

    return result;
}

// setter/getter dla matchManager
void ClubManager::setMatchManager(MatchManager* mgr)
{
    this->matchManager = mgr;
}

// --- FILTERS ---
Club* ClubManager::findClubByName(const char* name) const
{
    Club* curr = head;
    while (curr) {
        if (!strcmp(curr->data.name, name))
            return curr;
        curr = curr->next;
    }
    return nullptr;
}

Club* ClubManager::findClubByNameInWrapper(const char* name, ClubListNode* head) {
    if (!head)
        return nullptr;

    ClubListNode* curr = head;
    while (curr)
    {
        if (strcmp(curr->club->data.name, name) == 0)
            return curr->club;
        curr = curr->next;
    }
    return nullptr;
}

ClubListNode* ClubManager::findClubsByCountry(const Country country, ClubListNode* head) {
    if (!head)
        return nullptr;

    ClubListNode* resultHead = nullptr;
    ClubListNode* curr = head;

    while (curr)
    {
        if (country == curr->club->data.country)
        {
            resultHead = new ClubListNode{curr->club, resultHead};
        }
        curr = curr->next;
    }
    return resultHead;
}

ClubListNode* ClubManager::findClubsByCity(const char* city, ClubListNode* head) {
    if (!head)
        return nullptr;

    ClubListNode* resultHead = nullptr;
    ClubListNode* curr = head;

    while (curr)
    {
        if (strcmp(curr->club->data.city, city) == 0)
        {
            resultHead = new ClubListNode{curr->club, resultHead};
        }
        curr = curr->next;
    }
    return resultHead;
}

ClubListNode* ClubManager::findClubsByFoundedYear(const int founded_year, ClubListNode* head) {
    if (!head)
        return nullptr;

    ClubListNode* resultHead = nullptr;
    ClubListNode* curr = head;

    while (curr)
    {
        if (curr->club->data.founded_year == founded_year)
        {
            resultHead = new ClubListNode{curr->club, resultHead};
        }
        curr = curr->next;
    }
    return resultHead;
}

ClubListNode* ClubManager::findClubsByNewerFoundedYear(const int founded_year, ClubListNode* head) {
    if (!head)
        return nullptr;

    ClubListNode* resultHead = nullptr;
    ClubListNode* curr = head;

    while (curr)
    {
        if (curr->club->data.founded_year > founded_year)
        {
            resultHead = new ClubListNode{curr->club, resultHead};
        }
        curr = curr->next;
    }
    return resultHead;
}

ClubListNode* ClubManager::findClubsByOlderFoundedYear(const int founded_year, ClubListNode* head) {
    if (!head)
        return nullptr;

    ClubListNode* resultHead = nullptr;
    ClubListNode* curr = head;

    while (curr)
    {
        if (curr->club->data.founded_year < founded_year)
        {
            resultHead = new ClubListNode{curr->club, resultHead};
        }
        curr = curr->next;
    }
    return resultHead;
}

// --- DELETION ---
void ClubManager::deleteAllWrappedClubStadiums(StadiumListNode*& head)
{
    while (head)
    {
        StadiumListNode* next = head->next;
        delete head;
        head = next;
    }
}

void ClubManager::deleteAllWrappedClubPeople(PersonListNode*& head)
{
    while (head)
    {
        PersonListNode* next = head->next;
        delete head;
        head = next;
    }
}

void ClubManager::clearClubMemory(Club* club) const
{
    if (club)
    {
        if (this->matchManager)
            matchManager->removeClubFromMatchData(club);

        StadiumListNode* sCurr = club->data.stadium;
        while (sCurr)
        {
            if (sCurr->stadium)
                sCurr->stadium->ownedBy = nullptr;
            sCurr = sCurr->next;
        }

        PersonListNode* pCurr = club->data.playersHead;
        while (pCurr)
        {
            if (pCurr->person)
                pCurr->person->hiredBy = nullptr;
            pCurr = pCurr->next;
        }

        PersonListNode* stCurr = club->data.staffHead;
        while (stCurr)
        {
            if (stCurr->person)
                stCurr->person->hiredBy = nullptr;
            stCurr = stCurr->next;
        }

        delete[] club->data.name;
        delete[] club->data.city;

        deleteAllWrappedClubStadiums(club->data.stadium);
        deleteAllWrappedClubPeople(club->data.playersHead);
        deleteAllWrappedClubPeople(club->data.staffHead);

        delete club;
    }
}

bool ClubManager::deleteClub(Club* club) {
    if (!club) return false;

    if (club == head) {
        Club* temp = head;
        head = club->next;
        clearClubMemory(temp);
        return true;
    }

    Club* prev = head;
    while (prev && prev->next != club)
        prev = prev->next;
    if (prev) {
        prev->next = club->next;
        clearClubMemory(club);
        return true;
    }
    return false;
}

bool ClubManager::deleteWrappedClub(ClubListNode*& head, Club* target)
{
    if (!head || !target)
        return false;

    if (head->club == target)
    {
        ClubListNode* temp = head;
        head = head->next;
        delete temp;
        return true;
    }

    ClubListNode* prev = head;
    while (prev->next && prev->next->club != target)
    {
        prev = prev->next;
    }

    if (!prev->next)
        return false;

    ClubListNode* temp = prev->next;
    prev->next = temp->next;
    delete temp;

    return true;
}

void ClubManager::deleteAllClubs() {
    while(head) {
        Club* next = head->next;
        clearClubMemory(head);
        head = next;
    }
    this->head = nullptr;
}

void ClubManager::deleteAllWrappedList(ClubListNode*& head)
{
    while (head)
    {
        ClubListNode* next = head->next;
        delete head;
        head = next;
    }
}

void ClubManager::displayClub(const Club *club) {
    if (!club)
        return;

    cout << club->data.name << ": " << club->data.country << ", " << club->data.city <<
        ", founded in" << club->data.founded_year << endl;
    cout << "Number of stadiums:" << getClubPlayersCount(club) <<  endl;
    cout << "Number of players:" << getClubPlayersCount(club) << endl;
    cout << "Number of staff:" << getClubStaffCount(club) << endl;
}

void ClubManager::displayClubList() const
{
    Club* curr = head;

    cout << "Stadiums List" << endl;
    while (curr)
    {
        displayClub(curr);
        curr = curr->next;
    }
    cout << "===============" << endl;
}

void ClubManager::displayWrappedClub(const ClubListNode* wrapped_club)
{
    if (!wrapped_club)
        return;

    displayClub(wrapped_club->club);
}

void ClubManager::displayWrappedClubList(ClubListNode* wrapped_club)
{
    if (!wrapped_club)
        return;

    ClubListNode* curr = wrapped_club;

    cout << "Stadiums List" << endl;
    while (curr)
    {
        displayWrappedClub(curr);
        curr = curr->next;
    }
    cout << "===============" << endl;
}

void ClubManager::addStadiumToClub(Stadium* stadium, Club* club)
{
    if (!stadium || !club)
        return;

    StadiumListNode* temp = club->data.stadium;
    club->data.stadium = new StadiumListNode{stadium, temp};
    stadium->ownedBy = club;
}

void ClubManager::addPlayerToClub(Player* player, Club* club)
{
    if (!player || !club)
        return;

    PersonListNode* temp = club->data.playersHead;
    club->data.playersHead = new PersonListNode{player, temp};
    player->hiredBy = club;
}

void ClubManager::addStaffToClub(Staff* staff, Club* club)
{
    if (!staff || !club)
        return;

    PersonListNode* temp = club->data.staffHead;
    club->data.staffHead = new PersonListNode{staff, temp};
    staff->hiredBy = club;
}

bool ClubManager::removeStadiumFromClub(Stadium* stadium, Club* club)
{
    if (!stadium || !club)
        return false;

    if (StadiumManager::deleteWrappedStadium(club->data.stadium, stadium))
    {
        stadium->ownedBy = nullptr;
        return true;
    }
    return false;
}

bool ClubManager::removePersonFromClub(Person* person, Club* club)
{
    if (!person || !club)
        return false;

    if (club->data.playersHead)
    {
        if (PersonManager::deleteWrappedPerson(club->data.playersHead, person->id)){
            person->hiredBy = nullptr;
            return true;
        }
    }

    if (club->data.staffHead)
    {
        if (PersonManager::deleteWrappedPerson(club->data.staffHead, person->id))
        {
            person->hiredBy = nullptr;
            return true;
        }
    }
    return false;
}

int ClubManager::getClubStadiumsCount(const Club* club)
{
    if (!club)
        return 0;

    int count = 0;
    StadiumListNode* curr = club->data.stadium;
    while (curr)
    {
        count++;
        curr = curr->next;
    }
    return count;
}

int ClubManager::getClubPlayersCount(const Club* club)
{
    if (!club)
        return 0;

    int count = 0;
    PersonListNode* curr = club->data.playersHead;
    while (curr)
    {
        count++;
        curr = curr->next;
    }
    return count;
}

int ClubManager::getClubStaffCount(const Club* club)
{
    if (!club)
        return 0;

    int count = 0;
    PersonListNode* curr = club->data.staffHead;
    while (curr)
    {
        count++;
        curr = curr->next;
    }
    return count;
}
