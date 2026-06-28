import requests
import pandas as pd
import os

API_KEY = "FM7qtrAS8VQbg2I9w4qfKjUOpPSn8WA1FdcmDFs0PLp6svo89Uk"

headers = {
    "Authorization": f"Bearer {API_KEY}"
}

url = "https://api.pandascore.co/valorant/matches?sort=-begin_at&page[size]=100"

response = requests.get(url, headers=headers)

if response.status_code == 200:
    matches = response.json()

    data = []

    for match in matches:

        opponents = match.get("opponents", [])

        team1 = None
        team2 = None

        if len(opponents) >= 1:
            team1 = opponents[0]["opponent"].get("name")

        if len(opponents) >= 2:
            team2 = opponents[1]["opponent"].get("name")

        data.append({
            "match_id": match.get("id"),
            "match_name": match.get("name"),
            "tournament": (
                match.get("tournament") or {}
            ).get("name"),
            "team1": team1,
            "team2": team2,
            "winner_id": match.get("winner_id"),
            "begin_at": match.get("begin_at"),
            "status": match.get("status")
        })

    df = pd.DataFrame(data)

    os.makedirs("csv", exist_ok=True)

    df.to_csv("csv/matches.csv", index=False)

    print(f"Successfully fetched {len(df)} matches!\n")

    print(df.head(20))

else:
    print(response.status_code)
    print(response.text)