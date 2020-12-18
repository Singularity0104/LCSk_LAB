#include <iostream>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <fstream>
#include "test.h"
using namespace std;
#define TEST 500
#define MAX_K 8
#define MINLEN 64
#define MAXLEN 128

char str_1[MAXLEN + 10];
char str_2[MAXLEN + 10];
int k;
int n;
int error = 0;
int correct = 0;
clock_t clock_01 = 0;
clock_t clock_02 = 0;
clock_t clock_01_matchlist = 0;
clock_t clock_02_matchlist = 0;

void PrintProcess(unsigned int percent) {
    /* 进度条缓冲区 */
    char processbar[51] = {0};
    /* 初始化进度条 */
    memset(processbar, '-', 50);
    /* 绘制正常进度条 */
    if (percent <= 100)
    {
        memset(processbar, '#', percent / 2);
        printf("testing: [%s] %d%%\r", processbar, percent);
        /* 绘制完成后的进度条 */
    }
    else
    {
        memset(processbar, '#', 50);
        printf("testing: [%s] Done\r", processbar);
        printf("\n");
        printf("correct: %d\n", correct);
        printf("error: %d\n", error);
        printf("total time 1: %.8fs\n", (double)(clock_01) / (double)CLOCKS_PER_SEC);
        printf("total time 1(no matchlist): %.8fs\n", (double)(clock_01 - clock_01_matchlist) / (double)CLOCKS_PER_SEC);
        printf("total time 2: %.8fs\n", (double)(clock_02) / (double)CLOCKS_PER_SEC);
        printf("total time 2(no matchlist): %.8fs\n", (double)(clock_02 - clock_02_matchlist) / (double)CLOCKS_PER_SEC);
    }
    fflush(stdout);
}

int main()
{
    FILE *file = fopen("./log.txt", "w+");
    error = 0;
    correct = 0;
    srand((unsigned)time(NULL));
    printf("\ntest information:\nn: %d~%d\nk: %d~%d\n\n", MINLEN, MAXLEN, 1, MAX_K);
    for(int i = 0; i < TEST; i++) {
        memset(str_1, 0, sizeof(str_1));
        memset(str_2, 0, sizeof(str_2));
        k = 1 + rand() % MAX_K;
        n = MINLEN + rand() % (MAXLEN - MINLEN + 1);
        for(int j = 0; j < n; j++) {
            str_1[j] = 'a' + rand() % ('z' - 'a' + 1);
            str_2[j] = 'a' + rand() % ('z' - 'a' + 1);
        }
        str_1[n] = '\0';
        str_2[n] = '\0';

        clock_t start_t, end_t;
        start_t = clock();
        int test_01_res = get_res_test_01(k, n, str_1, str_2);
        // int test_01_res = 0;
        end_t = clock();
        clock_01 += (end_t - start_t);

        start_t = clock();
        int test_02_res = get_res_test_02(k, n, str_1, str_2);
        end_t = clock();
        clock_02 += (end_t - start_t);
        
        if(test_01_res == test_02_res) {
            correct++;
        }
        else {
            error++;
            char Buf[256];
            memset(Buf, 0, sizeof(Buf));
            sprintf(Buf, "Error Case %d\nn: %d\nk: %d\nstr_1: %s\nstr_2: %s\ntest_01_res: %d\ntest_02_res: %d\n\n", error, n, k, str_1, str_2, test_01_res, test_02_res);
            fputs(Buf, file);
        }
        PrintProcess((i * 100) / TEST);
    }
    PrintProcess(101);
    fclose(file);
    return 0;
    // printf("bbb\n");
}

// int main()
// {
//     const char *A;
//     const char *B;
//     A = "gvohhbnouxfyhvndqzvqkhvibomoqpmbshlsruygkhitjgfympfvknwlpiqznvrwdcwlpswuxmhqwrjucywgl";
//     B = "oevjbebjcwfievavyfjcrzxrfgmujfuccnyvbofzuzybkvjzhxjzrkvhsydahuvdlkxgzndoeonxnppoxykny";
//     for(int i = 0; i < 85; i++) {
//         str_1[i] = A[i];
//         str_2[i] = B[i];
//     }
//     cout << get_res_test_01(2, 85, str_1, str_2) << endl;
//     cout << get_res_test_02(2, 85, str_1, str_2) << endl;
//     A = "ggwwxxdhqgslegxmsasjdxmbdykxlagqeqnnhauxmvfpxcdhlgyvgabyumnrsqqewgibypapkwejmvbioupmb";
//     B = "pvxsgieruubzbjddspxpgihcjxjqdkmzqwzsltdxvwsspguhxatfxaxntzbkwxxdmubobzpvbxoyccbyoyynd";
//     for(int i = 0; i < 85; i++) {
//         str_1[i] = A[i];
//         str_2[i] = B[i];
//     }
//     cout << get_res_test_01(2, 85, str_1, str_2) << endl;
//     cout << get_res_test_02(2, 85, str_1, str_2) << endl;
//     return 0;
// }