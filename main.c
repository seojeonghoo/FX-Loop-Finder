#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define 버퍼크기 4096

double 원달러환율 = 0.00066;
double 달러엔환율 = 160.31;
double 엔원환율 = 9.4632;

void 입력버퍼정리(void) {
    int 문자;

    while ((문자 = getchar()) != '\n' && 문자 != EOF) {
    }
}

void 엔터대기(void) {
    int 문자;

    while (1) {
        printf("\nEnter를 누르면 계속합니다...");
        문자 = getchar();

        if (문자 == '\n' || 문자 == EOF) {
            return;
        }

        입력버퍼정리();
        printf("Enter만 눌러주세요.\n");
    }
}

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

void 화면지우고헤더출력(void) {
    system("cls");
    헤더출력();
}

void 메뉴출력(void) {
    printf("\n");
    printf("1. 환율 업데이트\n");
    printf("2. 차익 분석\n");
    printf("3. 현재 환율 보기\n");
    printf("4. 종료\n");
    printf("메뉴 선택: ");
}

void 환율표시(void) {
    printf("\n[현재 환율]\n");
    printf("1 KRW = %.8f USD\n", 원달러환율);
    printf("1 USD = %.8f JPY\n", 달러엔환율);
    printf("1 JPY = %.8f KRW\n", 엔원환율);
}

int 파일읽기(const char *파일이름, char *버퍼, int 최대크기) {
    FILE *파일 = fopen(파일이름, "r");
    int 읽은바이트;

    if (파일 == NULL) {
        return 0;
    }

    읽은바이트 = (int)fread(버퍼, 1, 최대크기 - 1, 파일);
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
    char JSON데이터[버퍼크기];
    double 새원달러환율;
    double 새달러엔환율;
    double 새엔원환율;

    if (!파일읽기("rates.json", JSON데이터, 버퍼크기)) {
        return 0;
    }

    새원달러환율 = JSON환율파싱(JSON데이터, "KRW_USD");
    새달러엔환율 = JSON환율파싱(JSON데이터, "USD_JPY");
    새엔원환율 = JSON환율파싱(JSON데이터, "JPY_KRW");

    if (새원달러환율 <= 0.0 || 새달러엔환율 <= 0.0 || 새엔원환율 <= 0.0) {
        return 0;
    }

    원달러환율 = 새원달러환율;
    달러엔환율 = 새달러엔환율;
    엔원환율 = 새엔원환율;

    return 1;
}

double 차익계산(double 시작금액, double 수수료율) {
    double 수수료비율 = 1.0 - 수수료율 / 100.0;
    double 결과금액 = 시작금액;

    결과금액 = 결과금액 * 원달러환율 * 수수료비율;
    결과금액 = 결과금액 * 달러엔환율 * 수수료비율;
    결과금액 = 결과금액 * 엔원환율 * 수수료비율;

    return 결과금액;
}

double 수익률계산(double 시작금액, double 최종금액) {
    return (최종금액 - 시작금액) / 시작금액 * 100.0;
}

void 결과표시(double 시작금액, double 최종금액) {
    double 손익 = 최종금액 - 시작금액;
    double 수익률 = 수익률계산(시작금액, 최종금액);

    printf("\n[분석 결과]\n");
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
}

void 차익분석하기(void) {
    double 시작금액;
    double 수수료율;
    double 최종금액;

    printf("시작 금액(KRW): ");
    scanf("%lf", &시작금액);

    printf("환전 1회당 수수료율(%%): ");
    scanf("%lf", &수수료율);
    입력버퍼정리();

    if (시작금액 <= 0.0) {
        printf("시작 금액은 0보다 커야 합니다.\n");
        return;
    }

    if (수수료율 < 0.0 || 수수료율 >= 100.0) {
        printf("수수료율은 0 이상 100 미만이어야 합니다.\n");
        return;
    }

    최종금액 = 차익계산(시작금액, 수수료율);
    결과표시(시작금액, 최종금액);
}

int main(void) {
    int 메뉴선택;

    while (1) {
        화면지우고헤더출력();
        메뉴출력();

        if (scanf("%d", &메뉴선택) != 1) {
            printf("숫자를 입력해주세요.\n");
            입력버퍼정리();
            continue;
        }
        입력버퍼정리();

        switch (메뉴선택) {
        case 1:
            화면지우고헤더출력();
            if (환율업데이트()) {
                printf("환율 정보를 업데이트했습니다.\n");
                환율표시();
            } else {
                printf("rates.json 파일을 읽지 못했습니다.\n");
                printf("먼저 Python 프로그램을 실행해 rates.json을 만들어주세요.\n");
            }
            엔터대기();
            break;

        case 2:
            화면지우고헤더출력();
            차익분석하기();
            엔터대기();
            break;

        case 3:
            화면지우고헤더출력();
            환율표시();
            엔터대기();
            break;

        case 4:
            화면지우고헤더출력();
            printf("프로그램을 종료합니다.\n");
            return 0;

        default:
            화면지우고헤더출력();
            printf("없는 메뉴입니다.\n");
            엔터대기();
            break;
        }
    }
}
