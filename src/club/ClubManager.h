#ifndef FOOTBALL_MANAGEMENT_SYSTEM_CLUB_MANAGER_H
#define FOOTBALL_MANAGEMENT_SYSTEM_CLUB_MANAGER_H

#include <functional>
#include <vector>

#include "Club.h"
#include "../match/MatchManager.h"
#include "../person/player/Player.h"


class ClubManager {
    Club* head;
    MatchManager* matchManager;

    void clearClubMemory(Club* club) const;
    static Club* copyClub(const Club* other);
public:
    explicit ClubManager(MatchManager* matchManager = nullptr);
    ~ClubManager();

    ClubManager(const ClubManager& other);
    ClubManager& operator=(const ClubManager& other);

    void club(const char* name, Country country, const char* city, int founded_year);
    void club(const char* name, Country country, const char* city);
    void club(const char* name, Country country, int founded_year);
    void club(const char* name, Country country);

    vector<Club*> getAllClubsCollection() const;

    Club* findClubByName(const char* name) const;

    static vector<Club*> filterClubs(const vector<Club*>& clubs, const function<bool(Club*)>& predicate);
    static vector<Club*> findClubsByCountry(Country country, const std::vector<Club*>& clubs);
    static vector<Club*> findClubsByCity(const char* city, const std::vector<Club*>& clubs);
    static vector<Club*> findClubsByFoundedYear(int founded_year, const std::vector<Club*>& clubs);
    static vector<Club*> findClubsByNewerFoundedYear(int founded_year, const std::vector<Club*>& clubs);
    static vector<Club*> findClubsByOlderFoundedYear(int founded_year, const std::vector<Club*>& clubs);

    bool deleteClub(Club* club);

    void deleteAllClubs();

    static void displayClub(const Club* club);
    void displayClubList() const;

    static void addStadiumToClub(Stadium* stadium, Club* club);
    static void addPlayerToClub(Person* person, Position position, Club* club);
    static void addStaffToClub(Person* person, Role role, Club* club);

    static bool removeStadiumFromClub(Stadium* stadium, Club* club);
    static bool removePersonFromClub(Person* person, Club* club);

    static int getClubStadiumsCount(const Club* club);
    static int getClubPlayersCount(const Club* club);
    static int getClubStaffCount(const Club* club);

    static vector<Player*> getPlayersByPosition(Position pos, const Club* club);
    static vector<Staff*> getStaffsByRole(Role role, const Club* club);
    static vector<Person*> extractPeopleFromPlayers(const vector<Player*>& players);
    static vector<Person*> extractPeopleFromStaff(const vector<Staff*>& staff);

    void setMatchManager(MatchManager* mgr);
};


#endif