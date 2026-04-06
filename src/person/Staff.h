#ifndef FOOTBALL_MANAGEMENT_SYSTEM_STAFF_H
#define FOOTBALL_MANAGEMENT_SYSTEM_STAFF_H
#pragma once

#include "Person.h"
#include "Role.h"

struct Staff : Person
{
    Role role;
};


#endif