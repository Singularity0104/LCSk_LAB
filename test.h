#ifndef TEST_H
#define TEST_H
#include <time.h>
extern clock_t clock_Sparse_matchlist;
extern clock_t clock_Dense_matchlist;
int get_res_test_DP(int K, int SIZE, char *STR_1, char *STR_2);
int get_res_test_Sparse(int K, int SIZE, char *STR_1, char *STR_2);
int get_res_test_Dense(int K, int SIZE, char *STR_1, char *STR_2);
#endif