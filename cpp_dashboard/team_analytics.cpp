#include "team_analytics.h"

#include <iostream>
#include <iomanip>

using namespace std;



//-------------------------------------------Team Analytics----------------------------------------------

void teamAnalytics(MYSQL* conn) {

    int year;
    string teamName;

    cout << "\nEnter Year: ";
    cin >> year;

    cin.ignore();

    cout << "Enter Team Name: ";
    getline(cin, teamName);

    // ==============================
    // Find Team ID
    // ==============================

   string teamQuery =
    "SELECT team_id, location, acronym "
    "FROM team "
    "WHERE team_name='" + teamName + "'";



if (mysql_query(conn, teamQuery.c_str())) {
    cout << "Query Error: " << mysql_error(conn) << endl;
    return;
}

MYSQL_RES* res = mysql_store_result(conn);

if (res == NULL) {
    cout << "Store Result Error: " << mysql_error(conn) << endl;
    return;
}

MYSQL_ROW row = mysql_fetch_row(res);

if (row == NULL) {
    cout << "\nTeam not found!\n";
    mysql_free_result(res);
    return;
}



long long teamId = atoll(row[0]);

string location = "Unknown";

if (row[1] != NULL) {
    location = row[1];
}

string acronym = "-";

if (row[2] != NULL){
    acronym = row[2];
}

mysql_free_result(res);



    // ==============================
    // Matches Played
    // ==============================

    int matchesPlayed = 0;

    string matchQuery =
        "SELECT COUNT(*) "
        "FROM Match_Info "
        "WHERE YEAR(begin_at)=" + to_string(year) +
        " AND (team1_id=" + to_string(teamId) +
        " OR team2_id=" + to_string(teamId) + ")";

    mysql_query(conn, matchQuery.c_str());

    res = mysql_store_result(conn);

    row = mysql_fetch_row(res);

    matchesPlayed = atoi(row[0]);

    mysql_free_result(res);

    // ==============================
    // Wins
    // ==============================

    int wins = 0;

    string winQuery =
        "SELECT COUNT(*) "
        "FROM Match_Info "
        "WHERE YEAR(begin_at)=" + to_string(year) +
        " AND winner_id=" + to_string(teamId);

    mysql_query(conn, winQuery.c_str());

    res = mysql_store_result(conn);

    row = mysql_fetch_row(res);

    wins = atoi(row[0]);

    mysql_free_result(res);

    // ==============================
    // Losses
    // ==============================

    int losses = matchesPlayed - wins;

    // ==============================
    // Win Percentage
    // ==============================

    double winPercentage = 0;

    if (matchesPlayed > 0) {
        winPercentage =
            (wins * 100.0) / matchesPlayed;
    }
    
    // ==============================
    // Tournaments Participated
    // ==============================

    int tournamentsPlayed = 0;

    string tournamentQuery =
        "SELECT COUNT(DISTINCT tournament_id) "
        "FROM Match_Info "
        "WHERE YEAR(begin_at)=" + to_string(year) +
        " AND (team1_id=" + to_string(teamId) +
        " OR team2_id=" + to_string(teamId) + ")";

    mysql_query(conn, tournamentQuery.c_str());

    res = mysql_store_result(conn);

    row = mysql_fetch_row(res);

    tournamentsPlayed = atoi(row[0]);

    mysql_free_result(res);

    // ==============================
    // Display Basic Analytics
    // ==============================

    cout << "\n====================================\n";
    cout << "TEAM ANALYTICS\n";
    cout << "====================================\n";

    cout << "Team                  : "
     << teamName
     << " ("
     << acronym
     << ")"
     << endl;

    cout << "Location              : "
         << location << endl;

    cout << "Year                  : "
         << year << endl;

    cout << "\nMatches Played        : "
         << matchesPlayed << endl;

    cout << "Wins                  : "
         << wins << endl;

    cout << "Losses                : "
         << losses << endl;

    cout << "Win Percentage        : "
         << fixed
         << setprecision(2)
         << winPercentage
         << "%" << endl;

    cout << "Tournaments Played    : "
         << tournamentsPlayed << endl;

    // ==============================
    // Last 5 Matches
    // ==============================

    cout << "\nLast 5 Matches        : ";

    string last5Query =
        "SELECT winner_id "
        "FROM Match_Info "
        "WHERE YEAR(begin_at)=" + to_string(year) +
        " AND (team1_id=" + to_string(teamId) +
        " OR team2_id=" + to_string(teamId) + ") "
        "ORDER BY begin_at DESC "
        "LIMIT 5";

    mysql_query(conn, last5Query.c_str());

    res = mysql_store_result(conn);

    while ((row = mysql_fetch_row(res))) {

        long long winnerId = atoll(row[0]);

        if (winnerId == teamId)
            cout << "W ";
        else
            cout << "L ";
    }

    mysql_free_result(res);

    // ==============================
    // Titles Won
    // ==============================

    cout << "\n\nTitles Won:\n";

    string titleQuery =
        "SELECT DISTINCT t.tournament_name "
        "FROM Tournament t "
        "JOIN Match_Info m "
        "ON t.tournament_id = m.tournament_id "
        "WHERE YEAR(m.begin_at)=" + to_string(year) +
        " AND m.winner_id=" + to_string(teamId);

    mysql_query(conn, titleQuery.c_str());

    res = mysql_store_result(conn);

    bool found = false;

    while ((row = mysql_fetch_row(res))) {

        found = true;

        cout << "- "
             << row[0]
             << " (Winner)"
             << endl;
    }

    if (!found) {
        cout << "No titles won.\n";
    }

    mysql_free_result(res);

    cout << "\nPress Enter to continue...";
    cin.get();
}


struct TeamStats {
    long long teamId;
    string teamName;
    string acronym;
    int titles = 0;
    int matchWins = 0;
};