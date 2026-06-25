# FX Loop Finder

<p align="left"> <img alt="KDMHS" src="https://img.shields.io/static/v1?label=&message=KDMHS&color=f2a6c9&labelColor=cd2370&style=flat-square&logo=data%3Aimage%2Fsvg%2Bxml%3Bbase64%2CPHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHZpZXdCb3g9IjAgMCAzOTYuODUgMzk2Ljg1Ij48cGF0aCBmaWxsPSIjZmZmIiBkPSJNMzQ3LjgsMjI4LjdjLTE2LjI2LDgxLjY0LTk1LjYxLDEzNC42NC0xNzcuMjUsMTE4LjM5LTgxLjY0LTE2LjI2LTEzNC42NC05NS42MS0xMTguMzktMTc3LjI1LDE2LjI2LTgxLjY0LDk1LjYxLTEzNC42NCwxNzcuMjUtMTE4LjM5LDM1LjU0LDcuMDgsNjcuMzQsMjYuNzIsODkuNTksNTUuMzNsLTI2LjczLDQ5LjMxYy0yMy45NC01MS4wNC04NC43NS03My4wMi0xMzUuOC00OS4wNy01MS4wNCwyMy45NC03My4wMiw4NC43NS00OS4wNywxMzUuOCwyMy45NCw1MS4wNCw4NC43NSw3My4wMiwxMzUuOCw0OS4wNywxOS42OC05LjIzLDM1LjkxLTI0LjUxLDQ2LjMxLTQzLjYxbDUzLjAxLTk4LjY0LDIzLjA1LTQxLjAyYy03LjQtMTMuMzktMTYuMzgtMjUuODUtMjYuNzMtMzcuMTFDMjY4LjcxLTUuMTMsMTQ5Ljc3LTEwLjQ1LDczLjExLDU5LjY0LTMuNTQsMTI5LjcyLTguODcsMjQ4LjY3LDYxLjIyLDMyNS4zM2M3MC4wOSw3Ni42NSwxODkuMDQsODEuOTgsMjY1LjY5LDExLjg5LDMwLjE1LTI3LjU3LDUwLjUtNjQuMjEsNTcuOTktMTA0LjM2bC0zNy4xMS00LjE1aDBaTTE5OS42MSwyNTcuNzRjLTMyLjMzLDAtNTguNTMtMjYuMi01OC41My01OC41M3MyNi4yLTU4LjUzLDU4LjUzLTU4LjUzLDU4LjUzLDI2LjIsNTguNTMsNTguNTMtMjYuMiw1OC41My01OC41Myw1OC41MyIvPjwvc3ZnPg%3D%3D&logoColor=white" /> <img alt="C Language" src="https://img.shields.io/badge/C%20Language-00599C?style=flat-square&logo=c&logoColor=white" /> <img alt="Python" src="https://img.shields.io/badge/Python-3776AB?style=flat-square&logo=python&logoColor=white" /> </p>

`FX Loop Finder`는 원화(KRW), 달러(USD), 엔화(JPY)를 차례대로 환전했을 때 차익이 생기는지 계산하는 C 콘솔 프로그램입니다.
사용자가 시작 금액과 수수료율을 입력하면 `KRW -> USD -> JPY -> KRW` 경로로 환전한 최종 금액을 계산하고, 시작 금액보다 커지는지 확인해 아비트라지 가능성을 판단합니다.

Python 프로그램이 환율 API에서 값을 받아 `rates.json` 파일로 저장하고, C 프로그램은 그 JSON 파일을 읽어서 환율 계산을 합니다.

## 주요 기능

- KRW -> USD -> JPY -> KRW 환전 경로 분석
- 환전 수수료율을 반영한 최종 금액 계산
- 예상 손익과 수익률 출력
- Python으로 `rates.json` 자동 업데이트

## 파일 구성

| 파일 | 설명 |
| --- | --- |
| `main.c` | 콘솔 화면, 메뉴, 환율 읽기, 차익 계산을 모두 담당하는 C 코드 (한국어 함수/변수명) |
| `main.py` | 환율 API에서 값을 받아 `rates.json`을 계속 저장하는 Python 코드 |
| `rates.json` | C 프로그램이 읽는 환율 데이터 파일 |

