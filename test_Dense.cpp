#include <iostream>
#include <map>
#include <cstring>
#include <string.h>
#include <time.h>
#include "test.h"
#include "MATCHLIST.hpp"
using namespace std;

static int N;
static int k;
#define inf 0x3fffffff
#define einf -100
#define MAXN inf
static map<int, int> THRESH[10000];

extern suffix S[1000000];                     // S数组
extern suffix X[1000000];                     // X数组

extern clock_t clock_Dense_matchlist;

// static void get_match_list(char *A, char *B)
// {
//     memset(matchlist, 0, sizeof(matchlist));
//     for (int i = 0; i < N - k + 1; i++)
//     {
//         for (int j = 0; j < N - k + 1; j++)
//         {
//             int now_i = i;
//             int now_j = j;

//             while (A[now_i] == B[now_j])
//             {
//                 if (now_j - j == k - 1)
//                 {
//                     matchlist[i + k - 1][j + k - 1] = 1;
//                     break;
//                 }
//                 now_i++;
//                 now_j++;
//             }
//         }
//     }
// }

// static int dense()
// {
//     THRESH[0].insert(pair<int, int>(0, einf));
//     THRESH[0].insert(pair<int, int>(1, inf));
//     for (int i = 1; i <= k - 1; ++i)
//     {
//         THRESH[i] = THRESH[i - 1];
//     }
//     for (int i = k; i <= N; ++i)
//     {
//         THRESH[i] = THRESH[i - 1];
//         int sz=THRESH[i - 1].size();
//         for (int h = 1; h <= sz-1; ++h)
//         {
//             int j1 = THRESH[i - k][h - 1];
//             int x = MAXN;
//             for (int j = j1 + k; j <= N; j++)
//             {
//                 if (matchlist[i - 1][j - 1] == 1)
//                 {
//                     x = j;
//                     break;
//                 }
//             }
//             int j2 = THRESH[i][h];
//             if (x != (MAXN))
//             {
//                 if(j2>x)
//                 THRESH[i][h] = x;
//                 if (j2 == inf)
//                 {
//                     THRESH[i].insert(pair<int, int>(h + 1, inf));
//                 }
//             }
//         }
//     }
//     return THRESH[N].size() - 2;
// }


int Dense()
{
    THRESH[0].insert(pair<int, int>(0, einf));
    THRESH[0].insert(pair<int, int>(1, inf));
    for (int i = 1; i <= k - 1; ++i)
    {
        THRESH[i] = THRESH[i - 1];
    }
    for (int i = k; i <= N; ++i)
    {
        THRESH[i] = THRESH[i - 1];
        int sz=THRESH[i - 1].size();
        for (int h = 1; h <= sz-1; ++h)
        {
            int j1 = THRESH[i - k][h - 1];
            int x = MAXN;
            for (int j = j1 + k; j <= N; j++)
            {
                if (is_equal(i-1,j-1,k))
                {
                    x = j;
                    break;
                }
            }
            int j2 = THRESH[i][h];
            if (x != (MAXN))
            {
                if(j2>x)
                THRESH[i][h] = x;
                if (j2 == inf)
                {
                    THRESH[i].insert(pair<int, int>(h + 1, inf));
                }
            }
        }
    }
    return THRESH[N].size() - 2;
}

int get_res_test_Dense(int K, int SIZE, char *STR_1, char *STR_2) {
    N = SIZE;
    k = K;
    clock_t start_t, end_t;
    start_t = clock();
    preprocess(STR_1,STR_2,k);
    end_t = clock();
    clock_Dense_matchlist += (end_t - start_t);
    return Dense();
}