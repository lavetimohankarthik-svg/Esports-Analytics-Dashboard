import pandas as pd

# Load CSVs
teams = pd.read_csv("csv/teams.csv")
matches = pd.read_csv("csv/past_matches_v2.csv")

# Create lookup dictionary
team_lookup = dict(zip(teams["team_name"], teams["team_id"]))

# Map names to IDs
matches["team1_id"] = matches["team1"].map(team_lookup)
matches["team2_id"] = matches["team2"].map(team_lookup)

# Save
matches.to_csv("csv/past_matches_final.csv", index=False)

print(matches.head())
print("\nMissing Team1 IDs:", matches["team1_id"].isna().sum())
print("Missing Team2 IDs:", matches["team2_id"].isna().sum())