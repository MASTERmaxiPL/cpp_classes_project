#include <iostream>
#include "StaffManager.h"

using namespace std;

StaffManager::~StaffManager() {
    removeAllStaff();
}

Staff* StaffManager::addStaff(Person* person, const Role role) {
    if (!person) return nullptr;

    Staff* newStaff = new Staff{person, role};
    staffVector.push_back(newStaff);
    return newStaff;
}

void StaffManager::updateStaffRole(Staff* staff, const Role newRole) {
    if (staff) {
        staff->role = newRole;
    }
}

// --- FILTERS ---
vector<Staff*> StaffManager::filterStaff(const vector<Staff*>& staffVector, const std::function<bool(Staff*)>& predicate) {
    std::vector<Staff*> results;
    for (Staff* staff : staffVector) {
        if (staff && predicate(staff)) {
            results.push_back(staff);
        }
    }
    return results;
}

Staff* StaffManager::findStaffByPersonId(uint32_t personId) const {
    for (Staff* staff : staffVector) {
        if (staff && staff->person->id == personId) {
            return staff;
        }
    }
    return nullptr;
}

std::vector<Staff*> StaffManager::findStaffByRole(Role role, const vector<Staff*>& staffVector) {
    return filterStaff(staffVector, [role](Staff* s) {
        return s->role == role;
    });
}

// --- DELETION ---
bool StaffManager::removeStaffByPersonId(uint32_t personId) {
    for (auto it = staffVector.begin(); it != staffVector.end(); ++it) {
        if ((*it)->person->id == personId) {
            delete *it;
            staffVector.erase(it);
            return true;
        }
    }
    return false;
}

void StaffManager::removeAllStaff() {
    for (Staff* staff : staffVector) {
        delete staff;
    }
    staffVector.clear();
}

// --- DISPLAY ---
void StaffManager::displayStaffMember(const Staff* staff)
{
    if (!staff || !staff->person) return;
    
    cout << "ID: " << staff->person->id 
         << " | " << staff->person->data.name << " " << staff->person->data.surname 
         << " | Role: " << staff->role << endl;
}

void StaffManager::displayAllStaff() const {
    cout << "--- Staff Members ---" << endl;
    for (Staff* staff : staffVector) {
        displayStaffMember(staff);
    }
    cout << "=====================" << endl;
}