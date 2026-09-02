#include<bits/stdc++.h>
using namespace std;

#define int long long
#define endl '\n'

const int MAXN=5e5+10; // 注意：由于是欧拉序，ST表相关数组要开到两倍！

vector<int> e[MAXN];

// --- 欧拉序与 LCA 核心数组 ---
int dep[MAXN];         // dep[u]: 节点 u 的深度
int seq[MAXN * 2];     // seq[i]: 欧拉序中第 i 个访问的节点编号
int first[MAXN];       // first[u]: 节点 u 在欧拉序中第一次出现的下标
int tot;               // 欧拉序长度计数器

// --- ST 表 ---
int st[MAXN * 2][21];  // st[i][j]: 从下标 i 开始，长度为 2^j 的区间内，深度最小的【节点编号】
int lg[MAXN * 2];      // log2 预处理数组，用于 O(1) 查询

// ================== 1. DFS 求欧拉序 ==================
void dfs(int u, int f, int d)
{
    dep[u] = d;
    seq[++tot] = u;    // 第一次访问该节点，加入欧拉序
    first[u] = tot;    // 记录第一次出现的下标
    
    for(int v : e[u])
    {
        if(v == f) continue;
        dfs(v, u, d + 1);
        seq[++tot] = u; // 【核心】：从子节点回溯回来后，再次将自己加入欧拉序
    }
}

// ================== 2. 初始化 ST 表 ==================
// 比较两个节点哪个深度更浅，返回深度更浅的节点编号
int get_min(int u, int v) 
{
    return dep[u] < dep[v] ? u : v;
}

void build_ST()
{
    // 预处理 log2 数组
    lg[1] = 0;
    for(int i = 2; i <= tot; i++) lg[i] = lg[i / 2] + 1;
    
    // 初始化 ST 表第 0 列 (长度为 2^0 = 1 的区间)
    for(int i = 1; i <= tot; i++) st[i][0] = seq[i];
    
    // 倍增递推 ST 表
    for(int j = 1; (1 << j) <= tot; j++)
    {
        for(int i = 1; i + (1 << j) - 1 <= tot; i++)
        {
            // 区间 [i, i + 2^j - 1] 的最小深度节点，等于两个半区间的最小深度节点的较浅者
            st[i][j] = get_min(st[i][j - 1], st[i + (1 << (j - 1))][j - 1]);
        }
    }
}

// ================== 3. O(1) 查询 LCA ==================
int query_lca(int u, int v)
{
    int L = first[u], R = first[v];
    if(L > R) swap(L, R); // 保证 L <= R
    
    int k = lg[R - L + 1];
    
    // 查询区间 [L, R] 的最小深度节点，拆分为两段长度为 2^k 的重叠区间
    return get_min(st[L][k], st[R - (1 << k) + 1][k]);
}

void solve()
{
    /* 实战调用示例
    int n, m, root; 
    cin >> n >> m >> root;
    for(int i = 1; i < n; i++)
    {
        int u, v; cin >> u >> v;
        e[u].push_back(v);
        e[v].push_back(u);
    }
    
    // 1. 求欧拉序
    tot = 0;
    dfs(root, 0, 1);
    
    // 2. 建 ST 表
    build_ST();
    
    // 3. O(1) 极速响应查询
    while(m--)
    {
        int u, v; cin >> u >> v;
        cout << query_lca(u, v) << endl;
    }
    */
}