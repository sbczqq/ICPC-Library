#include<bits/stdc++.h>
using namespace std;
#define int long long
using PII = pair<int,int>;

const int MAXN=2e5+5;
vector<int> adj[MAXN];
int dfn[MAXN],sz[MAXN];
int timer;

void dfs(int u,int p)
{
    dfn[u]=++timer;
    sz[u]=1;
    for(int v:adj[u])
    {
        if(v!=p)
        {
            dfs(v,u);
            sz[u]+=sz[v];
        }
    }
}

/* 核心调用：
 * timer=0; dfs(root,0);
 * * 1. 子树区间：
 * int L=dfn[u], R=dfn[u]+sz[u]-1;
 * * 2. 祖先判定 (u是否为v的祖先)：
 * bool ok=(dfn[u]<=dfn[v] && dfn[v]<=dfn[u]+sz[u]-1);
 */