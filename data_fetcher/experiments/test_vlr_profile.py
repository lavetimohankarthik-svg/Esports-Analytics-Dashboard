import requests
from bs4 import BeautifulSoup

player = "juja"

headers = {
    "User-Agent": "Mozilla/5.0"
}

search_url = f"https://www.vlr.gg/search/?q={player}"

response = requests.get(search_url, headers=headers)

print("Search Status:", response.status_code)

soup = BeautifulSoup(response.text, "html.parser")

links = soup.find_all("a", href=True)

found = False

for link in links:
    href = link["href"]

    # VLR player profiles look like /player/12345/name
    if "/player/" in href:
        print("Profile Found:")
        print("https://www.vlr.gg" + href)
        found = True
        break

if not found:
    print("No player profile found.")