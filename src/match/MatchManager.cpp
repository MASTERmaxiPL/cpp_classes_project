#include <iostream>
#include <cstring>
#include "MatchManager.h"

#include "../club/Club.h"


using namespace std;

// -- CONSTRUCTORS & DESTRUCTORS ---
MatchManager::MatchManager() : head(nullptr)
{
    idGen = IdGenerator();
}

MatchManager::~MatchManager()
{
    deleteAllMatches();
}

MatchManager::MatchManager(const MatchManager& other)
{
    head = nullptr;
    *this = other;
}

MatchManager& MatchManager::operator=(const MatchManager& other)
{
    if (this == &other)
        return *this;

    deleteAllMatches();

    Match* curr = other.head;
    while (curr)
    {
        this->match(curr->data.date, curr->data.home_club, curr->data.away_club, curr->data.stadium, curr->data.score_home_club, curr->data.score_away_club, curr->data.homeSquad, curr->data.awaySquad);
        curr = curr->next;
    }
    return *this;
}

// --- CREATION ---
void MatchManager::match(const tm& date, Club* home_club, Club* away_club, Stadium* stadium, const int score_home_club, const int score_away_club, MatchSquadEntry* homeSquad, MatchSquadEntry* awaySquad)
{
    if (home_club == nullptr || away_club == nullptr)
    {
        cerr << "Home club and away club cannot be null." << endl;
        return;
    }
    if (home_club == away_club)
    {
        cerr << "Home club and away club cannot be the same." << endl;
        return;
    }

    auto* newMatch = new Match;

    newMatch->id = idGen.generateId();
    newMatch->data.date = date;
    newMatch->data.home_club = home_club;
    newMatch->data.away_club = away_club;
    newMatch->data.stadium = stadium;
    newMatch->data.score_home_club = score_home_club;
    newMatch->data.score_away_club = score_away_club;
    newMatch->data.homeSquad = homeSquad;
    newMatch->data.awaySquad = awaySquad;

    Match* curr = head;
    head = newMatch;
    head->next = curr;
}

void MatchManager::match(const tm& date, Club* home_club, Club* away_club, Stadium* stadium, const int score_home_club, const int score_away_club)
{
    match(date, home_club, away_club, stadium, score_home_club, score_away_club, nullptr, nullptr);
}

void MatchManager::match(const tm& date, Club* home_club, Club* away_club, Stadium* stadium)
{
    match(date, home_club, away_club, stadium, -1, -1, nullptr, nullptr);
}

tm* MatchManager::createDate(const int day, const int month, const int year)
{
    const time_t now = time(nullptr);
    const tm current = *localtime(&now);
    const int currentYear = current.tm_year + 1900;

    if (year < 1980)
    {
        cerr << "Year must be 1980 or later." << endl;
        return nullptr;
    }
    if (year > currentYear + 10)
    {
        cerr << "Year must be " << (currentYear + 10) << " or earlier." << endl;
        return nullptr;
    }

    tm tmp{};
    tmp.tm_mday = day;
    tmp.tm_mon = month - 1; // tm_mon is 0-based
    tmp.tm_year = year - 1900; // tm_year is years since 1900
    tmp.tm_hour = 0;
    tmp.tm_min  = 0;
    tmp.tm_sec  = 0;
    tmp.tm_isdst = -1;

    time_t t = mktime(&tmp);
    if (t == -1 || tmp.tm_mon != month - 1 || tmp.tm_mday != day)
    {
        cerr << "Invalid date." << endl;
        return nullptr;
    }

    return new tm(tmp);
}

void MatchManager::addPlayerToSquad(Match* match, uint32_t playerId, Position pos, bool isHomeSquad)
{
    if (!match) return;

    // Create the node completely hidden inside the manager
    auto* newEntry = new MatchSquadEntry{playerId, pos, nullptr};

    if (isHomeSquad)
    {
        newEntry->next = match->data.homeSquad;
        match->data.homeSquad = newEntry;
    }
    else
    {
        newEntry->next = match->data.awaySquad;
        match->data.awaySquad = newEntry;
    }
}

