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
void 화면지우기(void);
void 입력버퍼정리(void);
void 엔터대기(void);
void 헤더표시(void);
void 헤더출력(void);
void 메뉴출력(void);
void 환율표시(void);

/* rates.json 읽기 관련 함수 */
int 파일읽기(const char *파일이름, char *버퍼, int 버퍼크기);
double JSON환율파싱(const char *JSON데이터, const char *키이름);
int 환율업데이트(void);

/* 계산 관련 함수 */
double 차익계산(double 시작금액, double 수수료율);
double 수익률계산(double 시작금액, double 최종금액);
void 결과표시(double 시작금액, double 최종금액, double 수익률);

#endif
