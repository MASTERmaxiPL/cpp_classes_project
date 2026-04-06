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

    void stadium(string name, Country country, string city, int numberOfSeats, string owning_club);
    void stadium(string name, Country country, string city, int numberOfSeats);
    void stadium(string name, Country country, string city, string owning_club);
    void stadium(string name, Country country, string city);

    Stadium* findStadiumByName(string name, Stadium* stadiums);
    Stadium* findStadiumsByCountry(Country country, Stadium* stadiums);
    Stadium* findStadiumsByCity(string city, Stadium* stadiums);
    Stadium* findStadiumsByMinSeats(int minSeats, Stadium* stadiums);
    Stadium* findStadiumsByMaxSeats(int maxSeats, Stadium* stadiums);
    Stadium* findStadiumsByClub(string owning_club, Stadium* stadiums);

    void deleteStadium(Stadium* stadium);

    void displayStadium(Stadium* stadium);
};

#endif