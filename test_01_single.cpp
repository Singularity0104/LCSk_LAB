#include <iostream>
#include <cstring>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <map>
// #include "test.h"
using namespace std;
#define DEBUG 1
#define PINF INT_MAX
#define END -2
int k;
char *str_1;
char *str_2;
int size_1;
int size_2;

int *MATCHLIST;
map<int, int> *THRESH;
int res;

char *get_k_string(char *str, int index);
void compute_MATCHLIST();
void init();
void end();
void LCSk();

int main()
{
    LCSk();
    return 0;
}

char *get_k_string(char *str, int index) {
    if(index < k - 1) {
        return NULL;
    }
    return &str[index - k + 1];
}

void compute_MATCHLIST() {
    char *substring_A;
    char *substring_B;
    for(int i = 0; i < size_1; i++) {
        substring_A = get_k_string(str_1, i);
        int colomn = 0;
        if(substring_A != NULL) {
            for(int j = k - 1; j < size_2; j++) {
                substring_B = get_k_string(str_2, j);
                int cmp = 1;
                for(int z = 0; z < k; z++) {
                    if(substring_A[z] != substring_B[z]) {
                        cmp = 0;
                    }
                }
                if(cmp == 1) {
                    MATCHLIST[i * (size_2 + 1) + colomn] = j;
                    colomn++;
                }
            }
        }
         MATCHLIST[i * (size_2 + 1) + colomn] = END;
    }
    return;
}

void init() {
    printf("please input k: ");
    scanf("%d", &k);

    printf("please input size_1: ");
    scanf("%d", &size_1);
    str_1 = new char[size_1 + 10];
    printf("please input str_1: ");
    scanf("%s", str_1);

    printf("please input size_2: ");
    scanf("%d", &size_2);
    str_2 = new char[size_2 + 10];
    printf("please input str_2: ");
    scanf("%s", str_2);

    assert(size_1 == strlen(str_1));
    assert(size_2 == strlen(str_2));
    MATCHLIST = new int [size_1 * (size_2 + 1)];
    THRESH = new map<int, int>[size_1 + 1];
    compute_MATCHLIST();
    return;
}

void end() {
    delete []MATCHLIST;
    delete []THRESH;
    return;
}

void LCSk() {
    init();
    THRESH[0][0] = -1;
    THRESH[0][1] = PINF;
    for(int i = 1; i <= k - 1; i++) {
        THRESH[i] = THRESH[i - 1];
    }
    for(int i = k; i <= size_1; i++) {
        THRESH[i] = THRESH[i - 1];
        int x;
        int colomn = 0;
        while(true) {
            x = MATCHLIST[(i - 1) * (size_2 + 1) + colomn];
            colomn++;
            if(x == END) {
                break;
            }
            int j_1 = -2;
            int h = 0;
            for(map<int, int>::iterator it = THRESH[i - k].begin(); it != THRESH[i - k].end(); it++) {
                if(it->second <= x - k && it->second > j_1) {
                    j_1 = it->second;
                    h = it->first;
                }
            }
            int j_2 = THRESH[i][h + 1];
            if(x < j_2) {
                THRESH[i][h + 1] = x;
                if(j_2 == PINF) {
                    THRESH[i][h + 2] = PINF;
                }
            }
        }
    }
    res = THRESH[size_1].size() - 2;
    printf("LCSk result: %d\n", res);

    end();
}

// int get_res_test_01(int K, int SIZE, char *STR_1, char *STR_2) {
//     k = K;
//     size_1 = SIZE;
//     size_2 = SIZE;
//     str_1 = STR_1;
//     str_2 = STR_2;
//     LCSk();
//     return res;
// }
