-- Create Database
CREATE DATABASE IF NOT EXISTS esports_analytics;

USE esports_analytics;

-- =========================
-- TEAM TABLE
-- =========================
CREATE TABLE Team (
    team_id BIGINT PRIMARY KEY,
    team_name VARCHAR(255) NOT NULL,
    acronym VARCHAR(50),
    location VARCHAR(100)
);

-- =========================
-- PLAYER TABLE
-- =========================
CREATE TABLE Player (
    player_id BIGINT PRIMARY KEY,
    player_name VARCHAR(255) NOT NULL,
    first_name VARCHAR(255),
    last_name VARCHAR(255),
    nationality VARCHAR(100),
    role VARCHAR(100),
    team_id BIGINT,

    FOREIGN KEY (team_id)
        REFERENCES Team(team_id)
        ON DELETE SET NULL
);

-- =========================
-- LEAGUE TABLE
-- =========================

CREATE TABLE league (
    league_id BIGINT PRIMARY KEY,
    league_name VARCHAR(255) NOT NULL
);
-- =========================
-- TOURNAMENT TABLE
-- =========================
CREATE TABLE Tournament (
    tournament_id BIGINT PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    tier VARCHAR(20),
    prizepool VARCHAR(255),
    begin_at DATETIME,
    end_at DATETIME,
    league_id BIGINT,

    FOREIGN KEY (league_id)
        REFERENCES league(league_id)
);

-- =========================
-- MATCH TABLE
-- =========================
CREATE TABLE Match_Info (
    match_id BIGINT PRIMARY KEY,

    team1 VARCHAR(255),
    team2 VARCHAR(255),

    team1_id BIGINT NOT NULL,
    team2_id BIGINT NOT NULL,

    winner_id BIGINT,

    status VARCHAR(50),
    begin_at DATETIME,

    tournament_id BIGINT, 

    FOREIGN KEY (team1_id)
        REFERENCES Team(team_id),

    FOREIGN KEY (team2_id)
        REFERENCES Team(team_id),

    FOREIGN KEY (winner_id)
        REFERENCES Team(team_id),

    FOREIGN KEY (tournament_id)
        REFERENCES tournament(tournament_id)
);

-- =========================
-- TOURNAMENT_LEAGUE_MAP TABLE
-- =========================

CREATE TABLE tournament_league_map (
    tournament_id BIGINT PRIMARY KEY,
    league_id BIGINT,
    
    FOREIGN KEY (tournament_id)
        REFERENCES tournament(tournament_id),

    FOREIGN KEY (league_id)
        REFERENCES league(league_id)
);

-- =========================
-- VLR_PLAYER_STATS TABLE
-- =========================

CREATE TABLE vlr_player_stats (
    vlr_match_id BIGINT PRIMARY KEY,
    match_date DATE,

    vlr_player_id BIGINT ,

    player_name VARCHAR(255),

    team_tag VARCHAR(50),

    rating FLOAT,
    acs FLOAT,

    kills INT,
    deaths INT,
    assists INT,

    kast FLOAT,
    adr FLOAT,

    hs_percentage FLOAT,

    fk INT,
    fd INT
);