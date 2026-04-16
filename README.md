# C++ Project - Football Management System

### The Football Management System is a C++ application designed to model the ecosystem of football clubs, stadiums, personnel, and match events.
### The system is split into pure data models (Entities) and dedicated functional controllers (Managers)

## Core Entities & Relations:
Person (Player & Staff): The base class for individuals in the system. Players possess specific tactical Positions, while Staff hold organizational Roles. Every person has a hiredBy pointer linking them to a Club. Each player and staff member has a unique id to facilitate tracking and management.

Stadium: Another base class, representing stadiums. It maintains an ownedBy pointer to link it to a specific Club. Each stadium has unique name.

Club: The central organizational entity. A Club maintains separate linked lists of its hired Players, Staff, and owned Stadiums. Each club has a unique name.

Match: Represents a scheduled or completed game. It binds a specific date, two distinct Club entities (Home and Away), a Stadium and custom MatchSquadEntry lists defining which players participated on which position. Each match has a unique id for tracking and management purposes.

## Managers:
### The system employs a Manager pattern to handle the lifecycle and querying of entities. Each Manager provides consistent Create, Read, Update, and Delete (CRUD) functionalities.

PersonManager: Handles adding players and staff and filtering them by different parameters.

ClubManager: Manages club data and acts as the bridge for assigning Stadiums and Personnel to a club.

StadiumManager: Registers venues and allows filtering by location or seating capacity restrictions.

MatchManager: The most complex controller. It schedules matches, validates date boundaries, updates scores, and manages the allocation of players to specific match squads.

## Limits, Restrictions, and Consistency
### To ensure the system remains completely consistent and robust, several strict rules and cascading behaviors are enforced:

1. Memory & Pointer Safety (Cascading Deletions): Because entities reference each other via raw pointers, deleting an entity must be handled manually. For example, if deletePerson() is called, before the person is removed, PersonManager calls ClubManager to remove the connection between it and assigned Club and then calls the MatchManager to wipe the player from all historical or future match squads. This guarantees no dangling pointers or "ghost" players remain in the system.

2. Domain-Specific Restrictions:

   - A Match strictly prohibits the same Club from being both the Home and Away team.

   - Match dates are hard-limited: they cannot be scheduled before 1980 or further than 10 years into the future.

   - Clubs and Stadiums cannot have duplicate names; attempting to create one with existing name triggers an automatic "update" rather than a duplicate entry.

3. Wrapper Lists: Filtering operations (e.g., findClubsByCountry) return lightweight ListNode wrappers rather than cloning the actual entities. This saves memory but requires the caller to delete the wrapper lists after use without deleting the actual underlying data.