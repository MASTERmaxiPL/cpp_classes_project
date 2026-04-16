#ifndef FOOTBALL_MANAGEMENT_SYSTEM_CLUB_MANAGER_H
#define FOOTBALL_MANAGEMENT_SYSTEM_CLUB_MANAGER_H

#import "Club.h"
#include "match/MatchManager.h"
#include "person/Player.h"


class ClubManager {
    Club* head;
    MatchManager* matchManager;

    static void deleteAllWrappedClubStadiums(StadiumListNode*& head);
    static void deleteAllWrappedClubPeople(PersonListNode*& head);
    void clearClubMemory(Club* club) const;
public:
    ClubManager(MatchManager* matchManager = nullptr);
    ~ClubManager();

    ClubManager(const ClubManager& other);
    ClubManager& operator=(const ClubManager& other);

    void club(const char* name, Country country, const char* city, int founded_year);
    void club(const char* name, Country country, const char* city);
    void club(const char* name, Country country, int founded_year);
    void club(const char* name, Country country);

    ClubListNode* getAllClubsWrapped() const;

    Club* findClubByName(const char* name);
    static Club* findClubByNameInWrapper(const char* name, ClubListNode* head) ;

    static ClubListNode* findClubsByCountry(Country country, ClubListNode* head);
    static ClubListNode* findClubsByCity(const char* city, ClubListNode* head);
    static ClubListNode* findClubsByFoundedYear(int founded_year, ClubListNode* head);
    static ClubListNode* findClubsByNewerFoundedYear(int founded_year, ClubListNode* head);
    static ClubListNode* findClubsByOlderFoundedYear(int founded_year, ClubListNode* head);

    bool deleteClub(Club* club);
    static bool deleteWrappedClub(ClubListNode*& head, Club* club);

    void deleteAllClubs();
    static void deleteAllWrappedList(ClubListNode*& head);

    static void displayClub(const Club* club);
    void displayClubList() const;

    static void displayWrappedClub(const ClubListNode* wrapped_club);
    static void displayWrappedClubList(ClubListNode* wrapped_club);

    static void addStadiumToClub(Stadium* stadium, Club* club);
    static void addPlayerToClub(Player* player, Club* club);
    static void addStaffToClub(Staff* staff, Club* club);

    static bool removeStadiumFromClub(Stadium* stadium, Club* club);
    static bool removePersonFromClub(Person* person, Club* club);

    static int getClubStadiumsCount(const Club* club);
    static int getClubPlayersCount(const Club* club);
    static int getClubStaffCount(const Club* club);

    void setMatchManager(MatchManager* mgr);
};


#endif