## 실행 준비

필요한 프로그램:

- GCC
- Python 3

Python 코드는 표준 라이브러리만 사용하므로 따로 설치할 패키지는 없습니다.

## 실행 방법

먼저 Python 프로그램을 실행해서 `rates.json`을 계속 업데이트합니다.

```powershell
python main.py
```

그 다음 새 터미널에서 C 프로그램을 컴파일하고 실행합니다.

```powershell
gcc main.c -o FXLoopFinder.exe -Wall -Wextra -finput-charset=UTF-8 -fexec-charset=UTF-8
.\FXLoopFinder.exe
```

또는 코드 에디터에서 직접 실행합니다.

```powershell
gcc main.c -o FXLoopFinder.exe && .\FXLoopFinder.exe
```

## 프로그램 흐름

```text
main.py
  -> 환율 API에서 KRW/USD, USD/JPY, JPY/KRW 값을 받음
  -> rates.json 파일에 저장

main.c
  -> rates.json 파일을 읽음
  -> KRW -> USD -> JPY -> KRW 순서로 환전 계산
  -> 최종 금액, 손익, 수익률 출력
```

## 주의사항

현재 `main.py`는 Frankfurter API를 사용합니다. 이 API는 초 단위로 변하는 실시간 트레이딩 시세가 아니라 최신 기준 환율 데이터를 제공합니다.

`main.py`는 2초마다 `rates.json`을 다시 저장하지만, API 원본 데이터가 바뀌지 않으면 환율 숫자도 그대로일 수 있습니다.

**콘솔 한글 출력:**
- Windows 환경에서 콘솔 한글이 깨질 경우 다음 명령어로 UTF-8을 활성화하세요:

```powershell
chcp 65001
```

**파일 구성:**
- `main.c` 파일 하나로 모든 기능을 처리합니다 (헤더파일 불필요)
- 모든 함수명과 변수명이 한국어로 되어 있습니다

## 차익 계산식

시작 금액을 `KRW`로 입력하면 아래 순서로 계산합니다.

```text
KRW -> USD -> JPY -> KRW
```

환전할 때마다 입력한 수수료율이 한 번씩 적용됩니다.

예를 들어 수수료율이 `0.1%`라면 각 환전 단계마다 `99.9%`만 다음 통화로 넘어갑니다.

## 코드 구조

### C 프로그램 (`main.c`)

**매크로 상수:**
- `버퍼크기`: 4096 (JSON 파일 읽기 버퍼)

**주요 함수 (모두 한국어 명명):**
- `입력버퍼정리()`: 입력 버퍼 정리 <손태영>
- `엔터대기()`: Enter만 입력될 때까지 대기 <손태영>
- `헤더출력()`: 프로그램 로고 출력 <손태영>
- `화면지우고헤더출력()`: 화면을 지운 뒤 프로그램 로고 출력 <손태영>
- `메뉴출력()`: 메뉴 화면 출력 <손태영>
- `환율표시()`: 현재 환율 표시 <서정후>
- `파일읽기()`: rates.json 파일 읽기 <서정후>
- `JSON환율파싱()`: JSON에서 환율 값 추출 <서정후>
- `환율업데이트()`: rates.json에서 환율 업데이트 <서정후>
- `차익계산()`: KRW → USD → JPY → KRW 계산 <서호현>
- `수익률계산()`: 수익률 계산 <서호현>
- `결과표시()`: 분석 결과 출력 <서호현>
- `차익분석하기()`: 입력을 받고 분석을 실행 <서호현>
- `main()`: 코드의 메인 함수로써 주요 함수들의 종합이다. <손태영>

**주요 변수 (모두 한국어 명명):**
- `원달러환율`: KRW -> USD 환율
- `달러엔환율`: USD -> JPY 환율
- `엔원환율`: JPY -> KRW 환율
- `메뉴선택`: 사용자가 선택한 메뉴 번호
- `시작금액`: 환전 시작 금액
- `수수료율`: 각 환전 단계의 수수료 비율
- `최종금액`: 계산된 최종 금액
