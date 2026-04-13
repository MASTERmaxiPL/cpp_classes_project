#ifndef FOOTBALL_MANAGEMENT_SYSTEM_CLUB_H
#define FOOTBALL_MANAGEMENT_SYSTEM_CLUB_H
# pragma once

#include "../utils/Country.h"
#include "../person/Staff.h"
#include "../stadium/Stadium.h"

using namespace std;

struct ClubData {
    char* name{};
    Country country{};
    char* city{};
    int founded_year{};

    StadiumListNode* stadium{nullptr};
    PersonListNode* playersHead{nullptr};
    PersonListNode* staffHead{nullptr};
};

struct Club {
    ClubData data;
    Club *next{nullptr};
};

struct ClubListNode {
    Club* club{};
    ClubListNode* next{nullptr};
};


#endif