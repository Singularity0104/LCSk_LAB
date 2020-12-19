// #include <iostream>
// #include <map>
// #include <cstring>
// #include <string.h>
// #include <time.h>
// #include "test.h"
// using namespace std;

// static int N;
// static int k;
// static int matchlist[10000][10000];
// #define inf 0x3fffffff
// #define einf -100
// static map<int, int> THRESH[10000];
// void get_match_list(char *A, char *B)
// {
//     // for(int i = 0; i < 10000; i++) {
//     //     for(int j = 0; j < 10000; j++) {
//     //         matchlist[i][j] = 0;
//     //     }
//     // }
//     memset(matchlist, 0, sizeof(matchlist));
//     for (int i = 0; i < N - k+1; i++)
//     {
//         for (int j = 0; j < N - k+1; j++)
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
// int find_h(int i, int j)
// {
//     int sz = THRESH[i].size();
//     for (int x = 0; x < sz; x++)
//     {
//         if(THRESH[i][x]<j) continue;
//         else return x-1;
//     }
// }

// int CR()
// {
//     THRESH[0].insert(pair<int, int>(0, einf));
//     THRESH[0].insert(pair<int, int>(1, inf));
//     for (int i = 1; i <=k - 1; ++i)
//     {
//         THRESH[i] = THRESH[i - 1];
//     }
//     for (int i = k; i <=N; ++i)
//     {
//         THRESH[i] = THRESH[i - 1];
//         for (int j = k ; j <= N; ++j)
//         {
//             if (matchlist[i-1][j-1] == 1)
//             {   
//                 int h=find_h(i-k,j-k+1);
//                 int j2=THRESH[i][h+1];
//                 if(j<j2)
//                 {
//                     THRESH[i].erase(h+1);
//                     THRESH[i].insert(pair<int ,int>(h+1,j));
//                     if(j2==inf)
//                     {   
//                         THRESH[i].insert(pair<int ,int>(h+2,j2));
//                     }
//                 }
//             }
//             else
//                 continue;
//         }
//     }
//     return THRESH[N].size()-2;
// }
// // int dense(){
// //     THRESH[0].insert(pair<int, int>(0, einf));
// //     THRESH[0].insert(pair<int, int>(1, inf));
// //     for (int i = 1; i <=k - 1; ++i)
// //     {
// //         THRESH[i] = THRESH[i - 1];
// //     }
// //     for (int i = k; i <=N; ++i)
// //     {
// //         THRESH[i] = THRESH[i - 1];
// //     for(int h = 0;h <= THRESH[i-1].size() - 1;++h){
// //         int j1 = THRESH[i- k][h - 1];
// //         int x;
// //         for (int j = j1+k-1; j <= N; ++j)
// //         {
// //             if (matchlist[i-1][j-1] == 1)
// //             {  x=j; break;}}
// //         int j2 = THRESH[i][h];
// //         if(x < j2){
// //             THRESH[i][h] = x;
// //             if(j2 == inf){
// //                 THRESH[i].insert(pair<int,int>(h+1,inf));
// //             }
// //         }
// //     }}
// //     return THRESH[N].size() - 2;
// // }   
// // int main()
// // {
// //     char *A = "AAAAAA";
// //     char *B = "AAAAAA";
// //     N = 6;
// //     k = 2;
// //     get_match_list(A, B);
// //     cout<<dense()<<endl;
// // }
// int get_res_test_02(int K, int SIZE, char *STR_1, char *STR_2) {
//     N = SIZE;
//     k = K;
//     clock_t start_t, end_t;
//     start_t = clock();
//     get_match_list(STR_1, STR_2);
//     end_t = clock();
//     clock_02_matchlist += (end_t - start_t);
//     return CR();
// }


#include <iostream>
#include <map>
#include <cstring>
#include <string>
#include "test.h"
using namespace std;

static int N;
static int k;
static int matchlist[10000][10000];
#define inf 0x3fffffff
#define einf -100
static map<int, int> THRESH[10000];
void get_match_list(char *A, char *B)
{
    memset(matchlist, 0, sizeof(matchlist));
    for (int i = 0; i < N - k + 1; i++)
    {
        for (int j = 0; j < N - k + 1; j++)
        {
            int now_i = i;
            int now_j = j;

            while (A[now_i] == B[now_j])
            {
                if (now_j - j == k - 1)
                {
                    matchlist[i + k - 1][j + k - 1] = 1;
                    break;
                }
                now_i++;
                now_j++;
            }
        }
    }
}
int find_h(int i, int j)
{

    int sz = THRESH[i].size();
    int left = 0;
    int right = sz-1;
    int mid = (left + right) / 2;
    // for (int x = 0; x < sz; x++)
    // {
    //     if (THRESH[i][x] < j) {}
    //     else
    //         return x - 1;
    // }
    while (!(THRESH[i][mid] < j && THRESH[i][mid + 1] >= j))
    {
        if (THRESH[i][mid + 1] < j)
        {
            left = mid;
            mid = (left + right) / 2;
        }
        if (THRESH[i][mid] >= j)
        {
            right = mid;
            mid = (left + right) / 2;
        }
    }
    return mid;
}

int CR()
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
        for (int j = k; j <= N; ++j)
        {
            if (matchlist[i - 1][j - 1] == 1)
            {
                int sz = THRESH[i-k].size();
                int left = 0;
                int right = sz - 1;
                int mid = (left + right) / 2;
                while (!(THRESH[i-k][mid] < j-k+1 && THRESH[i-k][mid + 1] >= j-k+1))
                {
                    if (THRESH[i-k][mid + 1] < j-k+1)
                    {
                        left = mid;
                        mid = (left + right) / 2;
                    }
                    if (THRESH[i-k][mid] >= j-k+1)
                    {
                        right = mid;
                        mid = (left + right) / 2;
                    }
                }
                int h = mid;
                int j2 = THRESH[i][h + 1];
                if (j < j2)
                {
                    THRESH[i][h + 1] = j;
                    if (j2 == inf)
                    {
                        THRESH[i].insert(pair<int, int>(h + 2, j2));
                    }
                }
            }
        }
    }
    return THRESH[N].size() - 2;
}
/* int dense()
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
        for (int h = 1; h <= THRESH[i - 1].size() - 1; ++h)
        {
            int j1 = THRESH[i - k][h - 1];
            int x = MAXN;
            for (int j = j1 + k - 1; j <= N; ++j)
            {
                if (matchlist[i - 1][j - 1] == 1)
                {
                    x = j;
                    break;
                }
            }

            int j2 = THRESH[i][h];
            if (x != (MAXN))
            {
                THRESH[i][h] = x;
                if (j2 == inf)
                {
                    THRESH[i].insert(pair<int, int>(h + 1, inf));
                }
            }
        }
    }
    return THRESH[N].size() - 2;
} */
int get_res_test_02(int K, int SIZE, char *STR_1, char *STR_2) {
    N = SIZE;
    k = K;
    clock_t start_t, end_t;
    start_t = clock();
    get_match_list(STR_1, STR_2);
    end_t = clock();
    clock_02_matchlist += (end_t - start_t);
    return CR();
}