# C++ Project - Football Management System

### The Football Management System is a C++ application designed to model the ecosystem of football clubs, stadiums, personnel, and match events.
### The system is split into pure data models (Entities) and dedicated functional controllers (Managers)

## Core Entities & Relations:
- **Person**: The core identity entity. It contains basic data like name, age, and nationality. To prevent memory leaks, `PersonManager` is the sole owner of these objects.
- **Player & Staff (Composition)**: `Player` and `Staff` entities have a pointer to a `Person`. `Player` adds tactical `Position` data, while `Staff` adds organizational `Role` data. This allows a single `Person` to hold multiple roles (e.g., a Player-Coach).
- **Stadium**: Represents venues, linked to a specific `Club` via an `ownedBy` pointer.
- **Club**: The central organizational entity. It maintains `std::vector` collections of its hired `Player`, `Staff`, and owned `Stadium` pointers.
- **Match**: Binds two `Club` entities, a `Stadium`, and a specific date. It manages participation through `MatchSquadEntry` vectors.

## Managers:
### The system employs a Manager pattern to handle the lifecycle and querying of entities. Each Manager provides consistent Create, Read, Update, and Delete (CRUD) functionalities.

- **PersonManager**: The owner of the master `Person` linked list. It handles raw C-string allocation and identity management.
- **PlayerManager & StaffManager**: Specialized managers that handle role-specific logic and storage using `std::vector`.
- **ClubManager**: Bridges entities by assigning Personnel and Stadiums to Clubs.
- **StadiumManager**: Manages venues and location-based filtering.
- **MatchManager**: Schedules matches, validates dates, and manages match-day squads.

### Data Querying (The Vector Pipeline)
All managers are using a functional filtering pattern. Search operations no longer return manual `ListNode` wrappers. Instead:
1. Data is returned as a `std::vector<T*>`.
2. This allows for **Chainable Filters** (e.g., `findPeopleByNationality(findPeopleOlderThan(all, 30), POLAND)`).
3. Memory management of the collection itself is automatic; only the underlying entity memory (managed by the specific Manager) requires manual cleanup.

## Limits, Restrictions, and Consistency
1. **Memory & Pointer Safety**: Cascading deletions are enforced. Deleting a `Person` in the `PersonManager` automatically triggers unlinking in the `ClubManager`, `MatchManager`, and removal of role wrappers in the `Player/StaffManagers`.
2. **Composition Integrity**: Deleting a "Role" (Player or Staff) does not delete the underlying `Person`, allowing individuals to remain in the system even after retiring or changing roles.
3. **Domain Rules**:
   - Clubs and Stadiums must have unique names; duplicate creation attempts trigger an automatic data update.
   - Match dates are restricted to a window of 1980 through 10 years into the future.