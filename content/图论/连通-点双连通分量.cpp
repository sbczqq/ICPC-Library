#include<bits/stdc++.h>
using namespace std;
#define int long long
#define endl '\n'
using ar2=array<int,2>;

const int N=2e5+10;
vector<ar2>g[N]; // 无向图，存{邻点,无向边编号}
vector<int>bcc[N]; // bcc[k]存第k个点双中的所有点，割点可以出现在多组中
int dfn[N],low[N],tim,stk[N],top,cnt; // 访问顺序、追溯值、时间戳、点栈、栈顶、分量数量
bool cut[N]; // 点是否为割点

void add(int u,int v,int id) // 第id条无向边，编号从1开始，重边使用不同编号
{
    g[u].push_back({v,id});
    g[v].push_back({u,id});
}

void tarjan(int u,int f) // f为进入u的边编号，DFS根传0
{
    dfn[u]=low[u]=++tim;
    stk[++top]=u; // 尚未完成划分的点暂存在栈中
    int son=0;
    for(auto [v,i]:g[u])
    {
        if(i==f)continue; // 只跳过进入边，保留其他通向父节点的重边
        if(!dfn[v])
        {
            son++;
            tarjan(v,i);
            low[u]=min(low[u],low[v]);
            if(low[v]>=dfn[u]) // v子树无法绕过u，在这里划分一个点双
            {
                if(f||son>=2)cut[u]=1; // 非根满足条件即为割点，根至少需要两棵子树
                cnt++;
                int x;
                do
                {
                    x=stk[top--];
                    bcc[cnt].push_back(x);
                }while(x!=v); // 弹到子节点v，取完当前点双的子树部分
                bcc[cnt].push_back(u); // u也属于这一组，但留在栈中参与其他分量
            }
        }
        else
            low[u]=min(low[u],dfn[v]);
    }
    if(!f&&!son)bcc[++cnt].push_back(u); // 孤立点或只有自环的点，单独作为一组
}

void vbcc(int n) // 提前建好g[1..n]，求点双和割点
{
    for(int i=1;i<=cnt;i++)bcc[i].clear();
    tim=top=cnt=0;
    for(int i=1;i<=n;i++)
    {
        dfn[i]=low[i]=0;
        cut[i]=0;
    }
    for(int i=1;i<=n;i++)
    {
        if(!dfn[i])
        {
            tarjan(i,0);
            top--; // 当前连通块只剩根节点留在栈中，将它移除
        }
    }
}
