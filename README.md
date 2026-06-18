# FX Loop Finder

`FX Loop Finder`는 원화(KRW), 달러(USD), 엔화(JPY)를 차례대로 환전했을 때 차익이 생기는지 계산하는 C 콘솔 프로그램입니다.

사용자가 시작 금액과 수수료율을 입력하면 `KRW -> USD -> JPY -> KRW` 경로로 환전한 최종 금액을 계산하고, 시작 금액보다 커지는지 확인해 아비트라지 가능성을 판단합니다.

Python 프로그램이 환율 API에서 값을 받아 `rates.json` 파일로 저장하고, C 프로그램은 그 JSON 파일을 읽어서 환율 계산을 합니다.

## 주요 기능

- KRW -> USD -> JPY -> KRW 환전 경로 분석
- 환전 수수료율을 반영한 최종 금액 계산
- 예상 손익과 수익률 출력
- Python으로 `rates.json` 자동 업데이트
- VS Code Code Runner에서 `Ctrl + Alt + N` 실행 지원

## 파일 구성

| 파일 | 설명 |
| --- | --- |
| `main.c` | 콘솔 화면, 메뉴, 환율 읽기, 차익 계산을 담당하는 C 코드 |
| `fx_loop.h` | `main.c`에서 사용하는 상수와 함수 원형을 모아 둔 헤더 파일 |
| `main.py` | 환율 API에서 값을 받아 `rates.json`을 계속 저장하는 Python 코드 |
| `rates.json` | C 프로그램이 읽는 환율 데이터 파일 |
| `.vscode/settings.json` | VS Code Code Runner 실행 설정 |

## 실행 준비

필요한 프로그램:

- GCC
- Python 3
- VS Code Code Runner 확장 프로그램

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

VS Code Code Runner를 사용한다면 `main.c` 파일을 열고 `Ctrl + Alt + N`을 누르면 터미널에서 바로 컴파일 후 실행됩니다.

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

콘솔 한글이 깨질 경우 터미널에서 UTF-8을 켠 뒤 실행하세요.

```powershell
chcp 65001
```

## 차익 계산식

시작 금액을 `KRW`로 입력하면 아래 순서로 계산합니다.

```text
KRW -> USD -> JPY -> KRW
```

환전할 때마다 입력한 수수료율이 한 번씩 적용됩니다.

예를 들어 수수료율이 `0.1%`라면 각 환전 단계마다 `99.9%`만 다음 통화로 넘어갑니다.
