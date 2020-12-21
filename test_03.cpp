#include <iostream>
#include <cstdio>
#include <cstring>
#include <map>
#include <ctime>
#include "test.h"
using namespace std;

#define inf 0x3fffffff
#define einf -1
#define END -2

static int N;
static int k;
static int *MATCHLIST;
// static map<int,int> THRESH[10001];             // < h, j >
static map<int,int> *THRESH;

static char *get_k_string(char *str, int index) {
    if(index < k - 1) {
        return NULL;
    }
    return &str[index - k + 1];
}
static void compute_MATCHLIST(char *str_1, char *str_2) {
    char *substring_A;
    char *substring_B;
    for(int i = 0; i < N; i++) {
        substring_A = get_k_string(str_1, i);
        int colomn = 0;
        if(substring_A != NULL) {
            for(int j = k - 1; j < N; j++) {
                substring_B = get_k_string(str_2, j);
                int cmp = 1;
                for(int z = 0; z < k; z++) {
                    if(substring_A[z] != substring_B[z]) {
                        cmp = 0;
                    }
                }
                if(cmp == 1) {
                    MATCHLIST[i * (N + 1) + colomn] = j;
                    colomn++;
                }
            }
        }
         MATCHLIST[i * (N + 1) + colomn] = END;
    }
    return;
}

static int32_t f_cr() {
    THRESH[0][0] = einf;    THRESH[0][1] = inf;
    for(int i = 1; i <= k - 1; i++) THRESH[i] = THRESH[i-1];
    for(int i = k; i <= N; i++) {
        THRESH[i] = THRESH[i-1];
        int colomn = 0;
        while (true) {
            int x = MATCHLIST[(i - 1) * (N + 1) + colomn++];
            if(x == END) break;
            map<int,int>::iterator p = THRESH[i-k].end();   p--;
            for(p; p != THRESH[i-k].begin(); p--) {
                if(p->second < x - k + 1) break;
            }
            int h = p->first;
            int j2 = THRESH[i][h+1];
            if(x < j2) {
                THRESH[i][h+1] = x;
                if(j2 == inf) THRESH[i][h+2] = inf;
            }
        }
    }
    return THRESH[N].size() - 2;
}

int get_res_test_03(int K, int SIZE, char *STR_1, char *STR_2) {
    k = K; N = SIZE; clock_t start_t, end_t;
    MATCHLIST = new int [N * (N + 1)];
    THRESH = new map<int,int>[N + 1];
    start_t = clock();
    compute_MATCHLIST(STR_1, STR_2);
    end_t = clock();
    clock_03_matchlist += (end_t - start_t);
    int ans_cr = f_cr();
    delete [] MATCHLIST;
    return ans_cr;
}