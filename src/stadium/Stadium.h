#ifndef CPP_CLASSES_PROJECT_PITCH_H
#define CPP_CLASSES_PROJECT_PITCH_H
#pragma once

#include "../utils/Country.h"

using namespace std;
struct Club;

struct StadiumData
{
    char* name;
    Country country;
    char* city;
    int numberOfSeats;
};

struct Stadium{
    StadiumData data{};
    Club* ownedBy{nullptr};
    Stadium* next{nullptr};
};

#endif