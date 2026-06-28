import requests
import pandas as pd
import time

API_KEY = "FM7qtrAS8VQbg2I9w4qfKjUOpPSn8WA1FdcmDFs0PLp6svo89Uk"

headers = {
    "Authorization": f"Bearer {API_KEY}"
}

BASE_URL = "https://api.pandascore.co/valorant/tournaments"

records = []

page = 1

while True:
    params = {
        "page": page,
        "per_page": 100
    }

    response = requests.get(BASE_URL, headers=headers, params=params)

    if response.status_code != 200:
        print("Error:", response.status_code)
        break

    tournaments = response.json()

    if not tournaments:
        break

    print(f"Fetched page {page}")

    for t in tournaments:
        league = t.get("league")

        if league:
            records.append({
                "tournament_id": t["id"],
                "league_id": league["id"]
            })

    page += 1
    time.sleep(1)

df = pd.DataFrame(records)

df.drop_duplicates(inplace=True)

df.to_csv(
    r"C:\Users\lavet\OneDrive\Desktop\dbms\CSV\tournament_leagues.csv",
    index=False
)

print(f"Saved {len(df)} mappings")