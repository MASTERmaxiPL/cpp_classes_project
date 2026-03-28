#ifndef CPP_CLASSES_PROJECT_PITCH_MANAGER_H
#define CPP_CLASSES_PROJECT_PITCH_MANAGER_H

#pragma once

#include <forward_list>

#include "Pitch.h"

class PitchManager
{
    forward_list<Pitch> pitch_list{};

    public:
    PitchManager();
    ~PitchManager();

    void pitch(string name, Country country, string city, int numberOfSeats, string owning_club);
    void pitch(string name, Country country, string city, int numberOfSeats);
    void pitch(string name, Country country, string city, string owning_club);
    void pitch(string name, Country country, string city);

    Pitch* findPitchByName(string name);
    forward_list<Pitch*> findPitchesByCountry(Country country);
    forward_list<Pitch*> findPitchesByCity(string city);
    forward_list<Pitch*> findPitchesByMinSeats(int minSeats);
    forward_list<Pitch*> findPitchesByMaxSeats(int maxSeats);
    forward_list<Pitch*> findPitchesByClub(string owning_club);
};

#endif //CPP_CLASSES_PROJECT_PITCH_MANAGER_H