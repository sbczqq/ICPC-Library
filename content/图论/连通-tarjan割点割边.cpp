#include<bits/stdc++.h>
using namespace std;
#define int long long
#define endl '\n'
using ar2=array<int,2>;

const int N=2e5+10,M=2e5+10;
vector<ar2>g[N]; // 无向图，存{邻点,无向边编号}
int dfn[N],low[N],tim; // 访问顺序、最早追溯顺序、时间戳
bool cut[N],br[M]; // 点是否为割点、边是否为割边

void add(int u,int v,int id) // 加入第id条无向边，编号从1开始
{
    g[u].push_back({v,id});
    g[v].push_back({u,id}); // 两个方向共用编号，重边使用不同编号
}

void tarjan(int u,int f) // f为进入u的边编号，DFS根传0
{
    dfn[u]=low[u]=++tim;
    int son=0; // DFS树中的子节点数量，用于判断根是否为割点
    for(auto [v,i]:g[u])
    {
        if(i==f)continue; // 只跳过进入边，其他通向父节点的重边仍要处理
        if(!dfn[v])
        {
            son++;
            tarjan(v,i);
            low[u]=min(low[u],low[v]);
            if(low[v]>dfn[u])br[i]=1; // 子树无法绕回u，这条树边是桥
            if(f&&low[v]>=dfn[u])cut[u]=1; // 非根：子树无法绕过u到达u的祖先
        }
        else
            low[u]=min(low[u],dfn[v]); // 非树边用邻点的访问顺序更新
    }
    if(!f&&son>=2)cut[u]=1; // 根至少有两棵DFS子树时才是割点
}

void cutbridge(int n,int m) // 提前建图，边编号为1..m
{
    tim=0;
    for(int i=1;i<=n;i++)
    {
        dfn[i]=low[i]=0;
        cut[i]=0;
    }
    for(int i=1;i<=m;i++)br[i]=0;
    for(int i=1;i<=n;i++)
        if(!dfn[i])tarjan(i,0); // 每个连通块分别寻找DFS根
}
