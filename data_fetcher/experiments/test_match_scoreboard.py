import requests
from bs4 import BeautifulSoup
import pandas as pd

HEADERS = {
    "User-Agent": "Mozilla/5.0"
}

MATCH_URL = "https://www.vlr.gg/646425/fokus-sakura-vs-hanami-project-queens-2026-split-2-lr1"

response = requests.get(MATCH_URL, headers=HEADERS)

print("Status:", response.status_code)

soup = BeautifulSoup(response.text, "html.parser")

# Find all tables
tables = soup.find_all("table")

print("\nTables Found:", len(tables))

for table_no, table in enumerate(tables):

    text = table.get_text(" ", strip=True)

    if "Juja" in text:

        print("\n==============================")
        print("JUJA TABLE FOUND")
        print("==============================")

        html = table.prettify()

        print(html[:10000])

        with open("juja_table.html", "w", encoding="utf-8") as f:
            f.write(html)

        print("\nSaved juja_table.html")


        break