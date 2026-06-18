#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fx_loop.h"

/* 통화 이름과 환율을 저장하는 전역 배열입니다. */
char currencyNames[CURRENCY_COUNT][NAME_LENGTH] = {"KRW", "USD", "JPY"};

double rates[CURRENCY_COUNT][CURRENCY_COUNT] = {
    {1.0, 0.00066, 0.0},
    {0.0, 1.0, 160.31},
    {9.4632, 0.0, 1.0}
};

/* 화면 전환에 사용하는 간단한 보조 함수들입니다. */
void clearScreen(void) {
    system("cls");
}

void clearInputLine(void) {
    int ch;

    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

void waitForEnter(void) {
    printf("\nEnter를 누르면 메뉴로 돌아갑니다...");
    getchar();
}

void showHeader(void) {
    clearScreen();
    printHeader();
}

/* 프로그램 로고를 출력합니다. */
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

void printMenu(void) {
    printf("\n");
    printf("1. 실시간 환율 업데이트\n");
    printf("2. 차익 분석하기\n");
    printf("3. 현재 환율 보기\n");
    printf("4. 프로그램 종료\n");
    printf("════════════════════════════════\n");
    printf("메뉴 번호를 입력하세요: ");
}

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

/* rates.json 전체 내용을 문자열 배열에 읽어 옵니다. */
int readFile(const char *fileName, char *buffer, int bufferSize) {
    FILE *file;
    int totalRead;

    buffer[0] = '\0';

    file = fopen(fileName, "r");
    if (file == NULL) {
        return 0;
    }

    totalRead = (int)fread(buffer, 1, bufferSize - 1, file);
    buffer[totalRead] = '\0';
    fclose(file);

    return totalRead > 0;
}

/* JSON 문자열에서 keyName 뒤에 있는 실수 값을 찾아 읽습니다. */
double parseJsonRate(const char *json, const char *keyName) {
    char key[32];
    char *position;
    double rate;

    sprintf(key, "\"%s\":", keyName);
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

/* Python이 만든 rates.json에서 최신 환율을 가져옵니다. */
int updateRates(void) {
    char json[BUFFER_SIZE];
    double krwUsd;
    double usdJpy;
    double jpyKrw;

    if (!readFile("rates.json", json, BUFFER_SIZE)) {
        return 0;
    }

    krwUsd = parseJsonRate(json, "KRW_USD");
    usdJpy = parseJsonRate(json, "USD_JPY");
    jpyKrw = parseJsonRate(json, "JPY_KRW");

    if (krwUsd <= 0.0 || usdJpy <= 0.0 || jpyKrw <= 0.0) {
        return 0;
    }

    rates[KRW][USD] = krwUsd;
    rates[USD][JPY] = usdJpy;
    rates[JPY][KRW] = jpyKrw;

    return 1;
}

/* KRW -> USD -> JPY -> KRW 순서로 환전한 최종 금액을 계산합니다. */
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

double calculateProfitRate(double startAmount, double finalAmount) {
    return (finalAmount - startAmount) / startAmount * 100.0;
}

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

int main(void) {
    int menu;
    double startAmount;
    double feePercent;
    double finalAmount;
    double profitRate;

    while (1) {
        showHeader();
        printMenu();
        scanf("%d", &menu);
        clearInputLine();

        if (menu == 1) {
            showHeader();
            printf("\nrates.json에서 환율 정보를 읽는 중입니다...\n");

            if (updateRates()) {
                showHeader();
                printf("환율 정보를 업데이트했습니다.\n");
                displayRates();
            } else {
                showHeader();
                printf("rates.json 파일을 읽지 못했습니다.\n");
                printf("먼저 Python 프로그램을 실행해 rates.json을 만들어주세요.\n");
            }

            waitForEnter();
        } else if (menu == 2) {
            showHeader();
            printf("시작 금액을 입력하세요(KRW): ");
            scanf("%lf", &startAmount);

            printf("환전 1회당 수수료율을 입력하세요(%%): ");
            scanf("%lf", &feePercent);
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
            showHeader();
            displayRates();
            waitForEnter();
        } else if (menu == 4) {
            showHeader();
            printf("프로그램을 종료합니다.\n");
            break;
        } else {
            showHeader();
            printf("없는 메뉴입니다.\n");
            waitForEnter();
        }
    }

    return 0;
}
