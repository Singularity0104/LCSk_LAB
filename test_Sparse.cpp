#include <iostream>
#include <map>
#include "test.hpp"
#include "MatchList.hpp"
using namespace std;

#define inf 0x3fffffff
#define einf -1
#define END -2

extern suffix S[1000000]; // S数组
extern suffix X[1000000]; // X数组
extern clock_t clock_Sparse_matchlist;
static int N;
static int k;
static map<int, int> *THRESH;

static char *get_k_string(char *str, int index)
{
    if (index < k - 1)
    {
        return NULL;
    }
    return &str[index - k + 1];
}

static int sparse()
{
    THRESH[0][0] = einf;
    THRESH[0][1] = inf;
    for (int i = 1; i <= k - 1; i++)
        THRESH[i] = THRESH[i - 1];
    for (int i = k; i <= N; i++)
    {
        THRESH[i] = THRESH[i - 1];
        int colomn = 0;
        int fb = X[i + N - k + 1].fb_pos;
        int fa = X[i + N - k + 1].fa_pos;
        for (int pos = fb; pos < fa; pos++)
        {
            int x = S[pos].start_pos;
            map<int, int>::iterator p = THRESH[i - k].end();
            p--;
            for (; p != THRESH[i - k].begin(); p--)
            {
                if (p->second < x)
                    break;
            }
            int h = p->first;
            int j2 = THRESH[i][h + 1];
            if (x + k - 1 < j2)
            {
                THRESH[i][h + 1] = x + k - 1;
                if (j2 == inf)
                    THRESH[i][h + 2] = inf;
            }
        }
    }
    return THRESH[N].size() - 2;
}

int get_res_test_Sparse(int K, int SIZE, char *STR_1, char *STR_2)
{
    k = K;
    N = SIZE;
    THRESH = new map<int, int>[N + 1];
    clock_t start_t, end_t;
    start_t = clock();
    preprocess(STR_1, STR_2, K);
    end_t = clock();
    clock_Sparse_matchlist += (end_t - start_t);
    int ans = sparse();
    delete[] THRESH;
    return ans;
}
