#ifndef CPP_CLASSES_PROJECT_PITCH_MANAGER_H
#define CPP_CLASSES_PROJECT_PITCH_MANAGER_H

#pragma once

#include <forward_list>

#include "Stadium.h"

class StadiumManager
{
    forward_list<Stadium> pitch_list{};

    public:
    StadiumManager();
    ~StadiumManager();

    void stadium(string name, Country country, string city, int numberOfSeats, string owning_club);
    void stadium(string name, Country country, string city, int numberOfSeats);
    void stadium(string name, Country country, string city, string owning_club);
    void stadium(string name, Country country, string city);

    Stadium* findStadiumByName(string name);
    forward_list<Stadium*> findStadiumsByCountry(Country country);
    forward_list<Stadium*> findStadiumsByCity(string city);
    forward_list<Stadium*> findStadiumsByMinSeats(int minSeats);
    forward_list<Stadium*> findStadiumsByMaxSeats(int maxSeats);
    forward_list<Stadium*> findStadiumsByClub(string owning_club);
};

#endif //CPP_CLASSES_PROJECT_PITCH_MANAGER_H