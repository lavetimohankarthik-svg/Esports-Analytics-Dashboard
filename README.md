# 🎮 Esports Analytics Dashboard

A DBMS project developed using **C++**, **MySQL**, and **Python** for collecting, storing, and analyzing Valorant esports data.

The project fetches data from the **PandaScore API** and **VLR.gg**, stores it in a relational database, and provides an interactive analytics dashboard for Teams, Leagues, Tournaments, and Players.

---

#  Features

## Team Analytics

* Team Name(acronym)
* Location
* Year
* Matches played
* Matches won
* Matches Lost
* Win percentage
* Tournament participated
* Last 5 Matches Result
* Tournament titles won

## League Analytics

* League Name
* Year
* League Winner
* Number of Matches in a league
* Number of tournaments in a league
* Number of Participating teams
* Top-5 teams in that League

## Tournament Analytics

* Tournament details
* Prize pool information
* Participating teams
* Winning teams
* Tournament statistics

## Player Analytics

* Player profile information
* Nationality
* Current team
* Team history
* Matches played
* Total kills
* Total assists
* K/D ratio
* Average Rating
* Headshot Rate
* Average Combat Score (ACS)

---

#  Technologies Used

### Languages

* C++
* Python
* SQL

### Database

* MySQL

### Libraries

* MySQL Connector/C
* Pandas
* Requests
* BeautifulSoup4

### Tools

* VS Code
* MySQL Workbench

### Data Sources

* PandaScore API
* VLR.gg





---

#  Entity Relationship Diagram

```text
TEAM (1) --------------------< PLAYER (M)

LEAGUE (1) ------------------< TOURNAMENT (M)

TOURNAMENT (1) -------------< MATCH_INFO (M)

TEAM (1) -------------------< MATCH_INFO (M)
            (team1_id)

TEAM (1) -------------------< MATCH_INFO (M)
            (team2_id)

TEAM (1) -------------------< MATCH_INFO (M)
            (winner_id)

TOURNAMENT (1) -----------< TOURNAMENT_LEAGUE_MAP (M)

LEAGUE (1) ---------------< TOURNAMENT_LEAGUE_MAP (M)

PLAYER (1) ----------------< VLR_PLAYER_STATS (M)

MATCH_INFO (1) -----------< VLR_PLAYER_STATS (M)
```

---



---

#  Data Collection Workflow

1. Fetch leagues using PandaScore API.
2. Fetch tournaments and League Mappings.
3. Fetch teams Information.
4. Fetch players Information.
5. Fetch matches Information.
6. Store these data in their respective CSV files.
7. Import generated CSV files into their corresponding MySQL tables.
8. Scrape player match statistics from VLR.gg.
9. Store its data in a CSV file.
10. Import player match statistics CSV into MySQL.
11. Connect the C++ dashboard to MySQL using MariaDB Connector/C.
12. Run the dashboard.

---

#  Running the Project

### 1. Create Database

```sql
SOURCE schema.sql;
```

### 2. Import CSV Files

Import all generated CSV files into their respective MySQL tables.

### 3. Build Dashboard

```bash
g++ main.cpp team_analytics.cpp league_analytics.cpp tournament_analytics.cpp player_analytics.cpp -I"C:\Program Files\MariaDB\MariaDB Connector C 64-bit\include" -L"C:\Program Files\MariaDB\MariaDB Connector C 64-bit\lib" -lmariadb -o dashboard.exe

```

### 4. Run Dashboard

```bash
dashboard.exe
```

---

#  Dashboard Menu

```text
=========================================
      ESPORTS ANALYTICS DASHBOARD
=========================================
1. Team Analytics
2. League Analytics
3. Tournament Analytics
4. Player Analytics
5. Exit
=========================================
```

---

# Outputs:
