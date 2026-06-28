import requests
from bs4 import BeautifulSoup

players = [
    "juja",
    "Ash",
    "ruincrest",
    "lovergirl",
    "always"
]

headers = {
    "User-Agent": "Mozilla/5.0"
}

for player in players:
    print(f"\nSearching for: {player}")

    url = f"https://www.vlr.gg/search/?q={player}"

    response = requests.get(url, headers=headers)

    print("Status:", response.status_code)

    if response.status_code == 200:
        print("Page Length:", len(response.text))

        if player.lower() in response.text.lower():
            print("Possible match found!")
        else:
            print("No obvious match.")
    else:
        print("Failed to access.")