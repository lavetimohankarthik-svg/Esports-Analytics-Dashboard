import requests
import pandas as pd
import os
import time

API_KEY = "FM7qtrAS8VQbg2I9w4qfKjUOpPSn8WA1FdcmDFs0PLp6svo89Uk"

headers = {
    "Authorization": f"Bearer {API_KEY}"
}

BASE_URL = "https://api.pandascore.co/valorant/tournaments"

data = []
page = 1

while True:
    params = {
        "sort": "-begin_at",
        "page": page,
        "per_page": 100
    }

    response = requests.get(BASE_URL, headers=headers, params=params)

    if response.status_code != 200:
        print("Error:", response.status_code)
        print(response.text)
        break

    tournaments = response.json()

    if not tournaments:
        print("No more tournaments found.")
        break

    print(f"Fetched page {page} ({len(tournaments)} tournaments)")

    for t in tournaments:
        data.append({
            "tournament_id": t.get("id"),
            "name": t.get("name"),
            "tier": t.get("tier"),
            "prizepool": t.get("prizepool"),
            "begin_at": t.get("begin_at"),
            "end_at": t.get("end_at")
        })

    page += 1
    time.sleep(1)   # avoid hitting rate limits

df = pd.DataFrame(data)

df.drop_duplicates(subset=["tournament_id"], inplace=True)

os.makedirs(
    r"C:\Users\lavet\OneDrive\Desktop\dbms\CSV",
    exist_ok=True
)

df.to_csv(
    r"C:\Users\lavet\OneDrive\Desktop\dbms\CSV\tournaments.csv",
    index=False
)

print("\nSuccessfully fetched", len(df), "unique tournaments!")
print(df.head())