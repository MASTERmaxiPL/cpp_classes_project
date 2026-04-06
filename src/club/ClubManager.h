#ifndef FOOTBALL_MANAGEMENT_SYSTEM_CLUB_MANAGER_H
#define FOOTBALL_MANAGEMENT_SYSTEM_CLUB_MANAGER_H

#import "Club.h"


class ClubManager {
    Club* head;

public:
    ClubManager();
    ~ClubManager();

    void club(string name, Country country, string city, int founded_year);
    void club(string name, Country country, string city);
    void club(string name, Country country, int founded_year);
    void club(string name, Country country);

    Club* findClubByName(string name, Club* clubs);
    Club* findClubsByCountry(Country country, Club* clubs);
    Club* findClubsByCity(string city, Club* clubs);
    Club* findClubsByFoundedYear(int founded_year, Club* clubs);

    void deleteClub(Club* club);

    void displayClub(Club* club);
};


#endif