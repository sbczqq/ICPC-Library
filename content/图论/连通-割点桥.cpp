#include<bits/stdc++.h>
using namespace std;

#define int long long
#define endl '\n'

const int MAXN=1e5+10;
const int MAXM=6e5+10; // 无向图双向建边，开两倍

// --- 链式前向星 ---
// 【核心避坑】：tot 必须从 1 开始！这样成对的边编号分别是 2/3, 4/5, 偶数^1=奇数，奇数^1=偶数
int head[MAXN], to[MAXM], nxt[MAXM], tot = 1; 

void add(int u, int v)
{
    to[++tot] = v;
    nxt[tot] = head[u];
    head[u] = tot;
}

// --- Tarjan 核心数组 ---
int dfn[MAXN], low[MAXN], timestamp;
bool is_cut_vertex[MAXN]; // 记录每个点是否是割点
bool is_bridge[MAXM];     // 记录每条边是否是割边 (用边编号索引)

// in_edge 是进入节点 u 的那条边的编号
void tarjan(int u, int in_edge)
{
    dfn[u] = low[u] = ++timestamp;
    int child_cnt = 0; // 记录在 DFS 树上的实际子节点数量，专为根节点特判准备
    
    for(int i = head[u]; i; i = nxt[i])
    {
        int v = to[i];
        
        // 遇到刚刚过来的那条边 (父节点直接走过来的边)，直接跳过，防止无向图死循环死磕
        if(i == (in_edge ^ 1)) continue;
        
        if(!dfn[v])
        {
            child_cnt++;
            tarjan(v, i);
            low[u] = min(low[u], low[v]); // 用子节点的 low 更新自己
            
            // ================== 1. 判定割边 ==================
            // 如果 v 及其子树，不经过 (u, v) 这条边，连 u 都回不到 (更别提 u 的祖先)
            // 那么 (u, v) 就是割边
            if(dfn[u] < low[v])
            {
                is_bridge[i] = is_bridge[i ^ 1] = true;
            }
            
            // ================== 2. 判定割点 ==================
            // 非根节点特判：只要有一个子树 v 无法越过 u 回到 u 的祖先，u 就是割点
            if(in_edge != 0 && dfn[u] <= low[v])
            {
                is_cut_vertex[u] = true;
            }
        }
        else 
        {
            // 遇到返祖边，更新 low (注意：这里用的是 dfn[v] 而不是 low[v])
            low[u] = min(low[u], dfn[v]);
        }
    }
    
    // 根节点特判：如果 u 是当前连通块 DFS 搜索树的根，且至少有两个互不相通的子分支
    if(in_edge == 0 && child_cnt >= 2)
    {
        is_cut_vertex[u] = true;
    }
}

void solve()
{
    /* 实战调用示例
    int n, m; cin >> n >> m;
    
    // 初始化链式前向星
    tot = 1;
    for(int i = 1; i <= n; i++) head[i] = 0;
    
    for(int i = 1; i <= m; i++)
    {
        int u, v; cin >> u >> v;
        if(u == v) continue; // 屏蔽自环
        add(u, v);
        add(v, u);
    }
    
    // 图可能不连通，遍历所有未访问的点作为 DFS 根节点
    for(int i = 1; i <= n; i++)
    {
        if(!dfn[i]) tarjan(i, 0); // 根节点没有 in_edge，传 0
    }
    
    // 输出割点
    int cut_v_cnt = 0;
    for(int i = 1; i <= n; i++) if(is_cut_vertex[i]) cut_v_cnt++;
    cout << "割点数量: " << cut_v_cnt << endl;
    
    // 输出割边 (注意一对双向边在数组中占两个位置，输出时要避免重复)
    // 可以通过遍历所有正向边 (i = 2, 4, 6...) 来输出
    for(int i = 2; i <= tot; i += 2)
    {
        if(is_bridge[i])
        {
            // to[i^1] 存的是边的起点，to[i] 存的是边的终点
            cout << "割边: " << to[i ^ 1] << " - " << to[i] << endl;
        }
    }
    */
}