// --- UPDATING ---
void MatchManager::updateMatch(Match* match, const tm& date, Club* home_club, Club* away_club, Stadium* stadium, const int score_home_club, const int score_away_club, MatchSquadEntry* homeSquad, MatchSquadEntry* awaySquad)
{
    if (!match)
        return;

    if (home_club == away_club)
        if (home_club)
            return;

    match->data.date = date;
    if (home_club)
        match->data.home_club = home_club;
    if (away_club && away_club != match->data.home_club)
        match->data.away_club = away_club;
    if (stadium)
        match->data.stadium = stadium;
    if (score_home_club != -1)
        match->data.score_home_club = score_home_club;
    if (score_away_club != -1)
        match->data.score_away_club = score_away_club;
    if (homeSquad)
        match->data.homeSquad = homeSquad;
    if (awaySquad)
        match->data.awaySquad = awaySquad;
}

// --- GETTERS ---
MatchListNode* MatchManager::getAllMatchesWrapped() const
{
    MatchListNode* result = nullptr;

    Match* curr = head;
    while (curr)
    {
        result = new MatchListNode{curr, result};
        curr = curr->next;
    }

    return result;
}

// --- FILTERS ---
Match* MatchManager::findMatchById(const uint32_t id, const MatchListNode* head)
{
    while (head)
    {
        if (head->match->id == id)
            return head->match;
        head = head->next;
    }
    return nullptr;
}

MatchListNode* MatchManager::findMatchesByDate(const tm& date, const int day_range, const int month_range, const int year_range, MatchListNode* head)
{
    if (!head)
    {
        return nullptr;
    }

    MatchListNode* result = nullptr;
    MatchListNode* curr = head;
    while (curr)
    {
        const tm& match_date = curr->match->data.date;
        if (abs(match_date.tm_mday - date.tm_mday) <= day_range &&
            abs(match_date.tm_mon - date.tm_mon) <= month_range &&
            abs(match_date.tm_year - date.tm_year) <= year_range)
        {
            result = new MatchListNode{curr->match, result};
        }
        curr = curr->next;
    }

    return result;
}

MatchListNode* MatchManager::findMatchesByClub(Club* club, MatchListNode* head)
{
    if (!head)
    {
        return nullptr;
    }

    MatchListNode* result = nullptr;
    MatchListNode* curr = head;
    while (curr)
    {
        if (curr->match->data.home_club == club || curr->match->data.away_club == club)
        {
            result = new MatchListNode{curr->match, result};
        }
        curr = curr->next;
    }

    return result;
}

MatchListNode* MatchManager::findMatchesByClubs(Club* club1, Club* club2, MatchListNode* head)
{
    if (!head)
    {
        return nullptr;
    }

    MatchListNode* result = nullptr;
    MatchListNode* curr = head;
    while (curr)
    {
        if ((curr->match->data.home_club == club1 && curr->match->data.away_club == club2) ||
            (curr->match->data.home_club == club2 && curr->match->data.away_club == club1))
        {
            result = new MatchListNode{curr->match, result};
        }
        curr = curr->next;
    }

    return result;
}

MatchListNode* MatchManager::findMatchesByStadium(Stadium* stadium, MatchListNode* head)
{
    if (!head)
    {
        return nullptr;
    }

    MatchListNode* result = nullptr;
    MatchListNode* curr = head;
    while (curr)
    {
        if (curr->match->data.stadium == stadium)
        {
            result = new MatchListNode{curr->match, result};
        }
        curr = curr->next;
    }

    return result;
}

MatchListNode* MatchManager::findUnplayedMatches(MatchListNode* head)
{
    if (!head)
    {
        return nullptr;
    }

    MatchListNode* result = nullptr;
    MatchListNode* curr = head;
    while (curr)
    {
        if (curr->match->data.score_home_club == -1 && curr->match->data.score_away_club == -1)
        {
            result = new MatchListNode{curr->match, result};
        }
        curr = curr->next;
    }

    return result;
}

// --- DELETION ---
void MatchManager::clearSquadMemory(MatchSquadEntry*& squadHead)
{
    while (squadHead)
    {
        MatchSquadEntry* next = squadHead->next;
        delete squadHead;
        squadHead = next;
    }
}

bool MatchManager::deleteMatch(Match* match)
{
    if (!head || !match)
        return false;

    if (head == match)
    {
        Match* temp = head;
        head = head->next;
        delete temp;
        return true;
    }

    Match* prev = head;
    while (prev->next && prev->next != match)
        prev = prev->next;

    if (prev->next)
    {
        Match* temp = prev->next;
        prev->next = prev->next->next;
        delete temp;
        return true;
    }

    return false;
}

