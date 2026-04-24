#include <iostream>
#include <vector>

#include "match/MatchManager.h"
#include "club/ClubManager.h"
#include "stadium/StadiumManager.h"
#include "person/PersonManager.h"
#include "person/player/PlayerManager.h"
#include "person/staff/StaffManager.h"

using namespace std;

int main()
{
    MatchManager mm;
    ClubManager cm(&mm);
    StadiumManager sm(&mm);
    PersonManager pm(&mm);
    PlayerManager plm;
    StaffManager stm;

    sm.stadium("Stadium First", static_cast<Country>(2), "CityA", 80000);
    sm.stadium("Stadium Second", static_cast<Country>(0), "CityC", 200000);
    sm.stadium("Stadium Third", static_cast<Country>(3), "CityD", 130000);

    auto allStadiums = sm.getAllStadiumsCollection();
    Stadium* stadiumX = sm.findStadiumByName("Stadium Second", allStadiums);

    cm.club("Team A", static_cast<Country>(0), "CityA", 1900);
    cm.club("Team B", static_cast<Country>(2), "CityB", 1900);
    cm.club("Team C", static_cast<Country>(4), "CityC", 1938);
    cm.club("Team D", static_cast<Country>(5), "CityD", 1924);

    Club* teamA = cm.findClubByName("Team A");
    cm.addStadiumToClub(stadiumX, teamA);
    cm.displayClub(teamA);

    pm.person("John", "Doe", 25, static_cast<Country>(0));
    pm.person("Max", "Payne", 28, static_cast<Country>(2));
    pm.person("John", "Doe", 32, static_cast<Country>(0));
    pm.person("Wendy", "Hamilz", 28, static_cast<Country>(1));
    pm.person("Ling", "Pollen", 28, static_cast<Country>(3));
    pm.person("Max", "Wong", 28, static_cast<Country>(0));

    auto allPeople = pm.getAllPeopleCollection();

    Person* p_john25 = PersonManager::findPeopleByAge(25, PersonManager::findPeopleByName("John", "Doe", allPeople))[0];
    Person* p_max = PersonManager::findPeopleByName("Max", "Payne", allPeople)[0];
    Person* p_john32 = PersonManager::findPeopleByAge(32, PersonManager::findPeopleByName("John", "Doe", allPeople))[0];
    Person* p_wendy = PersonManager::findPeopleByName("Wendy", "Hamilz", allPeople)[0];
    Person* s_ling = PersonManager::findPeopleByName("Ling", "Pollen", allPeople)[0];
    Person* s_max = PersonManager::findPeopleByName("Max", "Wong", allPeople)[0];

    plm.addPlayer(p_john25, static_cast<Position>(2));
    plm.addPlayer(p_max, static_cast<Position>(0));
    plm.addPlayer(p_john32, static_cast<Position>(3));
    plm.addPlayer(p_wendy, static_cast<Position>(1));

    stm.addStaff(s_ling, static_cast<Role>(0));
    stm.addStaff(s_max, static_cast<Role>(1));

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

    auto allMatches = mm.getAllMatchesCollection();
    if (allMatches.size() >= 2) {
        Match* match1 = allMatches[0];
        Match* match2 = allMatches[1];

        mm.addPlayerToSquad(match1, p_john25->id, static_cast<Position>(2), true);
        mm.addPlayerToSquad(match1, p_max->id, static_cast<Position>(0), true);
        mm.addPlayerToSquad(match1, p_john32->id, static_cast<Position>(1), false);
        mm.addPlayerToSquad(match1, p_wendy->id, static_cast<Position>(0), false);

        mm.addPlayerToSquad(match2, p_john25->id, static_cast<Position>(2), false);
        mm.addPlayerToSquad(match2, p_max->id, static_cast<Position>(0), false);
        mm.addPlayerToSquad(match2, p_john32->id, static_cast<Position>(1), true);
        mm.addPlayerToSquad(match2, p_wendy->id, static_cast<Position>(0), true);
    }

    cout << "=== Matches after creation ===" << endl;
    mm.displayMatchesList();

    // EXAMPLE 0
    auto olderPeople = PersonManager::findPeopleOlderThan(26, pm.getAllPeopleCollection());
    cout << "=== People older than 26 ===" << endl;
    for (Person* p : olderPeople) {
        pm.displayPerson(p);
    }

    // EXAMPLE 1
    auto matches_by_date = mm.findMatchesByDate(*date2, 0, 0, 0, mm.getAllMatchesCollection());
    delete date2;

    if (!matches_by_date.empty())
    {
        Match *m = matches_by_date[0];
        tm* newDate = mm.createDate(m->data.date.tm_mday + 5, m->data.date.tm_mon + 1, m->data.date.tm_year + 1900);
        if (newDate)
        {
            mm.updateMatch(m, *newDate, nullptr, nullptr, nullptr, 3, 2, nullptr, nullptr);
            delete newDate;
        }
    }
    cout << "=== Matches after update ===" << endl;
    mm.displayMatchesList();

    // EXAMPLE 2
    if (p_max)
    {
        cout << "Deleting person (Max Payne)..." << endl;
        plm.removePlayerByPersonId(p_max->id);
        pm.deletePerson(p_max->id);
    }
    cout << "=== Matches after person deletion (person removed from match squads) ===" << endl;

    if (stadiumX) {
        cout << "Deleting stadium: " << stadiumX->data.name << endl;
        sm.deleteStadium(stadiumX);
    }

    cout << "=== Matches after deletions (clubs/stadiums/persons updated) ===" << endl;
    mm.displayMatchesList();

    cout << "=== Clubs after deletions (stadiums/persons updated) ===" << endl;
    cm.displayClubList();

    return 0;
}