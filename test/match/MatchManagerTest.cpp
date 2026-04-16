#include <gtest/gtest.h>

#include "../../src/match/MatchManager.h"
#include "person/Player.h"

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

    auto* list = mm.getAllMatchesWrapped();
    EXPECT_NE(list, nullptr);
    EXPECT_EQ(list->next, nullptr);

    mm.deleteAllWrappedMatches(list);
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

    auto* list = mm.getAllMatchesWrapped();
    EXPECT_NE(list, nullptr);
    EXPECT_NE(list->next, nullptr);
    EXPECT_EQ(list->next->next, nullptr);

    mm.deleteAllWrappedMatches(list);
}

TEST_F(MatchManagerTest, UpdateMatch)
{
    auto* list = mm.getAllMatchesWrapped();
    auto* match = list->match;

    tm* newDate = mm.createDate(01, 01, 2020);
    mm.updateMatch(match, *newDate, nullptr, nullptr, nullptr, 3, 2, nullptr, nullptr);

    EXPECT_EQ(match->data.date.tm_mday, 1);
    EXPECT_EQ(match->data.date.tm_year, 2020-1900);

    EXPECT_NE(match->data.home_club, nullptr);

    EXPECT_EQ(match->data.score_home_club, 3);
    EXPECT_EQ(match->data.score_away_club, 2);

    EXPECT_NE(match->data.homeSquad, nullptr);

    mm.deleteAllWrappedMatches(list);
}

TEST(ClubManagerGetters, GetAllMatchesWrappedFromEmptyList) {
    MatchManager mm;

    MatchListNode* list = mm.getAllMatchesWrapped();
    EXPECT_EQ(list, nullptr);

    mm.deleteAllWrappedMatches(list);
}

TEST_F(MatchManagerTest, GetAllMatchesWrappedFromExistingList) {
    auto* list = mm.getAllMatchesWrapped();
    EXPECT_NE(list, nullptr);
    EXPECT_NE(list->next, nullptr);
    EXPECT_EQ(list->next->next->next->next->next, nullptr);

    mm.deleteAllWrappedMatches(list);
}

TEST_F(MatchManagerTest, TryToFindNotExistingMatchById)
{
    auto *list = mm.getAllMatchesWrapped();

    Match* match = mm.findMatchById(9999, list);
    EXPECT_EQ(match, nullptr);
}

TEST_F(MatchManagerTest, FindMatchById)
{
    auto* list = mm.getAllMatchesWrapped();
    auto* match = list->match;

    Match* foundMatch = mm.findMatchById(match->id, list);
    EXPECT_EQ(foundMatch, match);

    mm.deleteAllWrappedMatches(list);
}

TEST_F(MatchManagerTest, FindMatchesByDateWithEmptyList)
{
    auto* list = mm.getAllMatchesWrapped();
    mm.deleteAllWrappedMatches(list);

    auto* result = mm.findMatchesByDate(*mm.createDate(10, 11, 2015), 0, 0, 0, nullptr);
    EXPECT_EQ(result, nullptr);
}

TEST_F(MatchManagerTest, FindMatchesByDateWithNoMatchesInRange)
{
    auto* list = mm.getAllMatchesWrapped();

    auto* result = mm.findMatchesByDate(*mm.createDate(01, 01, 2000), 0, 0, 0, list);
    EXPECT_EQ(result, nullptr);

    mm.deleteAllWrappedMatches(list);
}

TEST_F(MatchManagerTest, FindMatchesByDateWithMatchesInRange)
{
    auto* list = mm.getAllMatchesWrapped();

    auto* result = mm.findMatchesByDate(*mm.createDate(15, 07, 2020), 0, 0, 0, list);
    EXPECT_NE(result, nullptr);
    EXPECT_EQ(result->next, nullptr);

    mm.deleteAllWrappedMatches(list);
    mm.deleteAllWrappedMatches(result);
}

TEST_F(MatchManagerTest, FindMatchesByDateWithMatchesInRangeWithRange)
{
    auto* list = mm.getAllMatchesWrapped();

    auto* result = mm.findMatchesByDate(*mm.createDate(15, 07, 2020), 5, 0, 0, list);
    EXPECT_NE(result, nullptr);
    EXPECT_NE(result->next, nullptr);
    EXPECT_EQ(result->next->next, nullptr);

    mm.deleteAllWrappedMatches(list);
    mm.deleteAllWrappedMatches(result);
}

