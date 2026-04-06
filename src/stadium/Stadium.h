#ifndef CPP_CLASSES_PROJECT_PITCH_H
#define CPP_CLASSES_PROJECT_PITCH_H
#pragma once

#include <string>

#include "../utils/Country.h"

using namespace std;

struct StadiumData
{
    string name;
    Country country;
    string city;
    int numberOfSeats;
};

struct Stadium{
    StadiumData data;
    Stadium* next{nullptr};
};

#endif