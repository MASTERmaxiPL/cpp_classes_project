#ifndef FOOTBALL_MANAGEMENT_SYSTEM_PLAYER_H
#define FOOTBALL_MANAGEMENT_SYSTEM_PLAYER_H
# pragma once

#include "../Person.h"
#include "Position.h"

struct Player {
    Person* person;
    Position position;
};

#endif