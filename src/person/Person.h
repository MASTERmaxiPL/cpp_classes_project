#ifndef FOOTBALL_MANAGEMENT_SYSTEM_PERSON_H
#define FOOTBALL_MANAGEMENT_SYSTEM_PERSON_H
#pragma once

#include <cstdint>

#include "../utils/Country.h"

using namespace std;
struct Club;

struct PersonData {
    char* name;
    char* surname;
    int age;
    Country nationality;
};

struct Person {
    uint32_t id{};
    PersonData data{};
    Club* hiredBy{nullptr};
    Person* next{nullptr};
};

#endif