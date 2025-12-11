#include<bits/stdc++.h>
using namespace std;

const int N = 500010;

vector<int> g[N];

int f[N][20];  // f[u][i] 表示 u 的第 2^i 个祖先
int de[N]; //深度数组，0号点深度为0作为哨兵
/**
 * DFS 预处理倍增数组和深度
 * @param u 当前节点
 * @param fa 父节点 (根节点调用时 fa 传 0)
 */
void dfs(int u, int fa) {
    de[u] = de[fa] + 1;
    f[u][0] = fa;

    // 倍增核心逻辑
    for(int i = 1; i < 20; i++) {
        f[u][i] = f[f[u][i - 1]][i - 1];
    }
    for(int v : g[u])
    {
        if(v == fa) continue;
        dfs(v, u);
    }
}

/**
 * 查询 LCA
 */
int lca(int a, int b) {
    if(de[a] < de[b]) swap(a, b);
    // 1. 将 a 跳到和 b 同一深度
    for(int i = 19; i >= 0; i--) {
        if(de[f[a][i]] >= de[b]) a = f[a][i];
    }
    if(a == b) return a;
    // 2. a 和 b 一起向上跳，直到父亲相同
    for(int i = 19; i >= 0; i--) {
        if(f[a][i] != f[b][i]) {
            a = f[a][i];
            b = f[b][i];
        }
    }
    return f[a][0]; // 返回父节点
}