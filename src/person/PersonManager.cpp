#include <iostream>
#include <cstring>

#include "PersonManager.h"

using namespace std;

// --- CONSTRUCTORS & DESTRUCTORS ---
PersonManager::PersonManager() : head(nullptr)
{
    idGen = IdGenerator();
}

PersonManager::~PersonManager()
{
    deleteAllPeople();
}

PersonManager::PersonManager(const PersonManager& other)
{
    head = nullptr;
    *this = other;
}

PersonManager& PersonManager::operator=(const PersonManager& other)
{
    if (this == &other)
        return *this;

    deleteAllPeople();

    Person* curr = other.head;
    while (curr)
    {
        if (dynamic_cast<Player*>(curr))
        {
            const auto* player_person = dynamic_cast<Player*>(curr);
            this->player(player_person->data.name, player_person->data.surname, player_person->data.age, player_person->data.nationality, player_person->position);
        }
        else if (dynamic_cast<Staff*>(curr))
        {
            const auto* staff_person = dynamic_cast<Staff*>(curr);
            this->staff(staff_person->data.name, staff_person->data.surname, staff_person->data.age, staff_person->data.nationality, staff_person->role);
        }
        curr = curr->next;
    }
    return *this;
}

// --- CREATION ---
void PersonManager::player(const char* name, const char* surname, const int age, const Country nationality, const Position position)
{
    if (!name || !surname)
    {
        cout << "Invalid input: name and surname cannot be null." << endl;
        return;
    }

    auto* newPlayer = new Player;

    newPlayer->id = idGen.generateId();

    newPlayer->data.name = new char[strlen(name) + 1];
    strcpy(newPlayer->data.name, name);

    newPlayer->data.surname = new char[strlen(surname) + 1];
    strcpy(newPlayer->data.surname, surname);

    newPlayer->data.age = age;
    newPlayer->data.nationality = nationality;
    newPlayer->position = position;

    Person* curr = head;
    head = newPlayer;
    head->next = curr;
}

void PersonManager::staff(const char* name, const char* surname, const int age, const Country nationality, const Role role)
{
    if (!name || !surname)
    {
        cout << "Invalid input: name and surname cannot be null." << endl;
        return;
    }

    auto* newStaff = new Staff;

    newStaff->id = idGen.generateId();

    newStaff->data.name = new char[strlen(name) + 1];
    strcpy(newStaff->data.name, name);

    newStaff->data.surname = new char[strlen(surname) + 1];
    strcpy(newStaff->data.surname, surname);

    newStaff->data.age = age;
    newStaff->data.nationality = nationality;
    newStaff->role = role;

    Person* curr = head;
    head = newStaff;
    head->next = curr;
}

// -- UPDATING ---
void PersonManager::updatePerson(Person* person, const char* name, const char* surname, const int age, const Country nationality)
{
    if (!person)
        return;

    if (name)
    {
        delete[] person->data.name;
        person->data.name = new char[strlen(name) + 1];
        strcpy(person->data.name, name);
    }
    if (surname)
    {
        delete[] person->data.surname;
        person->data.surname = new char[strlen(surname) + 1];
        strcpy(person->data.surname, surname);
    }
    person->data.age = age;
    person->data.nationality = nationality;
}

void PersonManager::updatePlayer(Player* player, const char* name, const char* surname, const int age, const Country nationality, const Position position)
{
    if (!player)
        return;

    if (position)
        player->position = position;

    updatePerson(player, name, surname, age, nationality);
}

void PersonManager::updateStaff(Staff* staff, const char* name, const char* surname, const int age, const Country nationality, const Role role)
{
    if (!staff)
        return;

    if (role)
        staff->role = role;

    updatePerson(staff, name, surname, age, nationality);
}

// --- GETTERS ---
PersonListNode* PersonManager::getAllPeopleWrapped() const
{
    PersonListNode* result = nullptr;

    Person* curr = head;

    while (curr) {
        result = new PersonListNode{curr, result};
        curr = curr->next;
    }

    return result;
}

// --- FILTERS ---
Person* PersonManager::findPersonById(const uint32_t id, const PersonListNode* head)
{
    while (head)
    {
        if (head->person->id == id)
            return head->person;
        head = head->next;
    }
    return nullptr;
}

PersonListNode* PersonManager::findPeopleByName(const char* name, const char* surname, const PersonListNode* head)
{
    PersonListNode* result = nullptr;

    while (head)
    {
        if (strcmp(head->person->data.name, name) == 0 && strcmp(head->person->data.surname, surname) == 0)
        {
            result = new PersonListNode{head->person, result};
        }
        head = head->next;
    }

    return result;
}

