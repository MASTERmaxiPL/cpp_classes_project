#ifndef FOOTBALL_MANAGEMENT_SYSTEM_STAFF_MANAGER_H
#define FOOTBALL_MANAGEMENT_SYSTEM_STAFF_MANAGER_H
#pragma once

#include <vector>
#include <functional>
#include "Staff.h"

class StaffManager {
    vector<Staff*> staffList;

public:
    StaffManager() = default;
    ~StaffManager();

    Staff* addStaff(Person* person, Role role);
    static void updateStaffRole(Staff* staff, Role newRole);

    vector<Staff*> filterStaff(const function<bool(Staff*)>& predicate) const;
    Staff* findStaffByPersonId(uint32_t personId) const;
    vector<Staff*> findStaffByRole(Role role) const;

    bool removeStaffByPersonId(uint32_t personId);
    void removeAllStaff();

    static void displayStaffMember(const Staff* staff);
    void displayAllStaff() const;
};

#endif