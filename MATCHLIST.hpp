#ifndef MATCHLIST_HPP
#define MATCHLIST_HPP

/* S中的后缀 */
struct suffix {
    int group_id; // 组号 组内的LCP都大于等于k
    int start_pos; // 在B#A中的起始索引
    int fa_pos;
    int fb_pos;
    int ng_pos;
};

extern suffix S[1000000];                     // S数组
extern suffix X[1000000];                     // X数组

/* 传入A,B 进行所有的预处理 */
void preprocess(const char *A, const char *B, int k);

/* 判断两个k string是否相等 */
bool is_equal(int idx_A, int idx_B, int k);

/* 生成随机字符串 */
char * rnd_str();

#endif