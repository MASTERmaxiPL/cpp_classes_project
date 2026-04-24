#ifndef CPP_CLASSES_PROJECT_PITCH_MANAGER_H
#define CPP_CLASSES_PROJECT_PITCH_MANAGER_H

#pragma once

# include <vector>
# include <functional>

#include "Stadium.h"
#include "../match/MatchManager.h"

class StadiumManager
{
    Stadium* head;
    MatchManager* matchManager;

    void clearStadiumMemory(const Stadium* stadium) const;
public:
    explicit StadiumManager(MatchManager* matchManager = nullptr);
    ~StadiumManager();

    StadiumManager(const StadiumManager& other);
    StadiumManager& operator=(const StadiumManager& other);

    void stadium(const char* name, Country country, const char* city, int numberOfSeats);
    void stadium(const char* name, Country country, const char* city);

    vector<Stadium*> getAllStadiumsCollection() const;

    static vector<Stadium*> filterStadiums(const std::vector<Stadium*>& stadiums, std::function<bool(Stadium*)> predicate);

    static Stadium* findStadiumByName(const char* name, const vector<Stadium*>& stadiums);
    static vector<Stadium*> findStadiumsByCountry(Country country, const vector<Stadium*>& stadiums);
    static vector<Stadium*> findStadiumsByCity(const char* city, const vector<Stadium*>& stadiums);
    static vector<Stadium*> findStadiumsByMinSeats(int minSeats, const vector<Stadium*>& stadiums);
    static vector<Stadium*> findStadiumsByMaxSeats(int maxSeats, const vector<Stadium*>& stadiums);

    bool deleteStadium(Stadium* stadium);

    void deleteAllStadiums();

    static void displayStadium(const Stadium* head);
    void displayStadiumList() const;

    void setMatchManager(MatchManager* mgr);
};

#endif