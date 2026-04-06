#ifndef FOOTBALL_MANAGEMENT_SYSTEM_PERSON_H
#define FOOTBALL_MANAGEMENT_SYSTEM_PERSON_H
#pragma once

#include <cstdint>
#include <string>

#include "../utils/Country.h"

using namespace std;

struct PersonData {
    string name;
    string surname;
    int age;
    Country nationality;
};

struct Person {
    uint32_t id{};
    PersonData data;
    Person* next{nullptr};

    virtual ~Person() = default;
};

#endif