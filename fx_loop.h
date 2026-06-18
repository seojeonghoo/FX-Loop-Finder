#ifndef FX_LOOP_H
#define FX_LOOP_H

/* 프로그램 전체에서 같이 쓰는 값입니다. */
#define CURRENCY_COUNT 3
#define NAME_LENGTH 4
#define BUFFER_SIZE 4096

/* 통화 이름 대신 사용할 번호입니다. */
#define KRW 0
#define USD 1
#define JPY 2

/* 화면 관련 함수 */
void clearScreen(void);
void clearInputLine(void);
void waitForEnter(void);
void showHeader(void);
void printHeader(void);
void printMenu(void);
void displayRates(void);

/* rates.json 읽기 관련 함수 */
int readFile(const char *fileName, char *buffer, int bufferSize);
double parseJsonRate(const char *json, const char *keyName);
int updateRates(void);

/* 계산 관련 함수 */
double calculateArbitrage(double startAmount, double feePercent);
double calculateProfitRate(double startAmount, double finalAmount);
void displayResult(double startAmount, double finalAmount, double profitRate);

#endif
