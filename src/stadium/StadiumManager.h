#ifndef CPP_CLASSES_PROJECT_PITCH_MANAGER_H
#define CPP_CLASSES_PROJECT_PITCH_MANAGER_H

#pragma once

#include "Stadium.h"

class StadiumManager
{
    Stadium* head;
public:
    StadiumManager();
    ~StadiumManager();

    void stadium(string name, Country country, string city, int numberOfSeats);
    void stadium(string name, Country country, string city);

    Stadium* findStadiumByName(string name, Stadium* stadiums);
    StadiumListNode* findStadiumsByCountry(Country country, Stadium* stadiums);
    StadiumListNode* findStadiumsByCity(string city, Stadium* stadiums);
    StadiumListNode* findStadiumsByMinSeats(int minSeats, Stadium* stadiums);
    StadiumListNode* findStadiumsByMaxSeats(int maxSeats, Stadium* stadiums);

    bool deleteStadium(Stadium* stadium);

    void displayStadium(Stadium* stadium);
};

#endif