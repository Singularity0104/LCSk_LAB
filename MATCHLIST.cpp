#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <algorithm>
#include <assert.h>
#include "MATCHLIST.hpp"

using namespace std;

// #define DEBUG

/* S中的后缀 */
// struct suffix {
//     int group_id; // 组号 组内的LCP都大于等于k
//     int start_pos; // 在B#A中的起始索引
//     int fa_pos;
//     int fb_pos;
//     int ng_pos;
// };

static const int test_str_len = 500; // 测试字符串的长度
static const int inf = 0x3f3f3f3f;   // infinite
static const int N = 1000000;        // memory size
static int len_A, len_B, len;        // 全局变量 传入串的长度

static int wa[N], wb[N], wv[N], wss[N]; // 用来存东西
static int str[N], sa[N];               // 存放要计算后缀数组的字符串
static int height[N];                   // height[i] = LCP(sa[i], sa[i-1])
static int buf[N];                      // 用于测试

suffix S[1000000]; // S数组
suffix X[1000000]; // X数组

/* transfer idx from string to sub string. */
int str2ss(int idx, int cnt_1);

/* transfer idx from sub string to string. */
int ss2str(int ss_idx, int cnt_1);

/* 判断连续3个字符是否相等 */
bool compare(int *str, int idx_1, int idx_2);

/* 比较三种类型的后缀, 使用下标表示后缀 */
bool cmp_suffix(int mod, int *str, int idx_1, int idx_2);

/* 利用string进行基数排序(不可复用) */
void sort(int *str, int *suffixs, int *result, int n, int m);

/* 连接字符串 */
char *concat(const char *A, const char *B);

/* 主函数 给定string求解后缀数组, 存入sa */
void solve(int *str, int *sa, int n, int m);

/* 输入字符串 预处理后调用solve计算后缀数组 */
int *get_sa(char *str);

/* 计算height 结果存入height数组 */
void get_height(char *str);

/* 判断B#A中位置为idx的串是否位于B中 */
/* 考虑到#开头的后缀不会位于S中, 因此不用考虑# */
bool in_B(int idx, int len_B, int len_A);

/* 判断B#A中位置为idx的串是否位于A中 */
bool in_A(int idx, int len_B, int len_A);

/* 生成S 并进行排序 然后存入X中 */
void sort_S(int k);

/* 以下是用于测试用的函数 */
bool is_equal_naive(const char *A, const char *B, int idx_A, int idx_B, int k);
/* 根据字典序比较两个字符串 */
bool cmp_str(char *str_1, char *str_2);
/* 测试能否正确判断kstring是否相等 */
void query_test(char *A, char *B);
/* 生成随机字符串 */
char *rnd_str();
/* 测试height结果 */
bool test_height(char *str, int n);
/* 测试后缀数组 */
bool test_sa(char *string, int n);

/* 可以设计为接口的函数 */
/* 传入A,B 进行所有的预处理 */
void preprocess(const char *A, const char *B, int k);

/* 判断两个k string是否相等 */
bool is_equal(int idx_A, int idx_B, int k);

// used to test.
// int main() {
//     srand(time(0));
//     int test_times = 100;
//     while (test_times--) {
//         cout << test_times << endl;
//         char *A = rnd_str();
//         char *B = rnd_str();
//         query_test(A, B);
//         free(A);
//         free(B);
//     }
//     cout << "pass all the test~" << endl;
//     return 0;
// }

/* implementation */
bool compare(int *str, int idx_1, int idx_2)
{
    return str[idx_1] == str[idx_2] &&
           str[idx_1 + 1] == str[idx_2 + 1] &&
           str[idx_1 + 2] == str[idx_2 + 2];
}

