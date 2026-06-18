#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <wininet.h>
#include "fx_loop.h"

/* 통화 이름을 문자열 배열에 저장합니다. */
char currencyNames[CURRENCY_COUNT][NAME_LENGTH] = {"KRW", "USD", "JPY"};

/*
    환율을 저장하는 2차원 배열입니다.

    rates[KRW][USD]는 KRW에서 USD로 바꿀 때의 환율입니다.
    rates[USD][JPY]는 USD에서 JPY로 바꿀 때의 환율입니다.
    rates[JPY][KRW]는 JPY에서 KRW로 바꿀 때의 환율입니다.
*/
double rates[CURRENCY_COUNT][CURRENCY_COUNT] = {
    {1.0, 0.00066, 0.0},
    {0.0, 1.0, 160.31},
    {9.4632, 0.0, 1.0}
};

/* 콘솔 화면을 깨끗하게 지웁니다. */
void clearScreen(void) {
    system("cls");
}

/*
    scanf()를 쓰면 엔터가 입력 버퍼에 남을 수 있습니다.
    이 함수는 그 남은 엔터를 비워 줍니다.
*/
void clearInputLine(void) {
    int ch;

    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

/* 결과를 바로 넘기지 않고, 사용자가 Enter를 누를 때까지 기다립니다. */
void waitForEnter(void) {
    printf("\nEnter를 누르면 메뉴로 돌아갑니다...");
    getchar();
}

/* 화면을 지우고 로고를 출력합니다. 자주 반복되는 코드를 함수로 묶었습니다. */
void showHeader(void) {
    clearScreen();
    printHeader();
}

/* 프로그램 맨 위에 보이는 로고 화면을 출력합니다. */
void printHeader(void) {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════╗\n");
    printf("║                                                      ║\n");
    printf("║                ███████████ █████ █████               ║\n");
    printf("║               ▒▒███▒▒▒▒▒▒█▒▒███ ▒▒███                ║\n");
    printf("║                ▒███   █ ▒  ▒▒███ ███                 ║\n");
    printf("║                ▒███████     ▒▒█████                  ║\n");
    printf("║                ▒███▒▒▒█      ███▒███                 ║\n");
    printf("║                ▒███  ▒      ███ ▒▒███                ║\n");
    printf("║                █████       █████ █████               ║\n");
    printf("║               ▒▒▒▒▒       ▒▒▒▒▒ ▒▒▒▒▒                ║\n");
    printf("║                                                      ║\n");
    printf("║             F X   L O O P   F I N D E R              ║\n");
    printf("║          C언어 기반 환율 아비트라지 분석기           ║\n");
    printf("║                                                      ║\n");
    printf("╚══════════════════════════════════════════════════════╝\n");
}

/* 사용자가 고를 수 있는 메뉴를 출력합니다. */
void printMenu(void) {
    printf("\n");
    printf("1. 실시간 환율 업데이트\n");
    printf("2. 차익 분석하기\n");
    printf("3. 현재 환율 보기\n");
    printf("4. 프로그램 종료\n");
    printf("════════════════════════════════\n");
    printf("메뉴 번호를 입력하세요: ");
}

/* 현재 배열에 저장된 환율 3개를 화면에 보여 줍니다. */
void displayRates(void) {
    printf("\n");
    printf("════════════════════════════════════════════════════════════════\n");
    printf("                        현재 환율 정보\n");
    printf("════════════════════════════════════════════════════════════════\n");
    printf("1 %s = %.8f %s\n", currencyNames[KRW], rates[KRW][USD], currencyNames[USD]);
    printf("1 %s = %.8f %s\n", currencyNames[USD], rates[USD][JPY], currencyNames[JPY]);
    printf("1 %s = %.8f %s\n", currencyNames[JPY], rates[JPY][KRW], currencyNames[KRW]);
    printf("════════════════════════════════════════════════════════════════\n");
}

/*
    인터넷 주소(url)에 접속해서 받은 글자를 buffer 배열에 저장합니다.
    성공하면 1, 실패하면 0을 return합니다.
*/
int httpGet(const char *url, char *buffer, int bufferSize) {
    HINTERNET internet;
    HINTERNET file;
    DWORD bytesRead;
    int totalRead;

    totalRead = 0;
    buffer[0] = '\0';

    /* 인터넷 사용을 시작합니다. */
    internet = InternetOpenA("FX Loop Finder", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (internet == NULL) {
        return 0;
    }

    /* url에 실제로 접속합니다. */
    file = InternetOpenUrlA(internet, url, NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (file == NULL) {
        InternetCloseHandle(internet);
        return 0;
    }

    /* 받은 글자를 buffer에 조금씩 저장합니다. */
    while (InternetReadFile(file, buffer + totalRead, bufferSize - totalRead - 1, &bytesRead) && bytesRead > 0) {
        totalRead = totalRead + (int)bytesRead;

        if (totalRead >= bufferSize - 1) {
            break;
        }
    }

    buffer[totalRead] = '\0';

    InternetCloseHandle(file);
    InternetCloseHandle(internet);

    return totalRead > 0;
}

/*
    API 응답 글자에서 원하는 환율 숫자만 찾아냅니다.

    예를 들어 currencyCode가 "USD"이면,
    응답 글자에서 "USD": 뒤에 있는 숫자를 읽습니다.
*/
double parseRate(const char *json, const char *currencyCode) {
    char key[16];
    char *position;
    double rate;

    sprintf(key, "\"%s\":", currencyCode);
    position = strstr(json, key);

    if (position == NULL) {
        return 0.0;
    }

    position = position + strlen(key);

    if (sscanf(position, "%lf", &rate) != 1) {
        return 0.0;
    }

    return rate;
}

/*
    환율 하나를 API에서 가져와 rates 배열에 저장합니다.
    예: updateOneRate(KRW, USD)는 KRW -> USD 환율을 업데이트합니다.
*/
int updateOneRate(int from, int to) {
    char url[256];
    char json[BUFFER_SIZE];
    double rate;

    sprintf(
        url,
        "https://api.frankfurter.app/latest?from=%s&to=%s",
        currencyNames[from],
        currencyNames[to]
    );

    if (!httpGet(url, json, BUFFER_SIZE)) {
        return 0;
    }

    rate = parseRate(json, currencyNames[to]);
    if (rate <= 0.0) {
        return 0;
    }

    rates[from][to] = rate;

    return 1;
}

/* 이 프로그램에서 필요한 세 방향의 환율을 모두 업데이트합니다. */
int updateRates(void) {
    int success;

    success = 1;
    success = success && updateOneRate(KRW, USD);
    success = success && updateOneRate(USD, JPY);
    success = success && updateOneRate(JPY, KRW);

    return success;
}

/*
    KRW -> USD -> JPY -> KRW 순서로 환전했을 때 최종 금액을 계산합니다.
    각 환전마다 수수료도 같이 적용합니다.
*/
double calculateArbitrage(double startAmount, double feePercent) {
    double feeRate;
    double result;

    feeRate = 1.0 - feePercent / 100.0;
    result = startAmount;

    result = result * rates[KRW][USD] * feeRate;
    result = result * rates[USD][JPY] * feeRate;
    result = result * rates[JPY][KRW] * feeRate;

    return result;
}

/* 시작 금액과 최종 금액을 비교해서 수익률을 계산합니다. */
double calculateProfitRate(double startAmount, double finalAmount) {
    double profitRate;

    profitRate = (finalAmount - startAmount) / startAmount * 100.0;

    return profitRate;
}

/* 계산 결과를 보기 좋게 출력합니다. */
void displayResult(double startAmount, double finalAmount, double profitRate) {
    double profit;

    profit = finalAmount - startAmount;

    printf("\n");
    printf("════════════════════════════════════════════════════════════════\n");
    printf("                           분석 결과\n");
    printf("════════════════════════════════════════════════════════════════\n");
    printf("환전 경로 : KRW -> USD -> JPY -> KRW\n");
    printf("시작 금액 : %.2f 원\n", startAmount);
    printf("최종 금액 : %.2f 원\n", finalAmount);
    printf("예상 손익 : %.2f 원\n", profit);
    printf("수익률    : %.4f%%\n", profitRate);

    if (profit > 0.0) {
        printf("판정      : 차익 가능성이 있습니다.\n");
    } else {
        printf("판정      : 차익 가능성이 낮습니다.\n");
    }

    printf("════════════════════════════════════════════════════════════════\n");
}

/* main 함수는 프로그램이 처음 시작되는 곳입니다. */
int main(void) {
    int menu;
    double startAmount;
    double feePercent;
    double finalAmount;
    double profitRate;

    while (1) {
        showHeader();
        printMenu();

        if (scanf("%d", &menu) != 1) {
            clearInputLine();
            showHeader();
            printf("\n숫자를 입력해주세요.\n");
            waitForEnter();
            continue;
        }

        clearInputLine();
        showHeader();

        if (menu == 1) {
            printf("\n환율 정보를 가져오는 중입니다...\n");

            if (updateRates()) {
                showHeader();
                printf("환율 정보를 업데이트했습니다.\n");
                displayRates();
            } else {
                showHeader();
                printf("환율 정보를 가져오지 못했습니다.\n");
            }

            waitForEnter();
        } else if (menu == 2) {
            printf("시작 금액을 입력하세요(KRW): ");

            if (scanf("%lf", &startAmount) != 1) {
                clearInputLine();
                showHeader();
                printf("\n올바른 금액을 입력해주세요.\n");
                waitForEnter();
                continue;
            }

            clearInputLine();

            printf("환전 1회당 수수료율을 입력하세요(%%): ");

            if (scanf("%lf", &feePercent) != 1) {
                clearInputLine();
                showHeader();
                printf("\n올바른 수수료율을 입력해주세요.\n");
                waitForEnter();
                continue;
            }

            clearInputLine();

            if (startAmount <= 0.0) {
                showHeader();
                printf("시작 금액은 0보다 커야 합니다.\n");
                waitForEnter();
            } else if (feePercent < 0.0 || feePercent >= 100.0) {
                showHeader();
                printf("수수료율은 0 이상 100 미만이어야 합니다.\n");
                waitForEnter();
            } else {
                finalAmount = calculateArbitrage(startAmount, feePercent);
                profitRate = calculateProfitRate(startAmount, finalAmount);

                showHeader();
                displayResult(startAmount, finalAmount, profitRate);
                waitForEnter();
            }
        } else if (menu == 3) {
            displayRates();
            waitForEnter();
        } else if (menu == 4) {
            printf("프로그램을 종료합니다.\n");
            break;
        } else {
            printf("없는 메뉴입니다.\n");
            waitForEnter();
        }
    }

    return 0;
}
