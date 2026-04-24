#ifndef FOOTBALL_MANAGEMENT_SYSTEM_MATCH_MANAGER_H
#define FOOTBALL_MANAGEMENT_SYSTEM_MATCH_MANAGER_H
#pragma once

#include <functional>
#include <vector>

#include "Match.h"
#include "../utils/IdGenerator.h"


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

    static void addPlayerToSquad(Match* match, uint32_t playerId, Position pos, bool isHomeSquad);

    static void updateMatch(Match* match, const tm& date, Club* home_club, Club* away_club, Stadium* stadium, int score_home_club, int score_away_club, MatchSquadEntry* homeSquad, MatchSquadEntry* awaySquad);

    static tm* createDate(int day, int month, int year) ;

    vector<Match*> getAllMatchesCollection() const;

    static Match* findMatchById(uint32_t id, const vector<Match*>& matches) ;
    static vector<Match*> filterMatches(const vector<Match*>& matches, const function<bool(Match*)>& predicate);
    static vector<Match*> findMatchesByDate(const tm& date, int day_range, int month_range, int year_range, const vector<Match*>& matches);
    static vector<Match*> findMatchesByClub(Club* club, const vector<Match*>& matches);
    static vector<Match*> findMatchesByClubs(Club* club1, Club* club2, const vector<Match*>& matches);
    static vector<Match*> findMatchesByStadium(Stadium* stadium, const vector<Match*>& matches);
    static vector<Match*> findUnplayedMatches(const vector<Match*>& matches);

    static void clearSquadMemory(MatchSquadEntry*& squadHead);

    bool deleteMatch(Match* match);

    void deleteAllMatches();

    static void displayMatch(const Match* match);
    void displayMatchesList() const;

    void removeClubFromMatchData(const Club* club) const;
    void removeStadiumFromMatchData(const Stadium* stadium) const;
    void removePersonFromMatchData(uint32_t personId) const;
};


#endif