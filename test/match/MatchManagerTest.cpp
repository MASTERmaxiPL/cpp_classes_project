#include <gtest/gtest.h>

#include "../../src/match/MatchManager.h"
#include "club/Club.h"
#include "../../src/person/player/Player.h"

class MatchManagerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        stadium_1->data.name = new char[10];
        strcpy(stadium_1->data.name, "Stadium1");

        auto* stadium_2 = new Stadium;
        stadium_2->data.name = new char[10];
        strcpy(stadium_2->data.name, "Stadium2");

        club_1->data.name = new char[10];
        strcpy(club_1->data.name, "HomeClub1");

        club_2->data.name = new char[10];
        strcpy(club_2->data.name, "HomeClub2");

        auto club_3 = new Club;
        club_3->data.name = new char[10];
        strcpy(club_3->data.name, "HomeClub3");

        auto club_4 = new Club;
        club_4->data.name = new char[10];
        strcpy(club_4->data.name, "HomeClub4");

        auto* player_1 = new MatchSquadEntry;
        player_1->player_id = 1;
        player_1->player_position = DEFENDER;
        player_1->next = nullptr;

        auto* player_2 = new MatchSquadEntry;
        player_2->player_id = 2;
        player_2->player_position = FORWARD;
        player_2->next = player_1;

        auto* player_3 = new MatchSquadEntry;
        player_3->player_id = 3;
        player_3->player_position = DEFENDER;
        player_3->next = nullptr;

        auto* player_4 = new MatchSquadEntry;
        player_4->player_id = 4;
        player_4->player_position = FORWARD;
        player_4->next = player_3;


        mm.match(*mm.createDate(10, 11, 2015), club_1, club_2, stadium_1, 2, 1, player_2, player_4);
        mm.match(*mm.createDate(26, 04, 2018), club_2, club_3, stadium_1, 2, 1, player_2, player_4);
        mm.match(*mm.createDate(20, 07, 2020), club_3, club_4, stadium_2, 2, 1, player_2, player_4);
        mm.match(*mm.createDate(15, 07, 2020), club_1, club_4, stadium_2, 2, 1, player_2, player_4);
        mm.match(*mm.createDate(15, 07, 2027), club_2, club_4, stadium_2);
    }

    MatchManager mm;
    Club * club_1 = new Club, *club_2 = new Club;
    Stadium *stadium_1 = new Stadium;
};

TEST_F(MatchManagerTest, AssignmentOperation) {
    MatchManager copy;
    copy = mm;

    auto list = copy.getAllMatchesCollection();
    EXPECT_NE(list.empty(), true);
    EXPECT_EQ(list.front()->id, 4);
}

TEST_F(MatchManagerTest, CopyConstructor) {
    MatchManager copy(mm);

    auto list = copy.getAllMatchesCollection();
    EXPECT_NE(list.empty(), true);
    EXPECT_EQ(list.front()->id, 4);
}

TEST(MatchManagerAdditionTest, AddMatchToEmptyList) {
    MatchManager mm;
    auto* stadium = new Stadium;
    stadium->data.name = new char[10];
    strcpy(stadium->data.name, "Stadium1");

    auto* club_1 = new Club;
    club_1->data.name = new char[10];
    strcpy(club_1->data.name, "HomeClub1");

    auto* club_2 = new Club;
    club_2->data.name = new char[10];
    strcpy(club_2->data.name, "HomeClub2");

    mm.match(*mm.createDate(10, 11, 2015), club_1, club_2, stadium);

    auto list = mm.getAllMatchesCollection();
    EXPECT_NE(list.empty(), true);
    EXPECT_EQ(list.front()->next, nullptr);
}

TEST(MatchManagerAdditionTest, AddMultipleMatches)
{
    MatchManager mm;
    auto* stadium = new Stadium;
    stadium->data.name = new char[10];
    strcpy(stadium->data.name, "Stadium1");

    auto* club_1 = new Club;
    club_1->data.name = new char[10];
    strcpy(club_1->data.name, "HomeClub1");

    auto* club_2 = new Club;
    club_2->data.name = new char[10];
    strcpy(club_2->data.name, "HomeClub2");

    mm.match(*mm.createDate(10, 11, 2015), club_1, club_2, stadium);
    mm.match(*mm.createDate(26, 04, 2018), club_1, club_2, stadium);

    auto list = mm.getAllMatchesCollection();
    EXPECT_NE(list.empty(), true);
    EXPECT_NE(list.front()->next, nullptr);
    EXPECT_EQ(list.front()->next->next, nullptr);
}

