#include "league_analytics.h"

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <iomanip>
#include <algorithm>

using namespace std;


//-------------------------------------------League Analytics----------------------------------------------

void leagueAnalytics(MYSQL* conn) {

struct TeamStats {
    long long teamId;
    string teamName;
    string acronym;
    int titles = 0;
    int matchWins = 0;
};

    int year;
    string leagueInput;

    cout << "\nEnter Year: ";
    cin >> year;

    cin.ignore();

    cout << "Enter League Name: ";
    getline(cin, leagueInput);

    MYSQL_RES* res;
    MYSQL_ROW row;

    // =====================================
    // Find League (Case Insensitive)
    // =====================================

    string query =
        "SELECT league_id, league_name "
        "FROM league "
        "WHERE LOWER(league_name) LIKE LOWER('%" +
        leagueInput + "%')";

    if(mysql_query(conn, query.c_str())) {
        cout << mysql_error(conn) << endl;
        return;
    }

    res = mysql_store_result(conn);

    row = mysql_fetch_row(res);

    if(row == NULL) {

        cout << "\nLeague not found!\n";

        mysql_free_result(res);

        cout << "\nPress Enter to continue...";
        cin.get();

        return;
    }

    long long leagueId = atoll(row[0]);

    string leagueName = row[1];

    mysql_free_result(res);

    // =====================================
    // Find tournaments of that league/year
    // =====================================

    vector<long long> tournaments;

    query =
        "SELECT tournament_id "
        "FROM tournament "
        "WHERE league_id=" +
        to_string(leagueId) +
        " AND YEAR(begin_at)=" +
        to_string(year);

    mysql_query(conn, query.c_str());

    res = mysql_store_result(conn);

    while((row = mysql_fetch_row(res))) {

        tournaments.push_back(
            atoll(row[0])
        );
    }

    mysql_free_result(res);

    if(tournaments.empty()) {

        cout << "\nNo tournaments found for "
             << leagueName
             << " in "
             << year
             << ".\n";

        cout << "\nPress Enter to continue...";
        cin.get();

        return;
    }
    // =====================================
    // League Summary Statistics
    // =====================================

    int totalTournaments = tournaments.size();

    int totalMatches = 0;

    set<long long> participatingTeams;

    // =====================================
    // Calculate Titles and Match Wins
    // =====================================

    map<long long, TeamStats> stats;

    for(long long tournamentId : tournaments) {

            // Total Matches

        query =
            "SELECT COUNT(*) "
            "FROM Match_Info "
            "WHERE tournament_id=" +
            to_string(tournamentId);

        mysql_query(conn, query.c_str());

        res = mysql_store_result(conn);

        row = mysql_fetch_row(res);

        if(row != NULL) {

            totalMatches += atoi(row[0]);
        }

        mysql_free_result(res);

            // Participating Teams

        query =
            "SELECT team1_id, team2_id "
            "FROM Match_Info "
            "WHERE tournament_id=" +
            to_string(tournamentId);

        mysql_query(conn, query.c_str());

        res = mysql_store_result(conn);

        while((row = mysql_fetch_row(res))) {

            participatingTeams.insert(
                atoll(row[0])
            );

            participatingTeams.insert(
                atoll(row[1])
            );
        }

        mysql_free_result(res);

        // Champion = Team with maximum match wins

        query =
            "SELECT winner_id, COUNT(*) AS wins "
            "FROM Match_Info "
            "WHERE tournament_id=" +
            to_string(tournamentId) +
            " AND winner_id IS NOT NULL "
            "GROUP BY winner_id "
            "ORDER BY wins DESC "
            "LIMIT 1";

        mysql_query(conn, query.c_str());

        res = mysql_store_result(conn);

        row = mysql_fetch_row(res);

        if(row != NULL) {

            long long champion = atoll(row[0]);

            stats[champion].teamId = champion;

            stats[champion].titles++;
        }

        mysql_free_result(res);

        // Match Wins

        query =
            "SELECT winner_id, COUNT(*) "
            "FROM Match_Info "
            "WHERE tournament_id=" +
            to_string(tournamentId) +
            " AND winner_id IS NOT NULL "
            "GROUP BY winner_id";

        mysql_query(conn, query.c_str());

        res = mysql_store_result(conn);

        while((row = mysql_fetch_row(res))) {

            long long teamId =
                atoll(row[0]);

            int wins =
                atoi(row[1]);

            stats[teamId].teamId =
                teamId;

            stats[teamId].matchWins +=
                wins;
        }

        mysql_free_result(res);
    }

    // =====================================
    // Fetch Team Details
    // =====================================

    vector<TeamStats> teams;

    for(auto &entry : stats) {

        TeamStats t =
            entry.second;

        query =
            "SELECT team_name, acronym "
            "FROM team "
            "WHERE team_id=" +
            to_string(t.teamId);

        mysql_query(conn, query.c_str());

        res = mysql_store_result(conn);

        row = mysql_fetch_row(res);

        if(row != NULL) {

            t.teamName =
                row[0];

            if(row[1] != NULL)
                t.acronym =
                    row[1];
            else
                t.acronym =
                    "-";
        }

        mysql_free_result(res);

        teams.push_back(t);
    }

    // =====================================
    // Sort
    // =====================================

    sort(teams.begin(), teams.end(),
            [](const TeamStats& a, const TeamStats& b) {

        if (a.matchWins != b.matchWins)
            return a.matchWins > b.matchWins;

        return a.teamName < b.teamName;
    });

    string leagueWinner = "N/A";

    string leagueWinnerAcronym = "-";

    if(!teams.empty()) {

        leagueWinner =
            teams[0].teamName;

        leagueWinnerAcronym =
            teams[0].acronym;
    }

    // =====================================
    // Display
    // =====================================

    cout << "\n============================================\n";
    cout << "LEAGUE ANALYTICS\n";
    cout << "============================================\n";

    cout << "League : "
         << leagueName
         << endl;

    cout << "Year   : "
         << year
         << endl;

    cout << "\nLeague Winner       : "
        << leagueWinner
        << " ("
        << leagueWinnerAcronym
        << ")"
        << endl;

    cout << "Total Matches       : "
        << totalMatches
        << endl;

    cout << "Total Tournaments   : "
        << totalTournaments
        << endl;

    cout << "Participating Teams : "
        << participatingTeams.size()
        << endl;
        
    cout << "\nTop 5 Teams\n";

    cout << "-----------------------------------------------------------\n";

    cout << left
         << setw(6)  << "Rank"
         << setw(28) << "Team"
         << setw(10) << "Titles"
         << setw(15) << "Match Wins"
         << endl;

    cout << "-----------------------------------------------------------\n";

    for(int i = 0;
        i < teams.size() && i < 5;
        i++) {

        string displayName =
            teams[i].teamName +
            " (" +
            teams[i].acronym +
            ")";

        cout << left
             << setw(6)  << i + 1
             << setw(28) << displayName
             << setw(10) << teams[i].titles
             << setw(15) << teams[i].matchWins
             << endl;
    }

    cout << "-----------------------------------------------------------\n";

    cout << "\nPress Enter to continue...";
    cin.get();
}