bool cmp_suffix(int mod, int *str, int idx_1, int idx_2)
{
    // wv -> 存储着模为1， 2后缀的rank
    // wv[suffix(i)] = rank(suffix(i))
    if (mod == 2)
        return str[idx_1] < str[idx_2] ||
               str[idx_1] == str[idx_2] && cmp_suffix(1, str, idx_1 + 1, idx_2 + 1);
    else
        return str[idx_1] < str[idx_2] || // BUG: 最后一个后缀如果是模0的，那么它无法借助下一个模1后缀比较。
               str[idx_1] == str[idx_2] && wv[idx_1 + 1] < wv[idx_2 + 1];
}

int str2ss(int idx, int cnt_1)
{
    /* 
     * if idx mod 3 == 1, let idx = 3 * k + 1
     * sub_idx = floor(idx / 3) --> 前面的组数
     * if idx mod 3 == 2, let idx = 3 * k + 2
     * sub_idx = floor((idx - 1) / 3) = floor(idx / 3)
     * 由于前面有cnt_1个模为1的后缀，所以要加上cnt_1
     */
    if (idx % 3 == 1)
        return idx / 3;
    else
        return idx / 3 + cnt_1;
}

int ss2str(int ss_idx, int cnt_1)
{
    if (ss_idx < cnt_1)
        return ss_idx * 3 + 1;
    else
        return (ss_idx - cnt_1) * 3 + 2;
}

void sort(int *str, int *suffixs, int *result, int n, int m)
{
    // n -> 后缀个数, m -> “桶”的大小
    // wv -> 存储键值
    for (int i = 0; i < n; i++)
        wv[i] = str[suffixs[i]];
    // wss -> “桶”
    for (int i = 0; i < m; i++)
        wss[i] = 0;
    // 把键值放入桶内
    for (int i = 0; i < n; i++)
        wss[wv[i]]++;
    // 累加计算排名
    for (int i = 1; i < m; i++)
        wss[i] += wss[i - 1];
    // 进行最后的排序, 先根据键值计算排名rank, 然后把后缀存入result[rank]
    for (int i = n - 1; i >= 0; i--)
        result[--wss[wv[i]]] = suffixs[i];
}

char *concat(const char *A, const char *B)
{
    char *result = (char *)malloc(len + 1);
    /* 先copy str B */
    strncpy(result, B, len_B + 1);
    /* 插入 # */
    result[len_B] = 120; // 选择一个最大值就行，可以起到中断的作用 只要保证LCP计算不出错就行，因为后面会根据st pos排序
    /* 再copy str A */
    strncpy(result + len_B + 1, A, len_A + 1);
    return result;
}