TEST_F(MatchManagerTest, UpdateMatch)
{
    auto list = mm.getAllMatchesCollection();
    auto* match = list.back();

    tm* newDate = mm.createDate(01, 01, 2020);
    mm.updateMatch(match, *newDate, nullptr, nullptr, nullptr, 3, 2, nullptr, nullptr);

    EXPECT_EQ(match->data.date.tm_mday, 1);
    EXPECT_EQ(match->data.date.tm_year, 2020-1900);

    EXPECT_NE(match->data.home_club, nullptr);

    EXPECT_EQ(match->data.score_home_club, 3);
    EXPECT_EQ(match->data.score_away_club, 2);

    EXPECT_NE(match->data.homeSquad, nullptr);
}

TEST(MatchManagerGetters, getAllMatchesCollectionFromEmptyList) {
    MatchManager mm;

    const auto list = mm.getAllMatchesCollection();
    EXPECT_EQ(list.empty(), true);
}

TEST_F(MatchManagerTest, getAllMatchesCollectionFromExistingList) {
    const auto list = mm.getAllMatchesCollection();
    EXPECT_NE(list.empty(), true);
    EXPECT_NE(list.front()->next, nullptr);
    EXPECT_EQ(list.front()->next->next->next->next->next, nullptr);
}

TEST_F(MatchManagerTest, TryToFindNotExistingMatchById)
{
    const auto list = mm.getAllMatchesCollection();

    Match* match = mm.findMatchById(9999, list);
    EXPECT_EQ(match, nullptr);
}

TEST_F(MatchManagerTest, FindMatchById)
{
    const auto list = mm.getAllMatchesCollection();
    auto* match = list.front();

    Match* foundMatch = mm.findMatchById(match->id, list);
    EXPECT_EQ(foundMatch, match);
}

TEST_F(MatchManagerTest, FindMatchesByDateWithEmptyList)
{
    const auto list = mm.getAllMatchesCollection();

    const auto result = mm.findMatchesByDate(*mm.createDate(10, 11, 2015), 0, 0, 0, list);
    EXPECT_EQ(result.empty(), false);
}

TEST_F(MatchManagerTest, FindMatchesByDateWithNoMatchesInRange)
{
    const auto list = mm.getAllMatchesCollection();

    const auto result = mm.findMatchesByDate(*mm.createDate(01, 01, 2000), 0, 0, 0, list);
    EXPECT_EQ(result.empty(), true);
}

TEST_F(MatchManagerTest, FindMatchesByDateWithMatchesInRange)
{
    const auto list = mm.getAllMatchesCollection();

    const auto result = mm.findMatchesByDate(*mm.createDate(15, 07, 2020), 0, 0, 0, list);
    EXPECT_NE(result.empty(), true);
    EXPECT_EQ(result.front() == result.back(), true);
}

TEST_F(MatchManagerTest, FindMatchesByDateWithMatchesInRangeWithRange)
{
    const auto list = mm.getAllMatchesCollection();

    const auto result = mm.findMatchesByDate(*mm.createDate(15, 07, 2020), 5, 0, 0, list);
    EXPECT_NE(result.empty(), true);
    EXPECT_EQ(result.size(), 2);
}

TEST_F(MatchManagerTest, FindMatchesByClubWithEmptyList)
{
    const auto list = mm.getAllMatchesCollection();

    const auto result = mm.findMatchesByClub(nullptr, list);
    EXPECT_EQ(result.empty(), true);
}

TEST_F(MatchManagerTest, FindMatchesByClubWithNoMatches)
{
    const auto list = mm.getAllMatchesCollection();

    auto* club = new Club;
    club->data.name = new char[10];
    strcpy(club->data.name, "NonExistingClub");

    const auto result = mm.findMatchesByClub(club, list);
    EXPECT_EQ(result.empty(), true);

    delete[] club->data.name;
    delete club;
}

TEST_F(MatchManagerTest, FindMatchesByClubWithMatches)
{
    const auto list = mm.getAllMatchesCollection();

    auto result = mm.findMatchesByClub(club_1, list);
    EXPECT_NE(result.empty(), true);
    EXPECT_EQ(result.size(), 2);
}

TEST_F(MatchManagerTest, FindMatchesByClubsWithNoMatches)
{
    const auto list = mm.getAllMatchesCollection();

    auto* club1 = new Club;
    club1->data.name = new char[10];
    strcpy(club1->data.name, "NonExistingClub1");

    const auto result = mm.findMatchesByClubs(club1, club_2, list);
    EXPECT_EQ(result.empty(), true);

    delete[] club1->data.name;
    delete club1;
}

TEST_F(MatchManagerTest, FindMatchesByClubsWithMatches)
{
    const auto list = mm.getAllMatchesCollection();

    const auto result = mm.findMatchesByClubs(club_1, club_2, list);
    EXPECT_EQ(result.empty(), false);
    EXPECT_EQ(result.size(), 1);
}

