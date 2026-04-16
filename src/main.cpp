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
    sm.stadium("Stadium First", static_cast<Country>(2), "CityA", 80000);
    sm.stadium("Stadium Second", static_cast<Country>(0), "CityC", 200000);
    sm.stadium("Stadium Third", static_cast<Country>(3), "CityD", 130000);
    Stadium* stadiumX = sm.findStadiumByName("Stadium Second");

    cm.club("Team A", static_cast<Country>(0), "CityA", 1900);
    cm.club("Team B", static_cast<Country>(2), "CityB", 1900);
    cm.club("Team C", static_cast<Country>(4), "CityC", 1938);
    cm.club("Team D", static_cast<Country>(5), "CityD", 1924);

    cm.addStadiumToClub(stadiumX, cm.findClubByName("Team A"));
    cm.displayClub(cm.findClubByName("Team A"));

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

    if (date1 && home_club && away_club) {
        cout << "Creating matches..." << endl;
        mm.match(*date1, home_club, away_club, stadiumX, -1, -1);
        mm.match(*date2, away_club, home_club, stadiumX, -1, -1);
    }
    delete date1;

    MatchListNode* matches = mm.getAllMatchesWrapped();
    Match* match1 = mm.findMatchById(0, matches);
    Match* match2 = mm.findMatchById(1, matches);

    if (match1 && match2) {
        mm.addPlayerToSquad(match1, 1, static_cast<Position>(2), true);
        mm.addPlayerToSquad(match1, 0, static_cast<Position>(0), true);
        mm.addPlayerToSquad(match1, 3, static_cast<Position>(1), false);
        mm.addPlayerToSquad(match1, 2, static_cast<Position>(0), false);

        mm.addPlayerToSquad(match2, 1, static_cast<Position>(2), false);
        mm.addPlayerToSquad(match2, 0, static_cast<Position>(0), false);
        mm.addPlayerToSquad(match2, 3, static_cast<Position>(1), true);
        mm.addPlayerToSquad(match2, 2, static_cast<Position>(0), true);
    }


    cout << "=== Matches after creation ===" << endl;

    mm.displayMatchesList();

    // EXAMPLE 0
    // Find people older than 26
    PersonListNode* people = pm.findPeopleOlderThan(26, pm.getAllPeopleWrapped());
    cout << "=== People older than 26 ===" << endl;
    pm.displayWrappedPeopleList(people);
    pm.deleteAllWrappedPeople(people);

    // EXAMPLE 1
    // Updating match data ( moving match by 5 days and setting score )
    MatchListNode* match_node = mm.findMatchesByDate(*date2, 0, 0, 0, matches);

    delete date2;

    if (match_node)
    {
        Match *m = match_node->match;

        if (m)
        {
            tm* newDate = mm.createDate(m->data.date.tm_mday + 5, m->data.date.tm_mon + 1, m->data.date.tm_year + 1900);
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
        cout << "Deleting person..." << endl;
        pm.deletePerson(1);
    }
    cout << "=== Matches after person deletion (person removed from match squads) ===" << endl;
    if (stadiumX)
        cout << "Deleting stadium: " << stadiumX->data.name << endl;
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