TEST_F(MatchManagerTest, FindMatchesByClubWithEmptyList)
{
    auto* list = mm.getAllMatchesWrapped();
    mm.deleteAllWrappedMatches(list);

    auto* result = mm.findMatchesByClub(nullptr, nullptr);
    EXPECT_EQ(result, nullptr);
}

TEST_F(MatchManagerTest, FindMatchesByClubWithNoMatches)
{
    auto* list = mm.getAllMatchesWrapped();

    auto* club = new Club;
    club->data.name = new char[10];
    strcpy(club->data.name, "NonExistingClub");

    auto* result = mm.findMatchesByClub(club, list);
    EXPECT_EQ(result, nullptr);

    mm.deleteAllWrappedMatches(list);
    delete[] club->data.name;
    delete club;
}

TEST_F(MatchManagerTest, FindMatchesByClubWithMatches)
{
    auto* list = mm.getAllMatchesWrapped();

    auto* result = mm.findMatchesByClub(club_1, list);
    EXPECT_NE(result, nullptr);
    EXPECT_NE(result->next, nullptr);
    EXPECT_EQ(result->next->next, nullptr);

    mm.deleteAllWrappedMatches(list);
    mm.deleteAllWrappedMatches(result);
}

TEST_F(MatchManagerTest, FindMatchesByClubsWithNoMatches)
{
    auto* list = mm.getAllMatchesWrapped();

    auto* club1 = new Club;
    club1->data.name = new char[10];
    strcpy(club1->data.name, "NonExistingClub1");

    auto* result = mm.findMatchesByClubs(club1, club_2, list);
    EXPECT_EQ(result, nullptr);

    mm.deleteAllWrappedMatches(list);
    delete[] club1->data.name;
    delete club1;
}

TEST_F(MatchManagerTest, FindMatchesByClubsWithMatches)
{
    auto* list = mm.getAllMatchesWrapped();

    auto* result = mm.findMatchesByClubs(club_1, club_2, list);
    EXPECT_NE(result, nullptr);
    EXPECT_EQ(result->next, nullptr);

    mm.deleteAllWrappedMatches(list);
    mm.deleteAllWrappedMatches(result);
}

TEST(MatchManagerFilterTest, FindMatchesByStadiumWithEmptyList)
{
    MatchManager mm;

    auto* stadium = new Stadium;
    stadium->data.name = new char[10];
    strcpy(stadium->data.name, "NonExistingStadium");

    auto* result = mm.findMatchesByStadium(stadium, nullptr);
    EXPECT_EQ(result, nullptr);

    delete[] stadium->data.name;
    delete stadium;
}

TEST_F(MatchManagerTest, FindMatchesByStadiumWithNoMatches)
{
    auto* list = mm.getAllMatchesWrapped();

    auto* stadium = new Stadium;
    stadium->data.name = new char[10];
    strcpy(stadium->data.name, "NonExistingStadium");

    auto* result = mm.findMatchesByStadium(stadium, list);
    EXPECT_EQ(result, nullptr);

    mm.deleteAllWrappedMatches(list);
    delete[] stadium->data.name;
    delete stadium;
}

TEST_F(MatchManagerTest, FindMatchesByStadiumWithMatches)
{
    auto* list = mm.getAllMatchesWrapped();

    auto* result = mm.findMatchesByStadium(stadium_1, list);
    EXPECT_NE(result, nullptr);
    EXPECT_NE(result->next, nullptr);
    EXPECT_EQ(result->next->next, nullptr);

    mm.deleteAllWrappedMatches(list);
    mm.deleteAllWrappedMatches(result);
}

TEST_F(MatchManagerTest, FindUnplayedMatchesWithEmptyList)
{
    auto* result = mm.findUnplayedMatches(nullptr);
    EXPECT_EQ(result, nullptr);
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

    auto* list = mm.getAllMatchesWrapped();

    auto* result = mm.findUnplayedMatches(list);
    EXPECT_EQ(result, nullptr);

    mm.deleteAllWrappedMatches(list);
}

TEST_F(MatchManagerTest, FindUnplayedMatchesWithUnplayedMatches)
{
    auto* list = mm.getAllMatchesWrapped();

    auto* result = mm.findUnplayedMatches(list);
    EXPECT_NE(result, nullptr);
    EXPECT_EQ(result->next, nullptr);

    mm.deleteAllWrappedMatches(list);
    mm.deleteAllWrappedMatches(result);
}

