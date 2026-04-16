#ifndef CPP_CLASSES_PROJECT_PITCH_MANAGER_H
#define CPP_CLASSES_PROJECT_PITCH_MANAGER_H

#pragma once

#include "Stadium.h"

class StadiumManager
{
    Stadium* head;

    static void clearStadiumMemory(Stadium* stadium);
public:
    StadiumManager();
    ~StadiumManager();

    StadiumManager(const StadiumManager& other);
    StadiumManager& operator=(const StadiumManager& other);

    void stadium(const char* name, Country country, const char* city, int numberOfSeats);
    void stadium(const char* name, Country country, const char* city);

    StadiumListNode* getAllStadiumsWrapped() const;

    Stadium* findStadiumByName(const char* name) const;
    static Stadium* findStadiumByNameInWrapper(const char* name, StadiumListNode* head) ;

    static StadiumListNode* findStadiumsByCountry(Country country, StadiumListNode* head) ;
    static StadiumListNode* findStadiumsByCity(const char* city, StadiumListNode* head) ;
    static StadiumListNode* findStadiumsByMinSeats(int minSeats, StadiumListNode* head) ;
    static StadiumListNode* findStadiumsByMaxSeats(int maxSeats, StadiumListNode* head);

    bool deleteStadium(Stadium* stadium);
    static bool deleteWrappedStadium(StadiumListNode*& head, Stadium* target);

    void deleteAllStadiums();
    static void deleteAllWrappedStadiums(StadiumListNode*& head);

    static void displayStadium(const Stadium* head);
    void displayStadiumList() const;

    static void displayWrappedStadium(const StadiumListNode* wrapped_stadium);
    static void displayWrappedStadiumList(StadiumListNode* wrapped_stadium);
};

#endif