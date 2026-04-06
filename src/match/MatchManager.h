#ifndef FOOTBALL_MANAGEMENT_SYSTEM_MATCH_MANAGER_H
#define FOOTBALL_MANAGEMENT_SYSTEM_MATCH_MANAGER_H
#include "Match.h"


class MatchManager {
    Match* head;

public:
    MatchManager();
    ~MatchManager();

    void match(tm date, Club* home_club, Club* away_club, int score_home_club, int score_away_club, Stadium stadium);
    void match(tm date, Club* home_club, Club* away_club, int score_home_club, int score_away_club);
    void match(tm date, Club* home_club, Club* away_club, Stadium stadium);
    void match(tm date, Club* home_club, Club* away_club);

    Match* findMatchesByDate(tm date, int day_range, int month_range, int year_range, Match* matches);
    Match* findMatchesByClub(Club club, Match* matches);
    Match* findMatchesByClubs(Club club1, Club club2, Match* matches);
    Match* findMatchesByStadium(Stadium stadium, Match* matches);
    Match* findUnplayedMatches(Match *matches);

    void deleteMatch(Match* match);

    void displayMatch(Match* match);
};


#endif