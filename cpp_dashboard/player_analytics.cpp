#include "player_analytics.h"

#include <iostream>
#include <iomanip>
#include <string>

using namespace std;


//-------------------------------------------Player Analytics----------------------------------------------

void playerAnalytics(MYSQL* conn)
{
    string playerName;
    int year;

    cout << "\n========== PLAYER ANALYTICS ==========\n";

    cout << "Enter Year: ";
    cin >> year;

    cin.ignore();

    cout << "Enter Player Name: ";
    getline(cin, playerName);

    string nationality = "-";

    string playerQuery =
        "SELECT nationality "
        "FROM player "
        "WHERE LOWER(player_name)=LOWER('" + playerName + "') "
        "LIMIT 1";

    if(mysql_query(conn, playerQuery.c_str()))
    {
        cout << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(res);

    if(row && row[0])
        nationality = row[0];

    mysql_free_result(res);



    // CURRENT TEAM (LATEST TEAM IN THAT YEAR)

    string currentTeam = "-";

    string latestTeamQuery =
        "SELECT team_tag "
        "FROM vlr_player_stats "
        "WHERE LOWER(player_name)=LOWER('" + playerName + "') "
        "AND YEAR(match_date)=" + to_string(year) + " "
        "ORDER BY match_date DESC "
        "LIMIT 1";

    if(mysql_query(conn, latestTeamQuery.c_str()))
    {
        cout << mysql_error(conn) << endl;
        return;
    }

    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);

    if(row && row[0])
        currentTeam = row[0];

    mysql_free_result(res);



    // OVERALL STATS

    string statsQuery =
        "SELECT "
        "COUNT(DISTINCT vlr_match_id), "
        "SUM(kills), "
        "SUM(assists), "
        "ROUND(SUM(kills)/NULLIF(SUM(deaths),0),2), "
        "ROUND(AVG(rating),2), "
        "ROUND(AVG(hs_percentage),2), "
        "ROUND(AVG(acs),2) "
        "FROM vlr_player_stats "
        "WHERE LOWER(player_name)=LOWER('" + playerName + "') "
        "AND YEAR(match_date)=" + to_string(year);

    if(mysql_query(conn, statsQuery.c_str()))
    {
        cout << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES* statsRes = mysql_store_result(conn);
    MYSQL_ROW statsRow = mysql_fetch_row(statsRes);

    if(!statsRow || !statsRow[0])
    {
        cout << "\nNo data found.\n";
        mysql_free_result(statsRes);
        return;
    }



    cout << "\n=========================================\n";
    cout << "Player       : " << playerName << endl;
    cout << "Nationality  : " << nationality << endl;
    cout << "Current Team : " << currentTeam << endl;



    // TEAM HISTORY TABLE

    string historyQuery =
        "SELECT "
        "team_tag, "
        "COUNT(DISTINCT vlr_match_id) AS matches "
        "FROM vlr_player_stats "
        "WHERE LOWER(player_name)=LOWER('" + playerName + "') "
        "AND YEAR(match_date)=" + to_string(year) + " "
        "GROUP BY team_tag "
        "ORDER BY MIN(match_date)";

    if(mysql_query(conn, historyQuery.c_str()))
    {
        cout << mysql_error(conn) << endl;
        mysql_free_result(statsRes);
        return;
    }

    MYSQL_RES* historyRes = mysql_store_result(conn);

    cout << "\nTeam History (" << year << ")\n";
    cout << "+----------------------+----------------+\n";
    cout << "| Team Name            | Matches Played |\n";
    cout << "+----------------------+----------------+\n";

    while((row = mysql_fetch_row(historyRes)))
    {
        cout << "| "
             << left << setw(20) << row[0]
             << " | "
             << right << setw(14) << row[1]
             << " |\n";
    }

    cout << "+----------------------+----------------+\n";

    mysql_free_result(historyRes);



    cout << "\nOverall Performance (" << year << ")\n";
    cout << "-----------------------------------------\n";

    cout << "Total Matches      : " << statsRow[0] << endl;
    cout << "Total Kills        : " << statsRow[1] << endl;
    cout << "Total Assists      : " << statsRow[2] << endl;
    cout << "K/D Ratio          : " << statsRow[3] << endl;
    cout << "Avg Rating         : " << statsRow[4] << " (MAX - 2)" << endl;
    cout << "Headshot Rate      : " << statsRow[5] << "%" << endl;
    cout << "Avg ACS            : " << statsRow[6] << endl;

    cout << "=========================================\n";

    mysql_free_result(statsRes);
}