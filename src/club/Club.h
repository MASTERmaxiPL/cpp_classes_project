#ifndef FOOTBALL_MANAGEMENT_SYSTEM_CLUB_H
#define FOOTBALL_MANAGEMENT_SYSTEM_CLUB_H
# pragma once

#include <string>

#include "../utils/Country.h"

using namespace std;

struct ClubData {
    string name;
    Country country;
    string city;
    int founded_year;
};

struct Club {
    ClubData data;
    Club *next{nullptr};
};


#endif