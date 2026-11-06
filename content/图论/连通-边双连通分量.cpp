#include<bits/stdc++.h>
using namespace std;
#define int long long
#define endl '\n'
using ar2=array<int,2>;

const int N=2e5+10,M=2e5+10;
vector<ar2>g[N]; // 无向图，存{邻点,无向边编号}
vector<int>bcc[N],ne[N]; // 每个边双的点集、缩点后由桥连接的森林
int dfn[N],low[N],tim,stk[N],top,id[N],cnt; // 访问顺序、追溯值、时间戳、点栈、栈顶、所属分量、分量数量
bool br[M]; // 第i条无向边是否为桥

void add(int u,int v,int id) // 第id条无向边，编号从1开始，重边使用不同编号
{
    g[u].push_back({v,id});
    g[v].push_back({u,id});
}

void tarjan(int u,int f) // f为进入u的边编号，DFS根传0
{
    dfn[u]=low[u]=++tim;
    stk[++top]=u;
    for(auto [v,i]:g[u])
    {
        if(i==f)continue; // 只跳过进入边，重边仍能更新追溯值
        if(!dfn[v])
        {
            tarjan(v,i);
            low[u]=min(low[u],low[v]);
            if(low[v]>dfn[u])br[i]=1; // 子树无法绕回u，这条树边是桥
        }
        else
            low[u]=min(low[u],dfn[v]);
    }
    if(dfn[u]==low[u]) // 找到一个边双的根，将这一组点全部弹出
    {
        cnt++;
        int v;
        do
        {
            v=stk[top--];
            id[v]=cnt;
            bcc[cnt].push_back(v); // 每个点只属于一个边双
        }while(v!=u);
    }
}

void ebcc(int n,int m) // 提前建图，边编号为1..m
{
    for(int i=1;i<=cnt;i++)
    {
        bcc[i].clear();
        ne[i].clear();
    }
    tim=top=cnt=0;
    for(int i=1;i<=n;i++)dfn[i]=low[i]=id[i]=0;
    for(int i=1;i<=m;i++)br[i]=0;
    for(int i=1;i<=n;i++)
        if(!dfn[i])tarjan(i,0); // 处理所有连通块，孤立点也会成为一个边双
    for(int u=1;u<=n;u++)
    {
        for(auto [v,i]:g[u])
        {
            int a=id[u],b=id[v];
            if(a!=b)ne[a].push_back(b); // 跨分量的原边一定是桥，两方向各建一次
        }
    }
}
