#ifndef FOOTBALL_MANAGEMENT_SYSTEM_PERSON_MANAGER_H
#define FOOTBALL_MANAGEMENT_SYSTEM_PERSON_MANAGER_H
#pragma once

#include "Person.h"
#include "../utils/Country.h"
#include "../match/MatchManager.h"
#include "../utils/IdGenerator.h"

class PersonManager {
    Person* head;
    IdGenerator idGen;
    MatchManager* matchManager;

    static void clearPersonMemory(Person* person);
    static Person* copyPerson(const Person* other);
public:
    explicit PersonManager(MatchManager* matchManager = nullptr);
    ~PersonManager();

    PersonManager(const PersonManager& other);
    PersonManager& operator=(const PersonManager& other);

    void person(const char* name, const char* surname, int age, Country nationality);

    static void updatePerson(Person* person, const char* name, const char* surname, int age, Country nationality);

    vector<Person*> getAllPeopleCollection() const;

    static vector<Person*> filterPeople(const std::vector<Person*>& people, const std::function<bool(Person*)>& predicate);
    Person* findPersonById(uint32_t id) const;
    static vector<Person*> findPeopleByName(const char* name, const char* surname, const std::vector<Person*>& people) ;
    static vector<Person*> findPeopleByAge(int age, const std::vector<Person*>& people) ;
    static vector<Person*> findPeopleYoungerThan(int age, const std::vector<Person*>& people);
    static vector<Person*> findPeopleOlderThan(int age, const std::vector<Person*>& people);
    static vector<Person*> findPeopleByNationality(Country nationality, const std::vector<Person*>& people);

    bool deletePerson(uint32_t personId);

    void deleteAllPeople();

    static void displayPerson(const Person* person);
    void displayPeopleList() const;

    void setMatchManager(MatchManager* mgr);
};


#endif