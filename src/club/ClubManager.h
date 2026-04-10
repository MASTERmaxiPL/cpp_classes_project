#ifndef FOOTBALL_MANAGEMENT_SYSTEM_CLUB_MANAGER_H
#define FOOTBALL_MANAGEMENT_SYSTEM_CLUB_MANAGER_H

#import "Club.h"


class ClubManager {
    Club* head;

    static void clearClubMemory(Club* club);

    static ClubListNode* wrap(Club* head);
public:
    ClubManager();
    ~ClubManager();

    void club(const char* name, Country country, const char* city, int founded_year);
    void club(const char* name, Country country, const char* city);
    void club(const char* name, Country country, int founded_year);
    void club(const char* name, Country country);


    Club* getAllClubs() const;
    ClubListNode* getAllClubsWrapped() const;

    Club* findClubByName(const char* name, Club* clubs);
    static Club* findClubByNameInWrapper(const char* name, ClubListNode* head) ;

    static ClubListNode* findClubsByCountry(Country country, ClubListNode* head);
    static ClubListNode* findClubsByCity(const char* city, ClubListNode* head);
    static ClubListNode* findClubsByFoundedYear(int founded_year, ClubListNode* head);
    static ClubListNode* findClubsByNewerFoundedYear(int founded_year, ClubListNode* head);
    static ClubListNode* findClubsByOlderFoundedYear(int founded_year, ClubListNode* head);

    bool deleteClub(Club* club);
    static bool deleteWrappedClub(ClubListNode*& head, Club* club);

    void deleteAllClubs(Club* head);
    static void deleteAllWrappedList(ClubListNode*& head);

    static void displayClub(const Club* club);
    static void displayClubList(Club* head);

    static void displayWrappedClub(const ClubListNode* wrapped_club);
    static void displayWrappedClubList(ClubListNode* wrapped_club);

    void addPlayerToClub(Player* player, Club* club);
    void addStaffToClub(Staff* staff, Club* club);
};


#endif