#ifndef FOOTBALL_MANAGEMENT_SYSTEM_CLUB_H
#define FOOTBALL_MANAGEMENT_SYSTEM_CLUB_H
# pragma once

#include <vector>

#include "../utils/Country.h"
#include "../person/staff/Staff.h"
#include "../stadium/Stadium.h"
#include "person/player/Player.h"

using namespace std;

struct ClubData {
    char* name{};
    Country country{};
    char* city{};
    int founded_year{};

    vector<Stadium*> stadiums;
    vector<Player*> players;
    vector<Staff*> staff;
};

struct Club {
    ClubData data;
    Club *next{nullptr};
};

#endif