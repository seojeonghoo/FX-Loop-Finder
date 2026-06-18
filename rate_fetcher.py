import json
import os
import time
import urllib.request
from datetime import datetime


# Python이 환율을 저장할 파일 이름입니다.
OUTPUT_FILE = "rates.json"

# 몇 초마다 API를 다시 받을지 정합니다.
UPDATE_SECONDS = 60


# from_currency에서 to_currency로 바꿀 때의 환율 하나를 가져옵니다.
def get_rate(from_currency, to_currency):
    url = f"https://api.frankfurter.app/latest?from={from_currency}&to={to_currency}"

    # API가 Python 기본 요청을 막을 수 있어서 User-Agent를 넣어 줍니다.
    request = urllib.request.Request(
        url,
        headers={
            "User-Agent": "FX-Loop-Finder/1.0"
        }
    )

    with urllib.request.urlopen(request, timeout=10) as response:
        text = response.read().decode("utf-8")

    data = json.loads(text)
    return data["rates"][to_currency]


# 필요한 세 환율을 API에서 받아 rates.json 파일에 저장합니다.
def save_rates():
    krw_usd = get_rate("KRW", "USD")
    usd_jpy = get_rate("USD", "JPY")
    jpy_krw = get_rate("JPY", "KRW")

    rates = {
        "updated_at": datetime.now().strftime("%Y-%m-%d %H:%M:%S"),
        "KRW_USD": krw_usd,
        "USD_JPY": usd_jpy,
        "JPY_KRW": jpy_krw
    }

    # 먼저 임시 파일에 저장하고, 마지막에 rates.json으로 바꿉니다.
    # 이렇게 하면 C 프로그램이 파일을 읽는 중간에 내용이 반쯤만 저장되는 일을 줄일 수 있습니다.
    temp_file = OUTPUT_FILE + ".tmp"
    with open(temp_file, "w", encoding="utf-8") as file:
        json.dump(rates, file, indent=4)

    os.replace(temp_file, OUTPUT_FILE)

    print("환율 저장 완료:", rates)


# 프로그램을 켜 두면 UPDATE_SECONDS마다 계속 환율을 새로 저장합니다.
def main():
    print("환율 자동 업데이트를 시작합니다.")
    print("종료하려면 Ctrl + C를 누르세요.")

    try:
        while True:
            try:
                save_rates()
            except Exception as error:
                print("환율을 가져오지 못했습니다:", error)

            time.sleep(UPDATE_SECONDS)
    except KeyboardInterrupt:
        print("\n환율 자동 업데이트를 종료합니다.")


if __name__ == "__main__":
    main()
