#ifndef FOOTBALL_MANAGEMENT_SYSTEM_PERSON_MANAGER_H
#define FOOTBALL_MANAGEMENT_SYSTEM_PERSON_MANAGER_H
#pragma once

#include <unordered_map>

#include "Person.h"
#include "Player.h"
#include "Staff.h"
#include "../utils/Country.h"


class PersonManager {
    Person* head;
    unordered_map<uint32_t, Person*> peopleById;
public:
    PersonManager();
    ~PersonManager();

    Player* player(string name, string surname, int age, Country nationality, Position position);
    Staff* staff(string name, string surname, int age, Country nationality, Role role);

    Person* findPeopleByName(string name, string surname, Person* people);
    Person* findPeopleByAge(int age, Person* people);
    Person* findPeopleByNationality(Country nationality, Person* people);

    Person* findPeopleYoungerThan(int age, Person* people);
    Person* findPeopleOlderThan(int age, Person* people);

    void deletePerson(Person* person);

    void displayPerson(Person* person);
};


#endif