void solve(int *str, int *sa, int n, int m)
{
    // n -> 字符的个数, m -> str中最大的字符
    int *ss = str + n; // sub string
    int *ssa = sa + n; // sub string's suffix array
    int cnt_0 = 0;     // 模为0后缀的个数
    /* if n = 3k, (n + 1)/3 = k
     * if n = 3k + 1, (n + 1)/3 = k
     * if n = 3k + 2, (n + 1)/3 = k + 1
     * 各种情况下都恰好为(n + 1)/3
     */
    int cnt_1 = (n + 1) / 3; // 模为1后缀的个数
    int cnt_12 = 0;
    str[n] = str[n + 1] = 0; // 补0 方便n = 3k + 1的时候最后一个模1后缀进行比较
    // 取出模3为1或者2的后缀，存入wa数组
    for (int i = 0; i < n; i++)
        if (i % 3 != 0)
        {
            wa[cnt_12] = i;
            cnt_12++;
        }
    // str + i -> 利用偏移, 取出后缀的第i+1个字符
    // 排序结果放在wb数组中.
    // wb[i] -> rank为i的后缀
    sort(str + 2, wa, wb, cnt_12, m);
    sort(str + 1, wb, wa, cnt_12, m);
    sort(str, wa, wb, cnt_12, m);
    // 计算sub string的每个字符的rank(就是string的后缀根据3个字符排序后的rank)
    // 因为每个字符都对应着原来的一个后缀
    int rak = 0;                    // rank从0开始
    ss[str2ss(wb[0], cnt_1)] = rak; // 排名第一的后缀rank为0
    rak++;
    for (int i = 1; i < cnt_12; i++)
        if (compare(str, wb[i], wb[i - 1])) // 如果俩后缀相等，那么rank相同
            ss[str2ss(wb[i], cnt_1)] = rak - 1;
        else
            ss[str2ss(wb[i], cnt_1)] = rak++;
    if (rak < cnt_12) // rak == cnt_12表示3个字符就已经能够将所有后缀进行排序
        solve(ss, ssa, cnt_12, rak);
    else
        for (int i = 0; i < cnt_12; i++) // 根据rank将模1，2的后缀排好
            ssa[ss[i]] = i;
    for (int i = 0; i < cnt_12; i++)
        if (ssa[i] < cnt_1) // 找到一个模为1的后缀，对应一个模为0的后缀
        {
            wb[cnt_0] = ssa[i] * 3; // 它的索引正好是ssa[i] * 3
            cnt_0++;
        }
    if (n % 3 == 1) // 如果n = 3k + 1, 那么会有一个遗漏的
    {
        wb[cnt_0] = n - 1; // 它的索引就是n-1(最后一个)
        cnt_0++;
    }
    sort(str, wb, wa, cnt_0, m); // 使用第一个字符进行基数排序, 结果放在wa中
    for (int i = 0; i < cnt_12; i++)
    {
        wb[i] = ss2str(ssa[i], cnt_1); // 恢复索引 BUG: 出现重复值
        wv[wb[i]] = i;                 // 模1，2后缀的rank数组
    }

    // merge
    int i = 0, j = 0, idx = 0;
    while (i < cnt_0 && j < cnt_12)
    {
        if (cmp_suffix(wb[j] % 3, str, wa[i], wb[j]))
        {
            sa[idx] = wa[i];
            i++;
            idx++;
        }
        else
        {
            sa[idx] = wb[j];
            j++;
            idx++;
        }
    }
    // 处理剩下的
    for (; i < cnt_0; idx++)
        sa[idx] = wa[i++];
    for (; j < cnt_12; idx++)
        sa[idx] = wb[j++];
}

char *rnd_str()
{
    char *str = (char *)malloc(sizeof(char) * (test_str_len + 1));
    for (int i = 0; i < test_str_len; i++)
        str[i] = 'a' + random() % 10;
    str[test_str_len] = '\0'; // 忘记加结束符了！！！！！！！！！！啊啊啊啊啊啊啊啊啊啊啊
    return str;
}

bool test_height(char *str, int n)
{
    for (int i = 1; i <= n; i++)
    {
        int h = height[i];
        int res = strncmp(str + sa[i], str + sa[i - 1], h);
        if (res != 0)
            return false;
    }
    return true;
}

bool test_sa(char *string, int n)
{
    // 确保没有重复的sa[i]出现
    memset(buf, 0, sizeof(buf));
    for (int i = 0; i <= n; i++)
    {
        buf[sa[i]]++;
        if (buf[sa[i]] > 1)
        {
            return false;
        }
    }

    // 确保顺序正确
    for (int i = 0; i <= n - 1; i++)
        if (!cmp_str(string + sa[i], string + sa[i + 1]))
        {
            cout << string + sa[i] << endl;
            cout << string + sa[i + 1] << endl;
            return false;
        }
    return true;
}

void get_height(char *str)
{
    int n = len;
    int *rank = (int *)malloc(sizeof(int) * (n + 5));
    // rank[i] -> 后缀i的排名 BUG: rank有重复值 因为后面补0，多了个字符串，所以sa的第一位被占用。
    for (int i = 0; i <= n; i++)
        rank[sa[i]] = i;
    int h = 0;
    // 遍历所有的后缀i
    for (int i = 0; i <= n; i++)
    {
        // 如果后缀i不是最后一名
        if (rank[i] > 0)
        {
            // 后缀j在后缀数组中排在后缀i的后面
            int j = sa[rank[i] - 1];
            // 计算他们的LCP 存入h
            while (str[i + h] == str[j + h])
                h++;
            height[rank[i]] = h;

            // 下一次比较只要从h-1开始 因为后缀数组相邻之间具有相似性
            if (h > 0)
                h--;
        }
    }
    free(rank);
}

