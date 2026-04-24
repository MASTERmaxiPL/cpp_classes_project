#include <iostream>
#include "StaffManager.h"

using namespace std;

StaffManager::~StaffManager() {
    removeAllStaff();
}

Staff* StaffManager::addStaff(Person* person, const Role role) {
    if (!person) return nullptr;

    Staff* newStaff = new Staff{person, role};
    staffList.push_back(newStaff);
    return newStaff;
}

void StaffManager::updateStaffRole(Staff* staff, const Role newRole) {
    if (staff) {
        staff->role = newRole;
    }
}

// --- FILTERS ---
std::vector<Staff*> StaffManager::filterStaff(const std::function<bool(Staff*)>& predicate) const {
    std::vector<Staff*> results;
    for (Staff* staff : staffList) {
        if (staff && predicate(staff)) {
            results.push_back(staff);
        }
    }
    return results;
}

Staff* StaffManager::findStaffByPersonId(uint32_t personId) const {
    for (Staff* staff : staffList) {
        if (staff && staff->person->id == personId) {
            return staff;
        }
    }
    return nullptr;
}

std::vector<Staff*> StaffManager::findStaffByRole(Role role) const {
    return filterStaff([role](Staff* s) {
        return s->role == role;
    });
}

// --- DELETION ---
bool StaffManager::removeStaffByPersonId(uint32_t personId) {
    for (auto it = staffList.begin(); it != staffList.end(); ++it) {
        if ((*it)->person->id == personId) {
            delete *it;
            staffList.erase(it);
            return true;
        }
    }
    return false;
}

void StaffManager::removeAllStaff() {
    for (Staff* staff : staffList) {
        delete staff;
    }
    staffList.clear();
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
    for (Staff* staff : staffList) {
        displayStaffMember(staff);
    }
    cout << "=====================" << endl;
}