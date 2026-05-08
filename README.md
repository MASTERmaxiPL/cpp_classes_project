# C++ Project - Football Management System

### The Football Management System is a C++ application designed to model the ecosystem of football clubs, stadiums, personnel, and match events.
### The system balances manual memory management (raw pointers and linked lists) with modern functional paradigms (lambdas and generic filtering).

## Core Entities & Relations:
- **Person**: The core identity entity. It contains basic data like name, age, and nationality. To maintain a single "source of truth, `PersonManager` is the sole owner of these objects.
- **Player & Staff (Composition)**: `Player` and `Staff` entities have a pointer to a `Person`. `Player` adds tactical `Position` data, while `Staff` adds organizational `Role` data. These wrappers are owned and managed entirely by the **Club** they belong to.
- **Stadium**: Represents venues, linked to a specific `Club` via an `ownedBy` pointer.
- **Club**: The central organizational entity. It maintains `std::vector` collections of its hired `Player`, `Staff`, and owned `Stadium` pointers.
- **Match**: Binds two `Club` entities, a `Stadium`, and a specific date. It manages participation through `MatchSquadEntry` vectors.

## Managers:
### The system employs a Manager pattern to handle the lifecycle and querying of entities. Each Manager provides consistent Create, Read, Update, and Delete (CRUD) functionalities.

- **PersonManager**: The owner of the master `Person` linked list. It handles raw C-string allocation and identity management.
- **ClubManager**: Bridges entities by assigning Personnel and Stadiums to Clubs. It handles the creation and destruction of `Player` and `Staff` wrappers when a person is hired or fired.
- **StadiumManager**: Manages venues and location-based filtering.
- **MatchManager**: Schedules matches, validates dates, and manages match-day squads.

### Data Querying (The Vector Pipeline)
The system employs a functional pipeline for advanced querying. By decoupling "Role" data from "Identity" data, search operations utilize vectors of pointers to allow for chainable filtering logic:

1. **Roster Filtering**: Use `ClubManager` and generic utility functions to find people by their club role (e.g., all `FORWARDS`).
2. **Unwrapping**: Convert the resulting wrapper pointers (e.g., `Player*`) into identity pointers (`Person*`).
3. **Identity Filtering**: Pass that list to `PersonManager` to filter by physical traits such as age or nationality.

## Limits, Restrictions, and Consistency
1. **Memory & Pointer Safety**: Cascading deletions are enforced. Deleting a `Person` in the `PersonManager` automatically triggers unlinking in the `ClubManager`, `MatchManager`, and removal of role wrappers in the `Player/StaffManagers`.
2. **Composition Integrity**: Deleting a "Role" (Player or Staff) does not delete the underlying `Person`, allowing individuals to remain in the system even after retiring or changing roles.
3. **Domain Rules**:
   - Clubs and Stadiums must have unique names; duplicate creation attempts trigger an automatic data update.
   - Match dates are restricted to a window of 1980 through 10 years into the future.
   - Person can be hired into single club at a time and can have only one role (Player or Staff) within that club. Re-hiring or role changes removed the person from the previous place of work.