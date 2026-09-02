#include<bits/stdc++.h>
using namespace std;

#define int long long
#define endl '\n'

const int MAXN=1e5+10;

vector<int> e[MAXN], ne[MAXN]; // e: 原图, ne: 缩点后的新图 (DAG)
int val[MAXN];     // 原图中每个点的权值

// --- Tarjan 核心数组 ---
int dfn[MAXN], low[MAXN], timestamp;
int stk[MAXN], top;
bool in_stk[MAXN];

// --- 缩点后新图信息 ---
int id[MAXN];      // id[u]: 原节点 u 属于第几个超级大点
int sum[MAXN];     // sum[i]: 第 i 个超级大点的权值和
int scc_cnt;       // 超级大点总数

// ================= 1. Tarjan 找 SCC =================
void tarjan(int u)
{
    dfn[u] = low[u] = ++timestamp;
    stk[++top] = u;
    in_stk[u] = true;
    
    for(int v : e[u])
    {
        if(!dfn[v])
        {
            tarjan(v);
            low[u] = min(low[u], low[v]);
        }
        else if(in_stk[v]) // 遇到还在栈中的点，说明成环
        {
            low[u] = min(low[u], dfn[v]);
        }
    }
    
    // 发现 SCC 的最高点，开始弹栈打包
    if(dfn[u] == low[u])
    {
        ++scc_cnt;
        int y;
        do {
            y = stk[top--];
            in_stk[y] = false;
            id[y] = scc_cnt;
            sum[scc_cnt] += val[y]; // 权值累加给超级大点
        } while(y != u);
    }
}

void solve()
{
    /* 实战调用示例
    int n, m; cin >> n >> m;
    for(int i = 1; i <= n; i++) cin >> val[i];
    
    for(int i = 1; i <= m; i++)
    {
        int u, v; cin >> u >> v;
        e[u].push_back(v);
    }
    
    // 1. 图可能不连通，必须遍历每个点跑 Tarjan
    for(int i = 1; i <= n; i++)
        if(!dfn[i]) tarjan(i);
        
    // 2. 核心：遍历原图所有边，建立 DAG 新图
    for(int u = 1; u <= n; u++)
    {
        for(int v : e[u])
        {
            int a = id[u], b = id[v];
            if(a != b) // 不在同一个 SCC 内，才连边
            {
                ne[a].push_back(b);
                // in_degree[b]++; // 若需拓扑排序，顺手统计入度
            }
        }
    }
    
    // 3. 在 ne 图(DAG)上跑 DP 或 拓扑排序...
    */
}