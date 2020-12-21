#include <iostream>
#include <cstdio>
#include <cstring>
#include <map>
#include <ctime>
using namespace std;

#define inf 0x3fffffff
#define einf -1

int N;
int k;
char A[10000];
char B[10000];
char tmp[20010];
char processbar[51];
int matchlist[10000][10000];
map<int,int> THRESH[10001];             // < h, j >

void PrintProcess(int N, unsigned int percent) {
    /* 进度条缓冲区 */
    char processbar[51] = {0};
    /* 初始化进度条 */
    memset(processbar, '-', 50);
    /* 绘制正常进度条 */
    if (percent <= 100) {
        memset(processbar, '#', percent / 2);
        printf("(N,%4d):\t[%s] %d%%\r", N, processbar, percent);
    }/* 绘制完成后的进度条 */
    else {
        memset(processbar, '#', 50);
        printf("(N,%4d):\t[%s] Done\n", N, processbar);
    }
    fflush(stdout);
}
void p(int i) {
    printf("%d:\t", i);
    map<int,int>::iterator qwq = THRESH[i].begin();
    for(qwq; qwq != THRESH[i].end(); qwq++) {
        printf("<%d, %d>\t", qwq->first, qwq->second);
    }
    puts("");
}
void p_ml() {
    for(int i(0); i < N; i++) {
        for(int j(0); j < N; j++) {
            cout << matchlist[i][j];
        }
        puts("");
    }
}
void get_match_list(char * A,char * B) {
    for (int i = 0; i <= N - k; i++) {
        for(int j = 0; j <= N - k; j++) {
            int now_i = i, now_j = j;
            while(A[now_i] == B[now_j]) {
                if(now_j - j == k - 1) { matchlist[i+k-1][j+k-1] = 1; break; }
                now_i++; now_j++;
            }
        }
    }
}
void f_cr() {
    THRESH[0].insert(pair<int,int>(0, einf));
    THRESH[0].insert(pair<int,int>(1, inf));
    p(0);
    for(int i = 1; i <= k - 1; i++) THRESH[i] = THRESH[i-1];
    for(int i = k; i <= N; i++) {
        THRESH[i] = THRESH[i-1];
        for(int j = k; j <= N; j++) {
            if(matchlist[i-1][j-1] == 1) {
                int x = j - 1;
                map<int,int>::iterator qwq; qwq = THRESH[i].end(); qwq--;
                for(qwq; qwq != THRESH[i].begin(); qwq--) {
                    if(qwq->second < x - k + 1) break;
                }
                int j1 = qwq->second; int h = qwq->first;
                int j2 = THRESH[i][h+1];
                if(x < j2) {
                    THRESH[i].erase(h+1);
                    THRESH[i][h+1] = x;
                    if(j2 == inf) THRESH[i][h+2] = inf;
                }
            }
        }
        p(i);
    }
}

int main() {
    // srand(0); FILE *wp = fopen("qwq.txt", "w+");
    // for(N = 1; N < 4096; N *= 2) {
    //     for(int i(0); i < N; i++) { A[i] = rand() % ('z'-'a' + 1) + 'a'; B[i] = rand() % ('z'-'a' + 1) + 'a'; }
    //     sprintf(tmp, "N: %d:\n", N); fputs(tmp, wp);
    //     sprintf(tmp, "A: %s\nB: %s\n", A, B); fputs(tmp, wp);
    //     for(k = 1; k < N; k++) {
    //         get_match_list(A, B);
    //         clock_t start = clock();
    //         f_cr();
    //         unsigned long ans_cr = THRESH[N].size() - 2;
    //         clock_t t_cr = clock() - start;
    //         for(int j(0); j <= N; j++) THRESH[j].clear();
    //         sprintf(tmp, "k: %d:\t[ans = %lu]. (%ld ms)\n", k, ans_cr, t_cr); fputs(tmp, wp);
    //         PrintProcess(N, k * 100 / N);
    //     }
    //     fputs("\n", wp);
    //     PrintProcess(N, 101);
    // }
    // fclose(wp);
    N = 6; k = 1;
    sprintf(A, "bwmgrr");
    sprintf(B, "mbfxbu");
    get_match_list(A, B);
    f_cr();
    cout << THRESH[N].size() - 2 << endl;
    return 0;
}