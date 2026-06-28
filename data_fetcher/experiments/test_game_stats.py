import requests
import json

API_KEY = "FM7qtrAS8VQbg2I9w4qfKjUOpPSn8WA1FdcmDFs0PLp6svo89Uk"

headers = {
    "Authorization": f"Bearer {API_KEY}"
}

game_id = 792673

url = f"https://api.pandascore.co/valorant/games/{game_id}"

response = requests.get(url, headers=headers)

print("Status Code:", response.status_code)

if response.status_code == 200:
    data = response.json()

    print(json.dumps(data, indent=2)[:10000])

else:
    print(response.text)