bool MatchManager::deleteWrappedMatch(MatchListNode*& head, const uint32_t matchId)
{
    if (!head)
        return false;

    if (head->match->id == matchId)
    {
        MatchListNode* temp = head;
        head = head->next;
        clearSquadMemory(temp->match->data.homeSquad);
        clearSquadMemory(temp->match->data.awaySquad);
        delete temp;
        return true;
    }

    MatchListNode* prev = head;
    while (prev->next && prev->next->match->id != matchId)
        prev = prev->next;

    if (prev->next)
    {
        MatchListNode* temp = prev->next;
        prev->next = prev->next->next;
        clearSquadMemory(temp->match->data.homeSquad);
        clearSquadMemory(temp->match->data.awaySquad);
        delete temp;
        return true;
    }

    return false;
}

void MatchManager::deleteAllMatches()
{
    while (head)
    {
        Match* next = head->next;
        delete head;
        head = next;
    }
}

void MatchManager::deleteAllWrappedMatches(MatchListNode*& head)
{
    while (head)
    {
        MatchListNode* next = head->next;
        delete head;
        head = next;
    }
}

static void removeSquadEntriesByPlayerId(MatchSquadEntry*& head, uint32_t playerId)
{
    MatchSquadEntry** curPtr = &head;
    while (*curPtr)
    {
        if ((*curPtr)->player_id == playerId)
        {
            MatchSquadEntry* tmp = *curPtr;
            *curPtr = tmp->next;
            delete tmp;
        }
        else
        {
            curPtr = &((*curPtr)->next);
        }
    }
}

void MatchManager::removeClubFromMatchData(const Club* club) const
{
    if (!club) return;

    Match* curr = head;
    while (curr)
    {
        if (curr->data.home_club == club)
        {
            curr->data.home_club = nullptr;
        }
        else if (curr->data.away_club == club)
        {
            curr->data.away_club = nullptr;
        }
        curr = curr->next;
    }
}

void MatchManager::removeStadiumFromMatchData(const Stadium* stadium) const
{
    if (!stadium) return;

    Match* curr = head;
    while (curr)
    {
        if (curr->data.stadium == stadium)
        {
            curr->data.stadium = nullptr;
        }
        curr = curr->next;
    }
}

void MatchManager::removePersonFromMatchData(const uint32_t personId) const
{
    Match* curr = head;
    while (curr)
    {
        removeSquadEntriesByPlayerId(curr->data.homeSquad, personId);
        removeSquadEntriesByPlayerId(curr->data.awaySquad, personId);
        curr = curr->next;
    }
}

// --- DISPLAY ---
void MatchManager::displayMatch(const Match* match)
{
    if (!match)
        return;

    cout << "Match ID: " << match->id << endl;

    int day = match->data.date.tm_mday;
    int month = match->data.date.tm_mon + 1;
    int year = match->data.date.tm_year + 1900;
    cout << "Date: "
         << (day < 10 ? "0" : "") << day << "/"
         << (month < 10 ? "0" : "") << month << "/"
         << year << endl;

    cout << "Home Club: " << (match->data.home_club ? match->data.home_club->data.name : string("nullptr")) << endl;
    cout << "Away Club: " << (match->data.away_club ? match->data.away_club->data.name : string("nullptr")) << endl;
    cout << "Stadium: " << (match->data.stadium ? match->data.stadium->data.name : string("nullptr")) << endl;
    if (match->data.score_home_club != -1 && match->data.score_away_club != -1)
        cout << "Score: " << match->data.score_home_club << " - " << match->data.score_away_club << endl;
}

void MatchManager::displayMatchesList() const
{
    Match* curr = head;
    cout << "Matches List: " << endl;
    while (curr)
    {
        displayMatch(curr);
        cout << "-----------------------------" << endl;
        curr = curr->next;
    }
}

void MatchManager::displayWrappedMatch(const MatchListNode* wrapped_match)
{
    if (!wrapped_match)
        return;

    displayMatch(wrapped_match->match);
}

void MatchManager::displayWrappedMatchesList(MatchListNode* head)
{
    cout << "Matches List: " << endl;
    while (head)
    {
        displayMatch(head->match);
        cout << "-----------------------------" << endl;
        head = head->next;
    }
}