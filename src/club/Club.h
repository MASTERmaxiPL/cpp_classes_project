#ifndef FOOTBALL_MANAGEMENT_SYSTEM_CLUB_H
#define FOOTBALL_MANAGEMENT_SYSTEM_CLUB_H
# pragma once

#include <string>

#include "../utils/Country.h"
#include "../person/Player.h"
#include "../person/Staff.h"
#include "../stadium/Stadium.h"

using namespace std;

struct ClubStadium {
    Stadium* stadium{};
    ClubStadium* next{nullptr};
};

struct ClubPlayer {
    Player* player{nullptr};
    ClubPlayer* next{nullptr};
};

struct ClubStaff {
    Staff* staff{nullptr};
    ClubStaff* next{nullptr};
};

struct ClubData {
    string name;
    Country country;
    string city;
    int founded_year;

    ClubStadium* stadium{nullptr};
    ClubPlayer* playersHead{nullptr};
    ClubStaff* staffHead{nullptr};
};

struct Club {
    ClubData data;
    Club *next{nullptr};
};


#endif