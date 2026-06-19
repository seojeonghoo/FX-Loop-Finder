#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fx_loop.h"

/* 통화 이름과 환율을 저장하는 전역 배열입니다. */
char 통화이름들[CURRENCY_COUNT][NAME_LENGTH] = {"KRW", "USD", "JPY"};
double 환율[CURRENCY_COUNT][CURRENCY_COUNT] = {
    {0.0, 0.00066, 0.0},
    {0.0, 0.0, 160.31},
    {9.4632, 0.0, 0.0}
};

/* 화면 전환에 사용하는 간단한 보조 함수들입니다. */
void 화면지우기(void) {
    system("cls");
}

void 입력버퍼정리(void) {
    int 문자;

    while ((문자 = getchar()) != '\n' && 문자 != EOF) {
    }
}

void 엔터대기(void) {
    printf("\nEnter를 누르면 메뉴로 돌아갑니다...");
    getchar();
}

void 헤더표시(void) {
    화면지우기();
    헤더출력();
}

/* 프로그램 로고를 출력합니다. */
void 헤더출력(void) {
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

void 메뉴출력(void) {
    printf("\n");
    printf("1. 실시간 환율 업데이트\n");
    printf("2. 차익 분석하기\n");
    printf("3. 현재 환율 보기\n");
    printf("4. 프로그램 종료\n");
    printf("════════════════════════════════\n");
    printf("메뉴 번호를 입력하세요: ");
}

void 환율표시(void) {
    printf("\n");
    printf("════════════════════════════════════════════════════════════════\n");
    printf("                        현재 환율 정보\n");
    printf("════════════════════════════════════════════════════════════════\n");
    printf("1 %s = %.8f %s\n", 통화이름들[KRW], 환율[KRW][USD], 통화이름들[USD]);
    printf("1 %s = %.8f %s\n", 통화이름들[USD], 환율[USD][JPY], 통화이름들[JPY]);
    printf("1 %s = %.8f %s\n", 통화이름들[JPY], 환율[JPY][KRW], 통화이름들[KRW]);
    printf("════════════════════════════════════════════════════════════════\n");
}

int 파일읽기(const char *파일이름, char *버퍼, int 버퍼크기) {
    FILE *파일;
    int 읽은바이트;

    버퍼[0] = '\0';

    파일 = fopen(파일이름, "r");
    if (파일 == NULL) {
        return 0;
    }

    읽은바이트 = (int)fread(버퍼, 1, 버퍼크기 - 1, 파일);
    버퍼[읽은바이트] = '\0';
    fclose(파일);

    return 읽은바이트 > 0;
}

double JSON환율파싱(const char *JSON데이터, const char *키이름) {
    char 키[32];
    char *위치;
    double 환율값;

    sprintf(키, "\"%s\":", 키이름);
    위치 = strstr(JSON데이터, 키);

    if (위치 == NULL) {
        return 0.0;
    }

    위치 = 위치 + strlen(키);

    if (sscanf(위치, "%lf", &환율값) != 1) {
        return 0.0;
    }

    return 환율값;
}

int 환율업데이트(void) {
    char JSON데이터[BUFFER_SIZE];
    double 원달러환율;
    double 달러엔환율;
    double 엔원환율;

    if (!파일읽기("rates.json", JSON데이터, BUFFER_SIZE)) {
        return 0;
    }

    원달러환율 = JSON환율파싱(JSON데이터, "KRW_USD");
    달러엔환율 = JSON환율파싱(JSON데이터, "USD_JPY");
    엔원환율 = JSON환율파싱(JSON데이터, "JPY_KRW");

    if (원달러환율 <= 0.0 || 달러엔환율 <= 0.0 || 엔원환율 <= 0.0) {
        return 0;
    }

    환율[KRW][USD] = 원달러환율;
    환율[USD][JPY] = 달러엔환율;
    환율[JPY][KRW] = 엔원환율;

    return 1;
}

double 차익계산(double 시작금액, double 수수료율) {
    double 수수료비율;
    double 결과금액;

    수수료비율 = 1.0 - 수수료율 / 100.0;
    결과금액 = 시작금액;

    결과금액 = 결과금액 * 환율[KRW][USD] * 수수료비율;
    결과금액 = 결과금액 * 환율[USD][JPY] * 수수료비율;
    결과금액 = 결과금액 * 환율[JPY][KRW] * 수수료비율;

    return 결과금액;
}

double 수익률계산(double 시작금액, double 최종금액) {
    return (최종금액 - 시작금액) / 시작금액 * 100.0;
}

void 결과표시(double 시작금액, double 최종금액, double 수익률) {
    double 손익 = 최종금액 - 시작금액;

    printf("\n");
    printf("════════════════════════════════════════════════════════════════\n");
    printf("                           분석 결과\n");
    printf("════════════════════════════════════════════════════════════════\n");
    printf("환전 경로 : KRW -> USD -> JPY -> KRW\n");
    printf("시작 금액 : %.2f 원\n", 시작금액);
    printf("최종 금액 : %.2f 원\n", 최종금액);
    printf("예상 손익 : %.2f 원\n", 손익);
    printf("수익률    : %.4f%%\n", 수익률);

    if (손익 > 0.0) {
        printf("판정      : 차익 가능성이 있습니다.\n");
    } else {
        printf("판정      : 차익 가능성이 낮습니다.\n");
    }

    printf("════════════════════════════════════════════════════════════════\n");
}

int main(void) {
    int 메뉴선택;
    double 시작금액;
    double 수수료율;
    double 최종금액;
    double 수익률;

    while (1) {
        헤더표시();
        메뉴출력();
        scanf("%d", &메뉴선택);
        입력버퍼정리();

        if (메뉴선택 == 1) {
            헤더표시();
            printf("\nrates.json에서 환율 정보를 읽는 중입니다...\n");

            if (환율업데이트()) {
                헤더표시();
                printf("환율 정보를 업데이트했습니다.\n");
                환율표시();
            } else {
                헤더표시();
                printf("rates.json 파일을 읽지 못했습니다.\n");
                printf("먼저 Python 프로그램을 실행해 rates.json을 만들어주세요.\n");
            }

            엔터대기();
        } else if (메뉴선택 == 2) {
            헤더표시();
            printf("시작 금액을 입력하세요(KRW): ");
            scanf("%lf", &시작금액);

            printf("환전 1회당 수수료율을 입력하세요(%%): ");
            scanf("%lf", &수수료율);
            입력버퍼정리();

            if (시작금액 <= 0.0) {
                헤더표시();
                printf("시작 금액은 0보다 커야 합니다.\n");
                엔터대기();
            } else if (수수료율 < 0.0 || 수수료율 >= 100.0) {
                헤더표시();
                printf("수수료율은 0 이상 100 미만이어야 합니다.\n");
                엔터대기();
            } else {
                최종금액 = 차익계산(시작금액, 수수료율);
                수익률 = 수익률계산(시작금액, 최종금액);

                헤더표시();
                결과표시(시작금액, 최종금액, 수익률);
                엔터대기();
            }
        } else if (메뉴선택 == 3) {
            헤더표시();
            환율표시();
            엔터대기();
        } else if (메뉴선택 == 4) {
            헤더표시();
            printf("프로그램을 종료합니다.\n");
            break;
        }
    }

    return 0;
}
