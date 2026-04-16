#ifndef FOOTBALL_MANAGEMENT_SYSTEM_MATCH_MANAGER_H
#define FOOTBALL_MANAGEMENT_SYSTEM_MATCH_MANAGER_H
#pragma once

#include "Match.h"
#include "utils/IdGenerator.h"


class MatchManager {
    Match* head;
    IdGenerator idGen;

public:
    MatchManager();
    ~MatchManager();

    MatchManager(const MatchManager& other);
    MatchManager& operator=(const MatchManager& other);

    void match(const tm& date, Club* home_club, Club* away_club, Stadium* stadium, int score_home_club, int score_away_club, MatchSquadEntry* homeSquad, MatchSquadEntry* awaySquad);
    void match(const tm& date, Club* home_club, Club* away_club, Stadium* stadium, int score_home_club, int score_away_club);
    void match(const tm& date, Club* home_club, Club* away_club, Stadium* stadium);

    static void updateMatch(Match* match, const tm& date, Club* home_club, Club* away_club, Stadium* stadium, int score_home_club, int score_away_club, MatchSquadEntry* homeSquad, MatchSquadEntry* awaySquad);

    static tm* createDate(int day, int month, int year) ;

    MatchListNode* getAllMatchesWrapped() const;

    static Match* findMatchById(uint32_t id, const MatchListNode* head) ;

    static MatchListNode* findMatchesByDate(const tm& date, int day_range, int month_range, int year_range, MatchListNode* head);
    static MatchListNode* findMatchesByClub(Club* club, MatchListNode* head);
    static MatchListNode* findMatchesByClubs(Club* club1, Club* club2, MatchListNode* head);
    static MatchListNode* findMatchesByStadium(Stadium* stadium, MatchListNode* head);
    static MatchListNode* findUnplayedMatches(MatchListNode* head);

    bool deleteMatch(Match* match);
    static bool deleteWrappedMatch(MatchListNode*& head, uint32_t matchId);

    void deleteAllMatches();
    static void deleteAllWrappedMatches(MatchListNode*& head);

    static void displayMatch(const Match* match);
    void displayMatchesList() const;

    static void displayWrappedMatch(const MatchListNode* wrapped_match);
    static void displayWrappedMatchesList(MatchListNode* head);
};


#endif