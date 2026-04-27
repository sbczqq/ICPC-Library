#include<bits/stdc++.h>
using namespace std;
#define int long long
#define endl '\n'
using PII = pair<int, int>;

const int N = 1e6 + 10;
int tr[N][26] = { 0 }, idx = 0;
int fail[N] = { 0 };
int cnt[N] = { 0 };

// 1. 建 Trie 树
void insert(string s)
{
    int he = 0;
    for(auto ch : s)
    {
        int dd = ch - 'a';
        if(tr[he][dd] == 0) tr[he][dd] = ++idx;
        he = tr[he][dd];
    }
    cnt[he]++; // 标记结尾
}

// 2. BFS 构建 fail 指针与 Trie 图
void build()
{
    queue<int> q;
    for(int i = 0; i < 26; i++)
    {
        if(tr[0][i]) q.push(tr[0][i]);
    }

    while(!q.empty())
    {
        int u = q.front();
        q.pop();
        for(int i = 0; i < 26; i++)
        {
            if(tr[u][i])
            {
                fail[tr[u][i]] = tr[fail[u]][i];
                q.push(tr[u][i]);
            }
            else
            {
                tr[u][i] = tr[fail[u]][i]; // Trie 图路径压缩优化
            }
        }
    }
}

// 3. 匹配文本串 (统计包含多少种模式串)
int query(string s)
{
    int u = 0, res = 0;
    for(auto ch : s)
    {
        int dd = ch - 'a';
        u = tr[u][dd]; // O(1) 状态转移

        // 跳 fail 指针收集所有匹配的模式串
        for(int j = u; j > 0 && cnt[j] != -1; j = fail[j])
        {
            res += cnt[j];
            cnt[j] = -1; // 打标记防重复统计
        }
    }
    return res;
}

/* 使用说明与多组数据排雷技巧：
 * 1. 核心调用流：
 * - for(模式串) insert(s);
 * - build();
 * - query(主串);
 * 2. 初始化防 TLE (极其关键)：
 * 在多组测试数据中，切忌使用 memset 清空整个 tr 数组，必会导致超时！
 * 应严格基于使用过的节点数量 idx 进行精准清空：
 * void init() {
 * for(int i = 0; i <= idx; i++) {
 * for(int j = 0; j < 26; j++) tr[i][j] = 0;
 * fail[i] = cnt[i] = 0;
 * }
 * idx = 0;
 * }
 */
```