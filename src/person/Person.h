#ifndef FOOTBALL_MANAGEMENT_SYSTEM_PERSON_H
#define FOOTBALL_MANAGEMENT_SYSTEM_PERSON_H
#pragma once

#include <cstdint>
#include <string>

#include "../utils/Country.h"

using namespace std;

struct PersonData {
    char* name;
    char* surname;
    int age;
    Country nationality;
};

struct Person {
    uint32_t id{};
    PersonData data{};
    Person* next{nullptr};

    virtual ~Person() = default;
};

struct PersonListNode {
    Person* person;
    PersonListNode* next;
};

#endif