bool cmp_str(char *str_1, char *str_2)
{
    int len_1 = strlen(str_1);
    int len_2 = strlen(str_2);
    int check_len = len_1 > len_2 ? len_1 : len_2;
    for (int i = 0; i < check_len; i++)
        if (str_1[i] < str_2[i])
            return true;
        else if (str_1[i] > str_2[i])
            return false;
    // 全都相等
    return len_1 < len_2;
}

bool in_B(int idx, int len_B, int len_A)
{
    if (idx < len_B)
        return true;
    return false;
}

bool in_A(int idx, int len_B, int len_A)
{
    return !in_B(idx, len_B, len_A);
}

void sort_S(int k)
{
    // len -> 字符串的长度
    int id = 1;  // group id
    int idx = 0; // idx in S
    int i = 0;
    int this_group_k = height[0]; // group的k值
    int st_idx = 0, ed_idx;       // group的起始位置与结束位置
    while (true)
    {
        // 跳过不合格的group
        while (height[i] < k && i <= len)
            i++;
        // 设置开始位置
        st_idx = i;
        this_group_k = height[i];
        // 寻找结束的位置 >= 都可以
        while (height[i] >= k && i <= len)
            i++;
        ed_idx = i;
        // BUG: 起始位置超出范围 才 等价于没有组需要插入
        if (st_idx > len)
            break;
        // 将该组插入S
        for (int j = st_idx - 1; j < ed_idx; j++)
        {
            suffix temp;
            temp.group_id = id;
            temp.start_pos = sa[j];
            S[idx] = temp;
            idx++;
        }
        id++;
        st_idx = i;
    }

    /* 开始基数排序 */
    suffix *helper = (suffix *)malloc(idx * sizeof(suffix));
    /* 首先用start pos排序 */
    memset(wss, 0, sizeof(int) * (len + 1000));
    for (int i = 0; i < idx; i++)
    {
        int pos = S[i].start_pos;
        wss[pos]++;
    }
    for (int i = 1; i < len; i++)
        wss[i] += wss[i - 1];
    for (int i = idx - 1; i >= 0; i--)
    {
        int rank = wss[S[i].start_pos] - 1;
        helper[rank] = S[i];
        wss[S[i].start_pos]--;
    }

    /* 再用group_id排序 */
    memset(wss, 0, sizeof(int) * (len + 1000));
    for (int i = 0; i < idx; i++)
    {
        int id = helper[i].group_id;
        wss[id]++;
    }
    for (int i = 1; i < id; i++)
        wss[i] += wss[i - 1];
    for (int i = idx - 1; i >= 0; i--)
    {
        int rank = wss[helper[i].group_id] - 1;
        S[rank] = helper[i];
        wss[helper[i].group_id]--;
    }
    free(helper);

    /* 计算每一个suffix的fa pos, fb pos, ng pos. */
    st_idx = 0, ed_idx = 0;
    int this_id = 1;
    /* 每个group至少有2个成员 */
    while (true)
    {
        // 找到结束位置
        while (S[ed_idx].group_id == this_id && ed_idx < idx)
            ed_idx++;
        // 找到pos 默认都是结束的位置
        int fa_pos = ed_idx, fb_pos = ed_idx, ng_pos = ed_idx;
        for (int i = st_idx; i < ed_idx; i++)
            if (in_B(S[i].start_pos, len_B, len_A))
            {
                fb_pos = i;
                break;
            }
        for (int i = st_idx; i < ed_idx; i++)
            if (in_A(S[i].start_pos, len_B, len_A))
            {
                fa_pos = i;
                break;
            }
        for (int i = st_idx; i < ed_idx; i++)
        {
            S[i].fa_pos = fa_pos;
            S[i].fb_pos = fb_pos;
            S[i].ng_pos = ng_pos;
            X[S[i].start_pos] = S[i];
        }
        // 修改起始位置
        if (ed_idx >= idx)
            break;
        st_idx = ed_idx;
        this_id = S[ed_idx].group_id;
    }

#ifdef DEBUG
    cout << "idx num: " << idx << endl;
    for (int i = 0; i < idx; i++)
    {
        cout << "idx: " << i << "    ";
        cout << "(" << S[i].group_id << "," << S[i].start_pos
             << ")"
             << "   ";
        cout << "fb_pos: " << S[i].fb_pos << "   ";
        cout << "fa_pos: " << S[i].fa_pos << "   ";
        cout << "ng_pos: " << S[i].ng_pos << "   " << endl;
    }
#endif
}

