import requests
import pandas as pd
from bs4 import BeautifulSoup
import os
import re
import time


HEADERS = {
    "User-Agent": "Mozilla/5.0"
}

players_df = pd.read_csv("csv/players.csv")

all_stats = []

old_df = pd.DataFrame()

if os.path.exists("csv/vlr_player_stats.csv"):
    old_df = pd.read_csv("csv/vlr_player_stats.csv")
    print("Existing rows:", len(old_df))
visited_matches = set()

os.makedirs("csv", exist_ok=True)


def get_vlr_profile(player_name):
    try:
        url = f"https://www.vlr.gg/search/?q={player_name}"

        r = requests.get(url, headers=HEADERS)

        if r.status_code != 200:
            return None

        match = re.search(r'/player/(\d+)/([^"]+)', r.text)

        if match:
            player_id = match.group(1)
            slug = match.group(2)

            return f"https://www.vlr.gg/player/matches/{player_id}/{slug}"

    except:
        pass

    return None


def get_recent_matches(profile_url):

    matches = []

    try:

        for page in range(1, 11):

            page_url = profile_url + f"?page={page}"

            r = requests.get(page_url, headers=HEADERS)

            if r.status_code != 200:
                break

            soup = BeautifulSoup(r.text, "html.parser")

            page_matches = 0

            for a in soup.find_all("a", href=True):

                href = a["href"]

                if re.match(r"^/\d+/.+-vs-.+", href):

                    full = "https://www.vlr.gg" + href

                    if full not in matches:
                        matches.append(full)
                        page_matches += 1

                if len(matches) >= 50:
                    return matches[:50]

            if page_matches == 0:
                break

        return matches[:50]

    except:
        return []


def parse_match(match_url):

    if "-vs-" not in match_url:
        return
    
    try:
        r = requests.get(match_url, headers=HEADERS)

        if r.status_code != 200:
            return

        soup = BeautifulSoup(r.text, "html.parser")

        match_id_match = re.search(r"/(\d+)/", match_url)

        if not match_id_match:
            return

        vlr_match_id = match_id_match.group(1)

        date_div = soup.find("div", {"data-utc-ts": True})

        match_date = None

        if date_div:
            match_date = date_div["data-utc-ts"].split()[0]

        tables = soup.find_all("table", class_="wf-table-inset")

        for table in tables:

            rows = table.find_all("tr")

            for row in rows:

                player_td = row.find("td", class_="mod-player")

                if not player_td:
                    continue

                player_link = player_td.find("a")

                if not player_link:
                    continue

                player_href = player_link.get("href", "")

                player_id_match = re.search(r"/player/(\d+)/", player_href)

                if player_id_match:
                    vlr_player_id = player_id_match.group(1)
                else:
                    vlr_player_id = None

                name_div = player_link.find("div", class_="text-of")

                player_name = (
                    name_div.get_text(strip=True)
                    if name_div
                    else ""
                )

                team_div = player_link.find(
                    "div",
                    class_="ge-text-light"
                )

                team_tag = (
                    team_div.get_text(strip=True)
                    if team_div
                    else ""
                )

                stats = row.find_all("td", class_="mod-stat")

                if len(stats) < 11:
                    continue

                try:
                    rating = stats[0].find(
                        "span",
                        class_="mod-both"
                    ).text.strip()

                    acs = stats[1].find(
                        "span",
                        class_="mod-both"
                    ).text.strip()

                    kills = stats[2].find(
                        "span",
                        class_="mod-both"
                    ).text.strip()

                    deaths = stats[3].find(
                        "span",
                        class_="mod-both"
                    ).text.strip()

                    assists = stats[4].find(
                        "span",
                        class_="mod-both"
                    ).text.strip()

                    kast = stats[6].find(
                        "span",
                        class_="mod-both"
                    ).text.strip().replace("%", "")

                    adr = stats[7].find(
                        "span",
                        class_="mod-both"
                    ).text.strip()

                    hs = stats[8].find(
                        "span",
                        class_="mod-both"
                    ).text.strip().replace("%", "")

                    fk = stats[9].find(
                        "span",
                        class_="mod-both"
                    ).text.strip()

                    fd = stats[10].find(
                        "span",
                        class_="mod-both"
                    ).text.strip()

                except:
                    continue

                all_stats.append({
                    "vlr_match_id": vlr_match_id,
                    "match_date": match_date,
                    "vlr_player_id": vlr_player_id,
                    "player_name": player_name,
                    "team_tag": team_tag,
                    "rating": rating,
                    "acs": acs,
                    "kills": kills,
                    "deaths": deaths,
                    "assists": assists,
                    "kast": kast,
                    "adr": adr,
                    "hs_percentage": hs,
                    "fk": fk,
                    "fd": fd
                })

    except Exception as e:
        print("Error:", e)


for _, player in players_df.iloc[83:].iterrows():

    player_name = player["player_name"]

    print(f"\nSearching: {player_name}")

    profile = get_vlr_profile(player_name)

    if not profile:
        print("No VLR profile found")
        continue

    # Must actually be a player profile
    if "/player/" not in profile:
        print("Invalid profile:", profile)
        continue

    print("Profile:", profile)

    matches = get_recent_matches(profile)

    print("Matches found:", len(matches))

    for match in matches:

        if match in visited_matches:
            continue

        visited_matches.add(match)

        print("Parsing:", match)

        parse_match(match)

        time.sleep(1)

    time.sleep(1)


new_df = pd.DataFrame(all_stats)

final_df = pd.concat(
    [old_df, new_df],
    ignore_index=True
)

final_df.drop_duplicates(
    subset=[
        "vlr_match_id",
        "vlr_player_id",
        "player_name"
    ],
    inplace=True
)

final_df.to_csv(
    "csv/vlr_player_stats.csv",
    index=False
)

print("Final Rows:", len(final_df))

print("\n================================")
print("Rows:", len(final_df))
print("Saved to csv/vlr_player_stats.csv")
print("================================")

print(final_df.head())