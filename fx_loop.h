#ifndef FX_LOOP_H
#define FX_LOOP_H

/* 이 파일은 main.c에서 같이 쓰는 약속을 모아 둔 헤더 파일입니다. */

/* 프로그램에서 사용하는 통화는 KRW, USD, JPY 총 3개입니다. */
#define CURRENCY_COUNT 3

/* "KRW"처럼 글자 3개짜리 문자열은 마지막에 '\0'이 필요해서 4칸을 잡습니다. */
#define NAME_LENGTH 4

/* API가 보내 준 글자를 저장할 공간 크기입니다. */
#define BUFFER_SIZE 4096

/* 통화 번호를 숫자 대신 이름으로 쓰기 위해 만든 상수입니다. */
#define KRW 0
#define USD 1
#define JPY 2

/* main.c에 있는 함수들을 미리 알려 줍니다. */
void clearScreen(void);
void clearInputLine(void);
void waitForEnter(void);
void showHeader(void);
void printHeader(void);
void printMenu(void);
void displayRates(void);

int httpGet(const char *url, char *buffer, int bufferSize);
double parseRate(const char *json, const char *currencyCode);
int updateOneRate(int from, int to);
int updateRates(void);

double calculateArbitrage(double startAmount, double feePercent);
double calculateProfitRate(double startAmount, double finalAmount);
void displayResult(double startAmount, double finalAmount, double profitRate);

#endif
