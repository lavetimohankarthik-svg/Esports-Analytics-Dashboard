import pandas as pd

df = pd.read_csv("CSV/past_matches_final.csv")

mysql_df = df[
    [
        "match_id",
        "team1_id",
        "team2_id",
        "winner_id",
        "status",
        "begin_at",
        "tournament_id"
    ]
]

mysql_df.to_csv("CSV/matches_mysql.csv", index=False)

print("Created matches_mysql.csv")
print(mysql_df.shape)
print(mysql_df.head())