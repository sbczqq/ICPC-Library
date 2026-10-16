#include<bits/stdc++.h>
using namespace std;
#define int long long
#define endl '\n'

const int N=2e5+10,M=2e5+10;
vector<pair<int,int>>e[N]; // 每条邻接记录存{终点,无向边编号}
int dfn[N],low[N],tim,tot; // DFS访问顺序、最早追溯顺序、时间戳、无向边数量
bool cut[N],bridge[M]; // 点是否为割点、无向边是否为桥

void add(int u,int v) // 添加一条无向边，两个方向使用同一个边编号
{
    tot++;
    e[u].push_back({v,tot});
    e[v].push_back({u,tot}); // bridge[tot]对应本次加入的无向边
}

void tarjan(int u,int from) // from为进入u的无向边编号，DFS根节点传0
{
    dfn[u]=low[u]=++tim; // 首次访问u，初始化访问顺序和追溯值
    int cnt=0; // u在DFS树中的子节点数量，用于判断根节点是否为割点
    for(auto [v,id]:e[u])
    {
        if(id==from)continue; // 只跳过进入边，其他通向父节点的重边仍要处理
        if(!dfn[v]) // v尚未访问，这条边成为DFS树边
        {
            cnt++;
            tarjan(v,id);
            low[u]=min(low[u],low[v]); // 将v子树能够追溯到的位置合并到u
            if(low[v]>dfn[u])bridge[id]=1; // v子树无法绕过这条边回到u或u的祖先，说明它是桥
            if(from&&low[v]>=dfn[u])cut[u]=1; // 非根节点：v子树无法绕过u到达u的祖先
        }
        else
            low[u]=min(low[u],dfn[v]); // 非树边用邻点的dfn更新，重边和自环均可处理
    }
    if(!from&&cnt>=2)cut[u]=1; // 根节点至少有两个DFS子树时才是割点
}

void cut_bridge(int n) // 用add提前建图，其余状态初始为零
{
    for(int i=1;i<=n;i++)
        if(!dfn[i])tarjan(i,0); // 分别处理每个连通块，根节点没有进入边
}