TEST_F(MatchManagerTest, ChainFilters)
{
    auto* list = mm.getAllMatchesWrapped();

    auto* matchesByClub = mm.findMatchesByClub(club_2, list);
    auto* unplayedMatchesByClub = mm.findUnplayedMatches(matchesByClub);

    EXPECT_NE(unplayedMatchesByClub, nullptr);
    EXPECT_EQ(unplayedMatchesByClub->next, nullptr);

    mm.deleteAllWrappedMatches(list);
    mm.deleteAllWrappedMatches(matchesByClub);
    mm.deleteAllWrappedMatches(unplayedMatchesByClub);
}

TEST_F(MatchManagerTest, DeleteMatch)
{
    auto* list = mm.getAllMatchesWrapped();
    auto* match = mm.findMatchById(1, list);

    bool deleted = mm.deleteMatch(match);
    EXPECT_TRUE(deleted);

    Match* foundMatch = mm.findMatchById(1, list);
    EXPECT_EQ(foundMatch, nullptr);

    mm.deleteAllWrappedMatches(list);
}

TEST_F(MatchManagerTest, DeleteNotExistingMatch)
{
    auto* list = mm.getAllMatchesWrapped();

    Match match;
    const bool deleted = mm.deleteMatch(&match);
    EXPECT_FALSE(deleted);

    mm.deleteAllWrappedMatches(list);
}

TEST_F(MatchManagerTest, DeleteWrappedMatch)
{
    auto* list = mm.getAllMatchesWrapped();
    const int32_t match_id = list->match->id;

    const bool deleted = mm.deleteWrappedMatch(list, match_id);
    EXPECT_TRUE(deleted);

    Match* foundMatch = mm.findMatchById(match_id, list);
    EXPECT_EQ(foundMatch, nullptr);

    mm.deleteAllWrappedMatches(list);
}

TEST_F(MatchManagerTest, DeleteAllMatches)
{
    mm.deleteAllMatches();

    auto* list = mm.getAllMatchesWrapped();
    EXPECT_EQ(list, nullptr);
}

TEST_F(MatchManagerTest, DeleteAllWrappedMatches)
{
    auto* list = mm.getAllMatchesWrapped();

    mm.deleteAllWrappedMatches(list);

    EXPECT_EQ(list, nullptr);
}

TEST_F(MatchManagerTest, DisplayMatch)
{
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    auto* list = mm.getAllMatchesWrapped();

    mm.displayMatch(list->match);

    std::cout.rdbuf(old);
    std::string output = buffer.str();

    EXPECT_NE(output.find("Date: 10/11/2015"), std::string::npos);
    EXPECT_NE(output.find("Home Club: HomeClub1"), std::string::npos);
    EXPECT_NE(output.find("Away Club: HomeClub2"), std::string::npos);
    EXPECT_NE(output.find("Stadium: Stadium1"), std::string::npos);
    EXPECT_NE(output.find("Score: 2 - 1"), std::string::npos);

    mm.deleteAllWrappedMatches(list);
}

TEST_F(MatchManagerTest, DisplayWrappedMatch)
{
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    auto* list = mm.getAllMatchesWrapped();

    mm.displayWrappedMatch(list);

    std::cout.rdbuf(old);
    std::string output = buffer.str();

    EXPECT_NE(output.find("Date: 10/11/2015"), std::string::npos);
    EXPECT_NE(output.find("Home Club: HomeClub1"), std::string::npos);
    EXPECT_NE(output.find("Away Club: HomeClub2"), std::string::npos);
    EXPECT_NE(output.find("Stadium: Stadium1"), std::string::npos);
    EXPECT_NE(output.find("Score: 2 - 1"), std::string::npos);

    mm.deleteAllWrappedMatches(list);
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

    auto* list = mm.getAllMatchesWrapped();
    mm.deleteAllWrappedMatches(list);
}

TEST_F(MatchManagerTest, DisplayWrappedMatchesList)
{
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    auto* list = mm.getAllMatchesWrapped();
    mm.displayWrappedMatchesList(list);

    std::cout.rdbuf(old);
    std::string output = buffer.str();

    EXPECT_NE(output.find("Date: 10/11/2015"), std::string::npos);
    EXPECT_NE(output.find("Date: 26/04/2018"), std::string::npos);
    EXPECT_NE(output.find("Date: 15/07/2027"), std::string::npos);

    mm.deleteAllWrappedMatches(list);
}