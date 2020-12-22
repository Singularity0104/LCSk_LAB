#include <iostream>
#include <map>
#include "test.hpp"
using namespace std;

static int k;
static char *str_1;
static char *str_2;
static int size_1;
static int size_2;
static int res;
static int *DP_mat;

static void DP()
{
    size_1++;
    size_2++;
    DP_mat = new int[size_1 * size_2];
    for (int i = 0; i < size_1; i++)
    {
        for (int j = 0; j < size_2; j++)
        {
            if (i < k || j < k)
            {
                DP_mat[i * size_2 + j] = 0;
                continue;
            }
            int flag = 1;
            for (int z = 1; z <= k; z++)
            {
                if (str_1[i - z] != str_2[j - z])
                {
                    flag = 0;
                    break;
                }
            }
            if (flag == 1)
            {
                DP_mat[i * size_2 + j] = DP_mat[(i - k) * size_2 + j - k] + 1;
            }
            else
            {
                if (DP_mat[(i - 1) * size_2 + j] > DP_mat[i * size_2 + j - 1])
                {
                    DP_mat[i * size_2 + j] = DP_mat[(i - 1) * size_2 + j];
                }
                else
                {
                    DP_mat[i * size_2 + j] = DP_mat[i * size_2 + j - 1];
                }
            }
        }
    }
    res = DP_mat[size_1 * size_2 - 1];
    delete[] DP_mat;
}

int get_res_test_DP(int K, int SIZE, char *STR_1, char *STR_2)
{
    k = K;
    size_1 = SIZE;
    size_2 = SIZE;
    str_1 = STR_1;
    str_2 = STR_2;
    DP();
    return res;
}