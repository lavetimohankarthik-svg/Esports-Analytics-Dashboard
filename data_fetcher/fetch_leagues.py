import requests
import pandas as pd

API_KEY = "FM7qtrAS8VQbg2I9w4qfKjUOpPSn8WA1FdcmDFs0PLp6svo89Uk"

url = "https://api.pandascore.co/valorant/leagues"

headers = {
    "Authorization": f"Bearer {API_KEY}"
}

all_leagues = []
page = 1

while True:
    params = {
        "page": page,
        "per_page": 100
    }

    response = requests.get(url, headers=headers, params=params)

    if response.status_code != 200:
        print("Error:", response.status_code)
        break

    data = response.json()

    if not data:
        break

    for league in data:
        all_leagues.append({
            "league_id": league.get("id"),
            "league_name": league.get("name")
        })

    print(f"Fetched page {page}")

    page += 1

df = pd.DataFrame(all_leagues)

df.drop_duplicates(subset=["league_id"], inplace=True)

df.to_csv(r"C:\Users\lavet\OneDrive\Desktop\dbms\CSV\leagues.csv", index=False)

print("Saved", len(df), "leagues")