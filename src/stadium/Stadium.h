#ifndef CPP_CLASSES_PROJECT_PITCH_H
#define CPP_CLASSES_PROJECT_PITCH_H
#pragma once

#include <string>

#include "../utils/Country.h"

using namespace std;

struct StadiumData
{
    char* name;
    Country country;
    char* city;
    int numberOfSeats;
};

struct Stadium{
    StadiumData data{};
    Stadium* next{nullptr};
};

struct StadiumListNode {
    Stadium* stadium{};
    StadiumListNode* next{nullptr};
};

#endif