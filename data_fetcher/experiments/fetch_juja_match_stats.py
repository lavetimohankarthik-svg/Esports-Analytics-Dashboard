import requests
from bs4 import BeautifulSoup
import re

HEADERS = {
    "User-Agent": "Mozilla/5.0"
}

PLAYER_NAME = "juja"

# ==============================
# Search VLR
# ==============================

search_url = f"https://www.vlr.gg/search/?q={PLAYER_NAME}"

response = requests.get(search_url, headers=HEADERS)

print("Search Status:", response.status_code)

if response.status_code != 200:
    exit()

soup = BeautifulSoup(response.text, "html.parser")

profile_url = None

for link in soup.find_all("a", href=True):

    href = link["href"]

    if "/search/r/player/" in href:

        profile_url = "https://www.vlr.gg" + href
        break

if profile_url is None:
    print("Player profile not found.")
    exit()

print("\nProfile URL:")
print(profile_url)


# ==============================
# Open Profile
# ==============================

profile_response = requests.get(
    profile_url,
    headers=HEADERS,
    allow_redirects=True
)

print("\nProfile Status:",
      profile_response.status_code)

print("\nFinal URL:")
print(profile_response.url)

profile_soup = BeautifulSoup(
    profile_response.text,
    "html.parser"
)


# ==============================
# Find Match History Links
# ==============================

match_links = []

for link in profile_soup.find_all("a", href=True):

    href = link["href"]

    # Match URLs look like /123456/team-a-vs-team-b
    if re.match(r"^/\d+/", href):

        full_url = "https://www.vlr.gg" + href

        if full_url not in match_links:
            match_links.append(full_url)

print("\nMatches Found:",
      len(match_links))

for i, link in enumerate(match_links[:5], 1):

    print(f"{i}. {link}")