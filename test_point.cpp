#include <iostream>
#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include <fstream>
#include "test.hpp"
using namespace std;

#define PRINTERROR 1
#define TEST 100
#define MIN_CHAR 'a'
#define MAX_CHAR 'h'
#define MIN_K 4
#define MAX_K 4
#define MINLEN 512
#define MAXLEN 512

char str_1[MAXLEN + 10];
char str_2[MAXLEN + 10];
int k;
int n;
int error = 0;
int correct = 0;
unsigned long long LCSk_sum = 0;
clock_t clock_DP = 0;
clock_t clock_Sparse = 0;
clock_t clock_Dense = 0;
clock_t clock_Sparse_matchlist = 0;
clock_t clock_Dense_matchlist = 0;

void print_test_info();
void print_head();
void print_row(const char *algo, clock_t time, int times);
void print_lastrow();
void PrintProcess(unsigned int percent);

int main()
{
    FILE *file = fopen("./log.txt", "w+");
    error = 0;
    correct = 0;
    // srand((unsigned)time(NULL));
    srand(clock());
    print_test_info();
    for (int i = 0; i < TEST; i++)
    {
        memset(str_1, 0, sizeof(str_1));
        memset(str_2, 0, sizeof(str_2));
        k = MIN_K + rand() % (MAX_K - MIN_K + 1);
        n = MINLEN + rand() % (MAXLEN - MINLEN + 1);
        for (int j = 0; j < n; j++)
        {
            str_1[j] = MIN_CHAR + rand() % (MAX_CHAR - MIN_CHAR + 1);
            str_2[j] = MIN_CHAR + rand() % (MAX_CHAR - MIN_CHAR + 1);
        }
        str_1[n] = '\0';
        str_2[n] = '\0';

        clock_t start_t, end_t;
        start_t = clock();
        int test_DP_res = get_res_test_DP(k, n, str_1, str_2);
        end_t = clock();
        clock_DP += (end_t - start_t);

        start_t = clock();
        int test_Sparse_res = get_res_test_Sparse(k, n, str_1, str_2);
        end_t = clock();
        clock_Sparse += (end_t - start_t);

        start_t = clock();
        int test_Dense_res = get_res_test_Dense(k, n, str_1, str_2);
        // int test_Dense_res = test_DP_res;
        end_t = clock();
        clock_Dense += (end_t - start_t);

        if (test_DP_res == test_Sparse_res && test_DP_res == test_Dense_res)
        {
            correct++;
            LCSk_sum += test_DP_res;
        }
#if PRINTERROR
        else
        {
            error++;
            char Buf[MAXLEN * 2 + 128];
            memset(Buf, 0, sizeof(Buf));
            sprintf(Buf, "Error Case %d\nn: %d\nk: %d\nstr_1: %s\nstr_2: %s\ntest_DP_res: %d\ntest_Sparse_res: %d\ntest_Dense_res: %d\n\n", error, n, k, str_1, str_2, test_DP_res, test_Sparse_res, test_Dense_res);
            fputs(Buf, file);
        }
#endif
        PrintProcess((i * 100) / TEST);
    }
    PrintProcess(101);
    fclose(file);
    return 0;
}

void print_test_info()
{
    printf("\nTest Information:\nN: %d~%d\nK: %d~%d\nChar: %c~%c\nTest Cases: %d\n\n", MINLEN, MAXLEN, MIN_K, MAX_K, MIN_CHAR, MAX_CHAR, TEST);
}

void print_head()
{
    for (int i = 0; i < 52; i++)
    {
        printf("-");
    }
    printf("\n");
    printf("| %-14s | %-14s | %-14s |\n", "Algorithm", "Totol T", "Average T");
    for (int i = 0; i < 52; i++)
    {
        printf("-");
    }
    printf("\n");
}

void print_row(const char *algo, clock_t time, int times)
{
    printf("| %-14s |", algo);
    printf(" %-13.6fs |", (double)(time) / (double)CLOCKS_PER_SEC);
    printf(" %-13.6fs |\n", ((double)(time) / (double)CLOCKS_PER_SEC) / (double)times);
}

void print_lastrow()
{
    for (int i = 0; i < 52; i++)
    {
        printf("-");
    }
    printf("\n");
    for (int i = 0; i < 52; i++)
    {
        printf("-");
    }
    printf("\n");
    printf("| %-14s | %-31.4f |\n", "Average LCSk", (double)LCSk_sum / (double)correct);
    for (int i = 0; i < 52; i++)
    {
        printf("-");
    }
    printf("\n");
}

void PrintProcess(unsigned int percent)
{
    /* 进度条缓冲区 */
    char processbar[51] = {0};
    /* 初始化进度条 */
    memset(processbar, '-', 50);
    /* 绘制正常进度条 */
    if (percent <= 100)
    {
        memset(processbar, '>', percent / 2);
        printf("Testing: [%s] %d%%\r", processbar, percent);
        /* 绘制完成后的进度条 */
    }
    else
    {
        memset(processbar, '>', 50);
        printf("Testing: [%s] Done\r", processbar);
        printf("\n\n");
#if PRINTERROR
        printf("Correct: %d\n", correct);
        printf("Error: %d\n", error);
#endif
        print_head();
        print_row("DP", clock_DP, TEST);
        print_row("Matchlist", (clock_Sparse_matchlist + clock_Dense_matchlist) / 2, 2 * TEST);
        // print_row("Matchlist", clock_Sparse_matchlist, TEST);
        print_row("Sparse", clock_Sparse, TEST);
        print_row("Sparse(core)", clock_Sparse - clock_Sparse_matchlist, TEST);
        print_row("Dense", clock_Dense, TEST);
        print_row("Dense(core)", clock_Dense - clock_Dense_matchlist, TEST);
        print_lastrow();
    }
    fflush(stdout);
}
