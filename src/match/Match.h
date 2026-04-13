#ifndef FOOTBALL_MANAGEMENT_SYSTEM_MATCH_H
#define FOOTBALL_MANAGEMENT_SYSTEM_MATCH_H
#include <ctime>

#include "club/Club.h"
#include "person/Position.h"
#include "stadium/Stadium.h"

using namespace std;

struct MatchSquadEntry {
    uint32_t player_id;
    Position player_position;
    MatchSquadEntry* next{nullptr};
};

struct MatchSquad {
    MatchSquadEntry* head{nullptr};
};

struct MatchData {
    tm date;
    Stadium* stadium;
    Club* home_club;
    Club* away_club;
    int score_home_club;
    int score_away_club;
    MatchSquad* homeSquad;
    MatchSquad* awaySquad;
};

struct Match {
    MatchData data{};
    Match *next{nullptr};
};

#endif