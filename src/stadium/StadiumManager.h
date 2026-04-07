#ifndef CPP_CLASSES_PROJECT_PITCH_MANAGER_H
#define CPP_CLASSES_PROJECT_PITCH_MANAGER_H

#pragma once

#include "Stadium.h"

class StadiumManager
{
    Stadium* head;
    void clearStadiumMemory(Stadium* stadium);
public:
    StadiumManager();
    ~StadiumManager();

    void stadium(char* name, Country country, char* city, int numberOfSeats);
    void stadium(char* name, Country country, char* city);

    Stadium* findStadiumByName(char* name, Stadium* head);
    StadiumListNode* findStadiumsByCountry(Country country, Stadium* head);
    StadiumListNode* findStadiumsByCity(char* city, Stadium* head);
    StadiumListNode* findStadiumsByMinSeats(int minSeats, Stadium* head);
    StadiumListNode* findStadiumsByMaxSeats(int maxSeats, Stadium* head);

    bool deleteStadium(Stadium* stadium);
    void deleteAllStadiums(Stadium* head);
    static void deleteAllWrapperList(StadiumListNode*& head);

    void displayStadium(Stadium* head) const;
};

#endif