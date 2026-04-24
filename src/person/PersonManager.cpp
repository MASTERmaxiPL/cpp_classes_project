#include <iostream>
#include <cstring>
#include <vector>
#include <functional>

#include "PersonManager.h"
#include "../club/ClubManager.h"
#include "../match/MatchManager.h"

using namespace std;

// --- CONSTRUCTORS & DESTRUCTORS ---
PersonManager::PersonManager(MatchManager* matchManager)
    : head(nullptr), idGen(IdGenerator()), matchManager(matchManager) {}

PersonManager::PersonManager(const PersonManager& other)
    : head(nullptr), idGen(other.idGen), matchManager(other.matchManager) {
    *this = other;
}

Person* PersonManager::copyPerson(const Person* other) {
    if (!other) return nullptr;

    auto* newNode = new Person();
    newNode->id = other->id;
    newNode->data.age = other->data.age;
    newNode->data.nationality = other->data.nationality;
    newNode->hiredBy = other->hiredBy;
    newNode->next = nullptr;

    if (other->data.name) {
        newNode->data.name = new char[strlen(other->data.name) + 1];
        strcpy(newNode->data.name, other->data.name);
    } else {
        newNode->data.name = nullptr;
    }

    if (other->data.surname) {
        newNode->data.surname = new char[strlen(other->data.surname) + 1];
        strcpy(newNode->data.surname, other->data.surname);
    } else {
        newNode->data.surname = nullptr;
    }

    return newNode;
}

PersonManager& PersonManager::operator=(const PersonManager& other) {
    if (this == &other) return *this;

    deleteAllPeople();

    this->matchManager = other.matchManager;
    this->idGen = other.idGen;

    Person* currentOther = other.head;
    Person** target = &this->head;

    while (currentOther) {
        *target = copyPerson(currentOther);

        if (*target) {
            target = &((*target)->next);
        }

        currentOther = currentOther->next;
    }

    *target = nullptr;
    return *this;
}

PersonManager::~PersonManager() {
    deleteAllPeople();
}

// --- CREATION ---
void PersonManager::person(const char* name, const char* surname, const int age, const Country nationality) {
    if (!name || !surname) {
        cout << "Invalid input: name and surname cannot be null." << endl;
        return;
    }

    auto* newPerson = new Person();
    newPerson->id = idGen.generateId();

    newPerson->data.name = new char[strlen(name) + 1];
    strcpy(newPerson->data.name, name);

    newPerson->data.surname = new char[strlen(surname) + 1];
    strcpy(newPerson->data.surname, surname);

    newPerson->data.age = age;
    newPerson->data.nationality = nationality;
    newPerson->hiredBy = nullptr;

    newPerson->next = head;
    head = newPerson;
}

// -- UPDATING ---
void PersonManager::updatePerson(Person* person, const char* name, const char* surname, const int age, const Country nationality) {
    if (!person) return;

    if (name) {
        delete[] person->data.name;
        person->data.name = new char[strlen(name) + 1];
        strcpy(person->data.name, name);
    }
    if (surname) {
        delete[] person->data.surname;
        person->data.surname = new char[strlen(surname) + 1];
        strcpy(person->data.surname, surname);
    }
    person->data.age = age;
    person->data.nationality = nationality;
}

vector<Person*> PersonManager::getAllPeopleCollection() const {
    vector<Person*> results;
    Person* curr = head;
    while (curr) {
        results.push_back(curr);
        curr = curr->next;
    }
    return results;
}

// --- GENERIC FILTER ---
vector<Person*> PersonManager::filterPeople(const vector<Person*>& people, const function<bool(Person*)>& predicate) {
    vector<Person*> results;
    for (Person* p : people) {
        if (p && predicate(p)) {
            results.push_back(p);
        }
    }
    return results;
}

// --- SPECIFIC FILTERS ---
Person* PersonManager::findPersonById(const uint32_t id) const
{
    Person* curr = head;
    while (curr) {
        if (curr->id == id) return curr;
        curr = curr->next;
    }
    return nullptr;
}

vector<Person*> PersonManager::findPeopleByName(const char* name, const char* surname, const vector<Person*>& people) {
    if (!name || !surname) return {};
    return filterPeople(people, [name, surname](Person* p) {
        return strcmp(p->data.name, name) == 0 && strcmp(p->data.surname, surname) == 0;
    });
}

vector<Person*> PersonManager::findPeopleByAge(const int age, const vector<Person*>& people) {
    return filterPeople(people, [age](Person* p) {
        return p->data.age == age;
    });
}

vector<Person*> PersonManager::findPeopleYoungerThan(const int age, const vector<Person*>& people) {
    return filterPeople(people, [age](Person* p) {
            return p->data.age < age;
        });
}

vector<Person*> PersonManager::findPeopleOlderThan(const int age, const vector<Person*>& people) {
    return filterPeople(people, [age](Person* p) {
            return p->data.age > age;
        });
}

vector<Person*> PersonManager::findPeopleByNationality(const Country nationality, const vector<Person*>& people)
{
    return filterPeople(people, [nationality](Person* p) {
        return p->data.nationality == nationality;
    });
}

// --- DELETION ---
void PersonManager::clearPersonMemory(Person* person) {
    if (person) {
        delete[] person->data.name;
        delete[] person->data.surname;
        delete person;
    }
}

bool PersonManager::deletePerson(uint32_t personId) {
    if (!head) return false;

    if (head->id == personId) {
        Person* temp = head;
        head = head->next;

        if (temp->hiredBy) {
            ClubManager::removePersonFromClub(temp, temp->hiredBy);
        }
        if (this->matchManager) {
            this->matchManager->removePersonFromMatchData(temp->id);
        }

        clearPersonMemory(temp);
        return true;
    }

    Person* prev = head;
    while (prev->next && prev->next->id != personId) {
        prev = prev->next;
    }

    if (prev->next) {
        Person* temp = prev->next;
        prev->next = prev->next->next;

        if (temp->hiredBy) {
            ClubManager::removePersonFromClub(temp, temp->hiredBy);
        }
        if (this->matchManager) {
            this->matchManager->removePersonFromMatchData(temp->id);
        }

        clearPersonMemory(temp);
        return true;
    }

    return false;
}

void PersonManager::deleteAllPeople() {
    while (head) {
        Person* next = head->next;
        clearPersonMemory(head);
        head = next;
    }
    this->head = nullptr;
}

// --- DISPLAY ---
void PersonManager::displayPerson(const Person* person) {
    if (!person) return;
    cout << "ID: " << person->id << " | " << person->data.name << " " << person->data.surname
         << " | Age: " << person->data.age
         << " | Nationality: " << person->data.nationality << endl;
}

void PersonManager::displayPeopleList() const {
    cout << "--- All People ---" << endl;
    const Person* curr = head;
    while (curr) {
        displayPerson(curr);
        curr = curr->next;
    }
    cout << "==================" << endl;
}

void PersonManager::setMatchManager(MatchManager* mgr) {
    this->matchManager = mgr;
}