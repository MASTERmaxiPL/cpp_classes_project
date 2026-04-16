#include <iostream>

#include "match/MatchManager.h"
#include "club/ClubManager.h"
#include "stadium/StadiumManager.h"
#include "person/PersonManager.h"

using namespace std;

int main()
{
    // Init of managers
    MatchManager mm;
    ClubManager cm(&mm);
    StadiumManager sm(&mm);
    PersonManager pm(&mm);

    // Create some clubs, stadiums and people
    cm.club("Team A", static_cast<Country>(0), "CityA", 1900);
    cm.club("Team B", static_cast<Country>(2), "CityB", 1900);
    cm.club("Team C", static_cast<Country>(4), "CityC", 1938);
    cm.club("Team D", static_cast<Country>(5), "CityD", 1924);

    sm.stadium("Stadium First", static_cast<Country>(2), "CityA", 80000);
    sm.stadium("Stadium Second", static_cast<Country>(0), "CityC", 200000);
    sm.stadium("Stadium Third", static_cast<Country>(3), "CityD", 130000);
    Stadium* stadiumX = sm.findStadiumByName("Stadium Second");

    pm.player("John", "Doe", 25, static_cast<Country>(0), static_cast<Position>(2));
    pm.player("Max", "Payne", 28, static_cast<Country>(2), static_cast<Position>(0));
    pm.player("John", "Doe", 32, static_cast<Country>(0), static_cast<Position>(3));
    pm.player("Wendy", "Hamilz", 28, static_cast<Country>(1), static_cast<Position>(1));
    pm.staff("Ling", "Pollen", 28, static_cast<Country>(3), static_cast<Role>(0));
    pm.staff("Max", "Wong", 28, static_cast<Country>(0), static_cast<Role>(1));

    // Preparing matches
    Club* home_club = cm.findClubByName("Team C");
    Club* away_club = cm.findClubByName("Team B");

    tm* date1 = mm.createDate(10, 10, 2019);
    tm* date2 = mm.createDate(20, 11, 2019);

    MatchSquadEntry* homeSquad = new MatchSquadEntry{1, static_cast<Position>(2), nullptr};

    MatchSquadEntry* awaySquad = new MatchSquadEntry{2, static_cast<Position>(0), nullptr};

    if (date1 && home_club && away_club) {
        mm.match(*date1, home_club, away_club, stadiumX, -1, -1, homeSquad, awaySquad);
        mm.match(*date2, away_club, home_club, stadiumX, -1, -1, awaySquad, homeSquad);
    }
    delete date1;


    cout << "=== Matches after creation ===" << endl;

    mm.displayMatchesList();

    // EXAMPLE 0
    // Find people older than 26
    PersonListNode* people = pm.findPeopleOlderThan(26, pm.getAllPeopleWrapped());
    cout << "=== People older than 26 ===" << endl;
    pm.displayWrappedPeopleList(people);
    pm.deleteAllWrappedPeople(people);

    // EXAMPLE 1
    // Updating match data ( moving match by 5 days )
    MatchListNode* matches = mm.getAllMatchesWrapped();
    MatchListNode* match_node = mm.findMatchesByDate(*date2, 0, 0, 0, matches);

    delete date2;

    if (match_node)
    {
        Match *m = match_node->match;

        if (m)
        {
            tm* newDate = mm.createDate(m->data.date.tm_mday + 5, m->data.date.tm_mon, m->data.date.tm_year);
            if (newDate)
            {
                mm.updateMatch(m, *newDate, nullptr, nullptr, nullptr, 3, 2, nullptr, nullptr);
                delete newDate;
            }
        }
    }
    cout << "=== Matches after update ===" << endl;
    mm.displayMatchesList();

    // EXAMPLE 2
    // Remove person and stadium, then updating matches data (removing person from match squads)
    PersonListNode* peopleWrapped = pm.getAllPeopleWrapped();
    if (peopleWrapped && peopleWrapped->person)
    {
        pm.deletePerson(1);
    }

    if (stadiumX)
        sm.deleteStadium(stadiumX);

    cout << "=== Matches after deletions (clubs/stadiums/persons updated) ===" << endl;
    mm.displayMatchesList();

    cout << "=== Clubs after deletions (stadiums/persons updated) ===" << endl;
    cm.displayClubList();

    // Cleaning data( wrapped lists)
    mm.deleteAllWrappedMatches(matches);
    mm.deleteAllWrappedMatches(match_node);
    pm.deleteAllWrappedPeople(peopleWrapped);

    return 0;
}