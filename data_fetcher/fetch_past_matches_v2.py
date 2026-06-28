import requests
import pandas as pd
import os
import time

API_KEY = "FM7qtrAS8VQbg2I9w4qfKjUOpPSn8WA1FdcmDFs0PLp6svo89Uk"

headers = {
    "Authorization": f"Bearer {API_KEY}"
}

all_matches = []

page = 1

os.makedirs(
    r"C:\Users\lavet\OneDrive\Desktop\dbms\CSV",
    exist_ok=True
)

while True:
    try:
        url = (
            f"https://api.pandascore.co/valorant/matches/past"
            f"?page[size]=100&page[number]={page}"
        )

        response = requests.get(
            url,
            headers=headers,
            timeout=30
        )

        if response.status_code != 200:
            print(f"\nError on page {page}")
            print(response.status_code)
            print(response.text)
            break

        matches = response.json()

        if len(matches) == 0:
            print("\nNo more matches found.")
            break

        print(f"Fetched page {page} ({len(matches)} matches)")

        for m in matches:

            team1 = None
            team2 = None

            team1_id = None
            team2_id = None

            opponents = m.get("opponents", [])

            if len(opponents) >= 1:
                opp = opponents[0].get("opponent")

                if opp:
                    team1 = opp.get("name")
                    team1_id = opp.get("id")

            if len(opponents) >= 2:
                opp = opponents[1].get("opponent")

                if opp:
                    team2 = opp.get("name")
                    team2_id = opp.get("id")

            winner_id = None

            winner = m.get("winner")

            if winner:
                winner_id = winner.get("id")

            tournament_id = None

            tournament = m.get("tournament")

            if tournament:
                tournament_id = tournament.get("id")

            all_matches.append({
                "match_id": m.get("id"),

                "team1": team1,
                "team2": team2,

                "team1_id": team1_id,
                "team2_id": team2_id,

                "winner_id": winner_id,

                "status": m.get("status"),

                "begin_at": m.get("begin_at"),

                "tournament_id": tournament_id
            })

        # Save progress after every page
        df = pd.DataFrame(all_matches)

        df.drop_duplicates(
            subset=["match_id"],
            inplace=True
        )

        output_path = (
            r"C:\Users\lavet\OneDrive\Desktop\dbms\CSV"
            r"\past_matches_final.csv"
        )

        df.to_csv(
            output_path,
            index=False
        )

        print(
            f"Saved {len(df)} unique matches so far."
        )

        page += 1

        # Avoid API rate limits
        time.sleep(0.5)

    except KeyboardInterrupt:

        print("\nStopped by user.")

        df = pd.DataFrame(all_matches)

        df.drop_duplicates(
            subset=["match_id"],
            inplace=True
        )

        df.to_csv(
            output_path,
            index=False
        )

        print(
            f"Progress saved. "
            f"{len(df)} matches written to CSV."
        )

        break

    except Exception as e:

        print(
            f"\nUnexpected error on page {page}:"
        )

        print(e)

        df = pd.DataFrame(all_matches)

        df.drop_duplicates(
            subset=["match_id"],
            inplace=True
        )

        df.to_csv(
            output_path,
            index=False
        )

        print(
            f"Progress saved. "
            f"{len(df)} matches written to CSV."
        )

        break

print("\nDone.")

if len(all_matches) > 0:

    final_df = pd.DataFrame(all_matches)

    final_df.drop_duplicates(
        subset=["match_id"],
        inplace=True
    )

    print(
        f"\nFinal unique matches: "
        f"{len(final_df)}"
    )

    print("\nFirst 10 rows:")

    print(final_df.head(10))