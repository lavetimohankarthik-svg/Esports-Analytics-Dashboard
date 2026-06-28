import requests
import pandas as pd
import os

# Replace with your NEW PandaScore API key
API_KEY = "FM7qtrAS8VQbg2I9w4qfKjUOpPSn8WA1FdcmDFs0PLp6svo89Uk"

headers = {
    "Authorization": f"Bearer {API_KEY}"
}

# Fetch 100 Valorant players
url = "https://api.pandascore.co/valorant/players?page[size]=100"

response = requests.get(url, headers=headers)

if response.status_code == 200:
    players = response.json()

    data = []

    for player in players:
        current_team = player.get("current_team")

        data.append({
            "player_id": player.get("id"),
            "player_name": player.get("name"),
            "first_name": player.get("first_name"),
            "last_name": player.get("last_name"),
            "nationality": player.get("nationality"),
            "role": player.get("role"),
            "team_id": current_team.get("id") if current_team else None,
            "team_name": current_team.get("name") if current_team else None
        })

    df = pd.DataFrame(data)

    os.makedirs("csv", exist_ok=True)

    df.to_csv("csv/players.csv", index=False)

    print(f"Successfully fetched {len(df)} players!\n")
    print(df.head(20))

else:
    print("Error:", response.status_code)
    print(response.text)