TEST(MatchManagerFilterTest, FindMatchesByStadiumWithEmptyList)
{
    MatchManager mm;

    auto* stadium = new Stadium;
    stadium->data.name = new char[10];
    strcpy(stadium->data.name, "NonExistingStadium");

    const auto list = mm.getAllMatchesCollection();

    const auto result = mm.findMatchesByStadium(stadium, list);
    EXPECT_EQ(result.empty(), true);

    delete[] stadium->data.name;
    delete stadium;
}

TEST_F(MatchManagerTest, FindMatchesByStadiumWithNoMatches)
{
    const auto list = mm.getAllMatchesCollection();

    auto* stadium = new Stadium;
    stadium->data.name = new char[10];
    strcpy(stadium->data.name, "NonExistingStadium");

    auto result = mm.findMatchesByStadium(stadium, list);
    EXPECT_EQ(result.empty(), true);

    delete[] stadium->data.name;
    delete stadium;
}

TEST_F(MatchManagerTest, FindMatchesByStadiumWithMatches)
{
    const auto list = mm.getAllMatchesCollection();

    const auto result = mm.findMatchesByStadium(stadium_1, list);
    EXPECT_EQ(result.empty(), false);
    EXPECT_EQ(result.size(), 2);
}

TEST(MatchManagerFilterTest, FindUnplayedMatchesWithNoUnplayedMatches)
{
    MatchManager mm;

    auto* stadium = new Stadium;
    stadium->data.name = new char[10];
    strcpy(stadium->data.name, "Stadium1");

    auto* club_1 = new Club;
    club_1->data.name = new char[10];
    strcpy(club_1->data.name, "HomeClub1");

    auto* club_2 = new Club;
    club_2->data.name = new char[10];
    strcpy(club_2->data.name, "HomeClub2");

    mm.match(*mm.createDate(10, 11, 2015), club_1, club_2, stadium, 2, 1);

    const auto list = mm.getAllMatchesCollection();

    const auto result = mm.findUnplayedMatches(list);
    EXPECT_EQ(result.empty(), true);
}

TEST_F(MatchManagerTest, FindUnplayedMatchesWithUnplayedMatches)
{
    const auto list = mm.getAllMatchesCollection();

    const auto result = mm.findUnplayedMatches(list);
    EXPECT_EQ(result.empty(), false);
    EXPECT_EQ(result.size(), 1);
}

TEST_F(MatchManagerTest, ChainFilters)
{
    const auto list = mm.getAllMatchesCollection();

    const auto matchesByClub = mm.findMatchesByClub(club_2, list);
    const auto unplayedMatchesByClub = mm.findUnplayedMatches(matchesByClub);

    EXPECT_NE(unplayedMatchesByClub.empty(), true);
    EXPECT_EQ(unplayedMatchesByClub.size(), 1);
}

TEST_F(MatchManagerTest, DeleteMatch)
{
    const auto list = mm.getAllMatchesCollection();
    auto* match = mm.findMatchById(1, list);

    const bool deleted = mm.deleteMatch(match);
    EXPECT_TRUE(deleted);

    Match* foundMatch = mm.findMatchById(1, list);
    EXPECT_EQ(foundMatch, nullptr);
}

TEST_F(MatchManagerTest, DeleteNotExistingMatch)
{
    auto list = mm.getAllMatchesCollection();

    Match match;
    const bool deleted = mm.deleteMatch(&match);
    EXPECT_FALSE(deleted);
}

TEST_F(MatchManagerTest, DeleteAllMatches)
{
    mm.deleteAllMatches();

    const auto list = mm.getAllMatchesCollection();
    EXPECT_EQ(list.empty(), true);
}

TEST_F(MatchManagerTest, DisplayMatch)
{
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    const auto list = mm.getAllMatchesCollection();

    mm.displayMatch(list.back());

    std::cout.rdbuf(old);
    std::string output = buffer.str();

    EXPECT_NE(output.find("Date: 10/11/2015"), std::string::npos);
    EXPECT_NE(output.find("Home Club: HomeClub1"), std::string::npos);
    EXPECT_NE(output.find("Away Club: HomeClub2"), std::string::npos);
    EXPECT_NE(output.find("Stadium: Stadium1"), std::string::npos);
    EXPECT_NE(output.find("Score: 2 - 1"), std::string::npos);
}

TEST_F(MatchManagerTest, DisplayMatchesList)
{
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    mm.displayMatchesList();

    std::cout.rdbuf(old);
    std::string output = buffer.str();

    EXPECT_NE(output.find("Date: 10/11/2015"), std::string::npos);
    EXPECT_NE(output.find("Date: 26/04/2018"), std::string::npos);
    EXPECT_NE(output.find("Date: 15/07/2027"), std::string::npos);

    auto list = mm.getAllMatchesCollection();
}