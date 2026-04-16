#ifndef FOOTBALL_MANAGEMENT_SYSTEM_PERSON_MANAGER_H
#define FOOTBALL_MANAGEMENT_SYSTEM_PERSON_MANAGER_H
#pragma once

#include "Person.h"
#include "Player.h"
#include "Staff.h"
#include "../utils/Country.h"
#include "match/MatchManager.h"
#include "utils/IdGenerator.h"

class PersonManager {
    Person* head;
    IdGenerator idGen;
    MatchManager* matchManager;

    static void clearPersonMemory(Person* person);
public:
    explicit PersonManager(MatchManager* matchManager = nullptr);
    ~PersonManager();

    PersonManager(const PersonManager& other);
    PersonManager& operator=(const PersonManager& other);

    void player(const char* name, const char* surname, int age, Country nationality, Position position);
    void staff(const char* name, const char* surname, int age, Country nationality, Role role);

    static void updatePerson(Person* person, const char* name, const char* surname, int age, Country nationality);
    static void updatePlayer(Player* player, const char* name, const char* surname, int age, Country nationality, Position position);
    static void updateStaff(Staff* staff, const char* name, const char* surname, int age, Country nationality, Role role);

    PersonListNode* getAllPeopleWrapped() const;

    static Person* findPersonById(uint32_t id, const PersonListNode* head);

    static PersonListNode* findPeopleByName(const char* name,const char* surname, const PersonListNode* head);
    static PersonListNode* findPeopleByAge(int age, const PersonListNode* head);
    static PersonListNode* findPeopleYoungerThan(int age, const PersonListNode* head);
    static PersonListNode* findPeopleOlderThan(int age, const PersonListNode* head);
    static PersonListNode* findPeopleByNationality(Country nationality, const PersonListNode* head);

    static PersonListNode* findPlayersByPosition(Position position, const PersonListNode* head);
    static PersonListNode* findStaffByRole(Role role, const PersonListNode* head);

    bool deletePerson(uint32_t personId);
    static bool deleteWrappedPerson(PersonListNode*& head, uint32_t personId);

    void deleteAllPeople();
    static void deleteAllWrappedPeople(PersonListNode*& head);

    static void displayPerson(const Person* person);
    void displayPeopleList() const;

    static void displayWrappedPerson(const PersonListNode* wrapped_person);
    static void displayWrappedPeopleList(PersonListNode* wrapped_person);

    void setMatchManager(MatchManager* mgr);
};


#endif