import requests
import pandas as pd
import os
import time

API_KEY = "FM7qtrAS8VQbg2I9w4qfKjUOpPSn8WA1FdcmDFs0PLp6svo89Uk"

headers = {
    "Authorization": f"Bearer {API_KEY}"
}

all_teams = []
page = 1

while True:
    url = f"https://api.pandascore.co/valorant/teams?page[size]=100&page[number]={page}"

    response = requests.get(url, headers=headers)

    if response.status_code != 200:
        print("Error:", response.status_code)
        break

    teams = response.json()

    if len(teams) == 0:
        break

    print(f"Fetched page {page}: {len(teams)} teams")

    for team in teams:
        all_teams.append({
            "team_id": team.get("id"),
            "team_name": team.get("name"),
            "acronym": team.get("acronym"),
            "location": team.get("location")
        })

    page += 1

    time.sleep(1)   # be polite to API

df = pd.DataFrame(all_teams)

df.drop_duplicates(subset=["team_id"], inplace=True)

os.makedirs("csv", exist_ok=True)

df.to_csv("csv/teams.csv", index=False)

print("\nTotal Teams:", len(df))