PersonListNode* PersonManager::findPeopleByAge(const int age, const PersonListNode* head)
{
    PersonListNode* result = nullptr;

    while (head)
    {
        if (head->person->data.age == age)
        {
            result = new PersonListNode{head->person, result};
        }
        head = head->next;
    }

    return result;
}

PersonListNode* PersonManager::findPeopleYoungerThan(const int age, const PersonListNode* head)
{
    PersonListNode* result = nullptr;

    while (head)
    {
        if (head->person->data.age < age)
        {
            result = new PersonListNode{head->person, result};
        }
        head = head->next;
    }

    return result;
}

PersonListNode* PersonManager::findPeopleOlderThan(const int age, const PersonListNode* head)
{
    PersonListNode* result = nullptr;

    while (head)
    {
        if (head->person->data.age > age)
        {
            result = new PersonListNode{head->person, result};
        }
        head = head->next;
    }

    return result;
}

PersonListNode* PersonManager::findPeopleByNationality(const Country nationality, const PersonListNode* head)
{
    PersonListNode* result = nullptr;

    while (head)
    {
        if (head->person->data.nationality == nationality)
        {
            result = new PersonListNode{head->person, result};
        }
        head = head->next;
    }
    return result;
}

PersonListNode* PersonManager::findPlayersByPosition(const Position position, const PersonListNode* head)
{
    PersonListNode* result = nullptr;

    while (head)
    {
        if (auto* player = dynamic_cast<Player*>(head->person))
        {
            if (player->position == position)
            {
                result = new PersonListNode{player, result};
            }
        }
        head = head->next;
    }

    return result;
}

PersonListNode* PersonManager::findStaffByRole(const Role role, const PersonListNode* head)
{
    PersonListNode* result = nullptr;

    while (head)
    {
        if (auto* staff = dynamic_cast<Staff*>(head->person))
        {
            if (staff->role == role)
            {
                result = new PersonListNode{staff, result};
            }
        }
        head = head->next;
    }

    return result;
}

// --- DELETION ---
void PersonManager::clearPersonMemory(Person* person)
{
    if (person)
    {
        delete[] person->data.name;
        delete[] person->data.surname;
        delete person;
    }
}

bool PersonManager::deletePerson(uint32_t personId)
{
    if (!head)
        return false;

    if (head->id == personId)
    {
        Person* temp = head;
        head = head->next;
        clearPersonMemory(temp);
        return true;
    }

    Person* prev = head;
    while (prev->next && prev->next->id != personId)
        prev = prev->next;
    if (prev->next)
    {
        Person* temp = prev->next;
        prev->next = prev->next->next;
        clearPersonMemory(temp);
        return true;
    }

    return false;
}

bool PersonManager::deleteWrappedPerson(PersonListNode*& head, const uint32_t personId)
{
    if (!head)
        return false;

    if (head->person->id == personId)
    {
        PersonListNode* temp = head;
        head = head->next;
        delete temp;
        return true;
    }

    PersonListNode* prev = head;
    while (prev->next && prev->next->person->id != personId)
    {
        prev = prev->next;
    }

    if (!prev->next)
        return false;

    PersonListNode* temp = prev->next;
    prev->next = temp->next;
    delete temp;

    return true;
}

void PersonManager::deleteAllPeople()
{
    while(head)
    {
        Person* next = head->next;
        clearPersonMemory(head);
        head = next;
    }
    this->head = nullptr;
}

void PersonManager::deleteAllWrappedPeople(PersonListNode*& head)
{
    while (head)
    {
        PersonListNode* next = head->next;
        delete head;
        head = next;
    }
}

// --- DISPLAY ---
void PersonManager::displayPerson(const Person* person)
{
    if (!person)
        return;

    cout << person->data.name << " " << person->data.surname << "| " << person->data.age <<
        "| nationality: " << person->data.nationality;

    if (auto* player = dynamic_cast<const Player*>(person))
    {
        cout << "| position: " << player->position;
    }
    else if (auto* staff = dynamic_cast<const Staff*>(person))
    {
        cout << "| role: " << staff->role;
    }
}

void PersonManager::displayPeopleList() const
{
    Person* curr = head;

    cout << "Stadiums List" << endl;
    while (curr)
    {
        displayPerson(curr);
        curr = curr->next;
    }
    cout << "===============" << endl;
}

void PersonManager::displayWrappedPerson(const PersonListNode* wrapped_person)
{
    if (!wrapped_person)
        return;

    displayPerson(wrapped_person->person);
}

void PersonManager::displayWrappedPeopleList(PersonListNode* wrapped_person)
{
    if (!wrapped_person)
        return;

    PersonListNode* curr = wrapped_person;

    cout << "Stadiums List" << endl;
    while (curr)
    {
        displayWrappedPerson(curr);
        curr = curr->next;
    }
    cout << "===============" << endl;
}