void preprocess(const char *A, const char *B, int k)
{
    // 处理全局变量
    len_A = strlen(A);
    len_B = strlen(B);
    len = len_A + len_B + 1;
    // 连接 BA
    char *B_A = concat(A, B);
    // 计算后缀数组
    get_sa(B_A);
#ifdef DEBUG
    bool check = test_sa(B_A, len);
    assert(check == true);
#endif
    // 计算height
    get_height(B_A);
#ifdef DEBUG
    check = test_height(B_A, len);
    assert(check == true);
#endif
    // 计算X
    memset(X, -1, sizeof(suffix) * (len + 10));
    sort_S(k); // 计算好X， X就是matchlist
    free(B_A);
}

bool is_equal(int idx_A, int idx_B, int k)
{
    // 计算起始位置
    int st_pos_A = idx_A - k + 1 + len_B + 1;
    // 如果S中不存在
    if (X[st_pos_A].start_pos == -1)
        return false;
    // 找到S中与其匹配的所有串
    int st_idx = X[st_pos_A].fb_pos;
    int ed_idx = X[st_pos_A].fa_pos;
#ifdef DEBUG
    if (idx_A == 1 && idx_B == 14)
    {
        int st_pos_B = idx_B - k + 1;
        cout << "st_pos_A: " << st_pos_A << endl;
        cout << str + st_pos_A << endl;
        cout << "st_pos_B: " << st_pos_B << endl;
        cout << str + st_pos_B << endl;
        cout << "st_idx in S: " << st_idx << endl;
        cout << "ed_idx in S: " << ed_idx << endl;
    }

#endif
    // 进行查找
    for (int idx = st_idx; idx < ed_idx; idx++)
        if (S[idx].start_pos == idx_B - k + 1)
            return true;
    return false;
}

bool is_equal_naive(const char *A, const char *B, int idx_A, int idx_B, int k)
{
    for (int i = 0; i < k; i++)
        if (A[idx_A - i] != B[idx_B - i])
            return false;
    return true;
}

void query_test(char *A, char *B)
{
    for (int k = 1; k <= 50; k++)
    {
        preprocess(A, B, k);
        for (int idx_A = k - 1; idx_A < test_str_len; idx_A++)
            for (int idx_B = k - 1; idx_B < test_str_len; idx_B++)
            {
                bool ans = is_equal_naive(A, B, idx_A, idx_B, k);
                bool my_ans = is_equal(idx_A, idx_B, k);
                if (my_ans != ans)
                {
                    cout << endl;
                    cout << "go to debug QWQ" << endl;
                    cout << "this is information:" << endl;
                    cout << "k: " << k << endl;
                    cout << "idx_A: " << idx_A << endl;
                    cout << A[idx_A] << endl;
                    cout << "idx_B: " << idx_B << endl;
                    cout << B[idx_B] << endl;
                    cout << ans << " ";
                    cout << my_ans << endl;
                    sleep(100);
                }
            }
    }
}

int *get_sa(char *string)
{
    int max_val = 100;
    for (int i = 0; i < len; i++)
    {
        str[i] = string[i];
        max_val = string[i] > max_val ? string[i] : max_val;
    }
    // 当模0的后缀是最后一个后缀时，+1可以帮助它获得一个辅助模1后缀
    solve(str, sa, len + 1, max_val + 100); // 开大一点 保险
    return sa;
}