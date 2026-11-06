#include<bits/stdc++.h>
using namespace std;
#define int long long
#define endl '\n'

const int N=2e5+10;
vector<int>e[N],ne[N]; // 原有向图、缩点后的有向无环图
int dfn[N],low[N],tim; // 访问顺序、最早追溯顺序、时间戳
int stk[N],top,id[N],cnt; // 尚未归属分量的点栈、栈顶、所属分量、分量数量
bool st[N]; // 点是否仍在栈中

void tarjan(int u) // 求有向图的强连通分量
{
    dfn[u]=low[u]=++tim;
    stk[++top]=u;
    st[u]=1;
    for(auto v:e[u])
    {
        if(!dfn[v]) // 尚未访问，继续搜索
        {
            tarjan(v);
            low[u]=min(low[u],low[v]); // 用子节点的追溯值更新自己
        }
        else if(st[v])
            low[u]=min(low[u],dfn[v]); // 只考虑仍在栈中的点，使用它的访问顺序
    }
    if(dfn[u]==low[u]) // 找到一个强连通分量的DFS根
    {
        cnt++;
        int v;
        do
        {
            v=stk[top--];
            st[v]=0;
            id[v]=cnt; // 弹到u为止，这些点属于同一个分量
        }while(v!=u);
    }
}

void scc(int n) // 提前建好e[1..n]，求分量并建立缩点图
{
    tim=top=cnt=0;
    for(int i=1;i<=n;i++)
    {
        dfn[i]=low[i]=id[i]=0;
        st[i]=0;
        ne[i].clear(); // 清空计算状态，保留原图
    }
    for(int i=1;i<=n;i++)
        if(!dfn[i])tarjan(i); // 处理所有未访问的点
    for(int u=1;u<=n;u++)
    {
        for(auto v:e[u])
        {
            int a=id[u],b=id[v];
            if(a!=b)ne[a].push_back(b); // 跨分量的边加入新图，平行边不去重
        }
    }
}
