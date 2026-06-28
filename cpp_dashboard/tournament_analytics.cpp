#include "tournament_analytics.h"

#include <iostream>
#include <cstring>
#include <set>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;



//-------------------------------------------Tournament Analytics----------------------------------------------

void tournamentAnalytics(MYSQL* conn) {

struct TeamStats {
    long long teamId;
    string teamName;
    string acronym;
    int titles = 0;
    int matchWins = 0;
};

    int year;
    string tournamentInput;

    cout << "\nEnter Year: ";
    cin >> year;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter Tournament Name: ";
    getline(cin, tournamentInput);

    MYSQL_RES* res;
    MYSQL_ROW row;

    long long tournamentId;

    string tournamentName;
    string prizePool = "-";
    string leagueName = "Unknown";

    // =====================================
    // Find Tournament
    // =====================================

    string query =
        "SELECT tournament_id, name, prizepool, league_id "
        "FROM tournament "
        "WHERE LOWER(name) LIKE LOWER('%" +
        tournamentInput +
        "%') "
        "AND YEAR(begin_at)=" +
        to_string(year);

    if(mysql_query(conn, query.c_str())) {

        cout << mysql_error(conn) << endl;
        return;
    }

    res = mysql_store_result(conn);

    row = mysql_fetch_row(res);

    if(row == NULL) {

        cout << "\nTournament not found!\n";

        mysql_free_result(res);

        cout << "\nPress Enter to continue...";
        cin.get();

        return;
    }

    tournamentId = atoll(row[0]);

    tournamentName = row[1];

    if(row[2] != NULL &&
        strlen(row[2]) > 0) {

        prizePool = row[2];
    }

    long long leagueId = atoll(row[3]);

    mysql_free_result(res);

    // =====================================
    // League Name
    // =====================================

    query =
        "SELECT league_name "
        "FROM league "
        "WHERE league_id=" +
        to_string(leagueId);

    mysql_query(conn, query.c_str());

    res = mysql_store_result(conn);

    row = mysql_fetch_row(res);

    if(row != NULL)
        leagueName = row[0];

    mysql_free_result(res);

    // =====================================
    // Total Matches
    // =====================================

    int totalMatches = 0;

    query =
        "SELECT COUNT(*) "
        "FROM Match_Info "
        "WHERE tournament_id=" +
        to_string(tournamentId);

    mysql_query(conn, query.c_str());

    res = mysql_store_result(conn);

    row = mysql_fetch_row(res);

    totalMatches = atoi(row[0]);

    mysql_free_result(res);

    // =====================================
    // Participating Teams
    // =====================================

    set<long long> participatingTeams;

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

    // =====================================
    // Top Teams
    // =====================================

    vector<TeamStats> teams;

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

        TeamStats t;

        t.teamId = atoll(row[0]);

        t.matchWins = atoi(row[1]);

        teams.push_back(t);
    }

    mysql_free_result(res);

    // =====================================
    // Fetch Team Details
    // =====================================

    for(auto &t : teams) {

        query =
            "SELECT team_name, acronym "
            "FROM team "
            "WHERE team_id=" +
            to_string(t.teamId);

        mysql_query(conn, query.c_str());

        res = mysql_store_result(conn);

        row = mysql_fetch_row(res);

        if(row != NULL) {

            t.teamName = row[0];

            if(row[1] != NULL)
                t.acronym = row[1];
            else
                t.acronym = "-";
        }

        mysql_free_result(res);
    }

    // =====================================
    // Sort Teams
    // =====================================

    sort(
        teams.begin(),
        teams.end(),

        [](const TeamStats& a,
           const TeamStats& b) {

            return a.matchWins >
                   b.matchWins;
        }
    );

    // =====================================
    // Champion
    // =====================================

    string champion = "N/A";
    string championAcronym = "-";

    int championWins = 0;

    if(!teams.empty()) {

        champion = teams[0].teamName;

        championAcronym = teams[0].acronym;

        championWins = teams[0].matchWins;
    }

    // =====================================
    // Display
    // =====================================

    cout << "\n============================================\n";
    cout << "TOURNAMENT ANALYTICS\n";
    cout << "============================================\n";

    cout << "Tournament          : "
         << tournamentName << endl;

    cout << "League              : "
         << leagueName << endl;

    cout << "Year                : "
         << year << endl;

    cout << "Prize Pool          : "
         << prizePool << endl;

    cout << "\nTotal Matches       : "
         << totalMatches << endl;

    cout << "Participating Teams : "
         << participatingTeams.size() << endl;

    cout << "\nTournament Champion : "
         << champion
         << " ("
         << championAcronym
         << ")"
         << endl;

    cout << "Champion Wins       : "
         << championWins
         << endl;

    cout << "\nTop 5 Teams\n";

    cout << "--------------------------------------------------\n";

    cout << left
         << setw(6)  << "Rank"
         << setw(30) << "Team"
         << setw(15) << "Match Wins"
         << endl;

    cout << "--------------------------------------------------\n";

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
             << setw(30) << displayName
             << setw(15) << teams[i].matchWins
             << endl;
    }

    cout << "--------------------------------------------------\n";

    cout << "\nPress Enter to continue...";
    cin.get();
}


