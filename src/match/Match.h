#ifndef FOOTBALL_MANAGEMENT_SYSTEM_MATCH_H
#define FOOTBALL_MANAGEMENT_SYSTEM_MATCH_H
#include <ctime>

#include "club/Club.h"
#include "stadium/Stadium.h"

using namespace std;

struct MatchData {
    tm date;
    Club* club_1;
    Club* club_2;
    int score_club_1;
    int score_club_2;
    Stadium* stadium;
};

struct Match {
    MatchData data;
    Match *next{nullptr};
};

#endif