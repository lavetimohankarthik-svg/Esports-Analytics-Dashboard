#include <iostream>
#include <mysql.h>

#include "team_analytics.h"
#include "league_analytics.h"
#include "tournament_analytics.h"
#include "player_analytics.h"

using namespace std;



void showMenu() {
    cout << "\n=========================================\n";
    cout << "      ESPORTS ANALYTICS DASHBOARD\n";
    cout << "=========================================\n";
    cout << "1. Team Analytics\n";
    cout << "2. League Analytics\n";
    cout << "3. Tournament Analytics\n";
    cout << "4. Player Analytics\n";
    cout << "5. Exit\n";
    cout << "=========================================\n";
    cout << "Enter Choice: ";
}

int main() {

    MYSQL *conn = mysql_init(NULL);

    conn = mysql_real_connect(
        conn,
        "localhost",
        "root",
        "lmk@1411",
        "esports_analytics",
        3306,
        NULL,
        0
    );

    if (conn == NULL) {
        cout << "Database Connection Failed!\n";
        cout << mysql_error(conn) << endl;
        return 1;
    }

    cout << "Connected to esports_analytics successfully!\n";

    int choice;

    do {
        showMenu();
        cin >> choice;

        switch(choice) {

            case 1:
                teamAnalytics(conn);
                break;

            case 2:
                leagueAnalytics(conn);
                break;

            case 3:
                tournamentAnalytics(conn);
                break;

            case 4:
                playerAnalytics(conn);
                break;

            case 5:
                cout << "\nExiting Dashboard...\n";
                break;

            default:
                cout << "\nInvalid Choice!\n";
        }

    } while(choice != 5);

    mysql_close(conn);

    return 0;
}

