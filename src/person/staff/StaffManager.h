#ifndef FOOTBALL_MANAGEMENT_SYSTEM_STAFF_MANAGER_H
#define FOOTBALL_MANAGEMENT_SYSTEM_STAFF_MANAGER_H
#pragma once

#include <vector>
#include <functional>
#include "Staff.h"

class StaffManager {
public:
    vector<Staff*> staffVector;

    StaffManager() = default;
    ~StaffManager();

    Staff* addStaff(Person* person, Role role);
    static void updateStaffRole(Staff* staff, Role newRole);

    static vector<Staff*> filterStaff(const vector<Staff*>& staffVector, const function<bool(Staff*)>& predicate);
    Staff* findStaffByPersonId(uint32_t personId) const;

    static vector<Staff*> findStaffByRole(Role role, const vector<Staff*>& staffVector);

    bool removeStaffByPersonId(uint32_t personId);
    void removeAllStaff();

    static void displayStaffMember(const Staff* staff);
    void displayAllStaff() const;
};

#endif