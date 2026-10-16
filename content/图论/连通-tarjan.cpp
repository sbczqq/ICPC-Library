#include<bits/stdc++.h>
using namespace std;
#define int long long
#define endl '\n'

const int N=2e5+10;
vector<int>e[N],ne[N]; // 原有向图、缩点后的有向无环图
int dfn[N],low[N],tim; // DFS访问顺序、能够追溯到的最早访问顺序、时间戳
int stk[N],top,id[N],cnt; // 尚未归属分量的点栈、栈顶、所属分量编号、分量数量
bool st[N]; // st[u]表示u是否仍在栈中

void tarjan(int u) // 求有向图的强连通分量
{
    dfn[u]=low[u]=++tim; // 首次访问u，初始化访问顺序和追溯值
    stk[++top]=u; // 将u放入栈中，等待确定所属分量
    st[u]=1;
    for(auto v:e[u]) // 遍历u的所有出边
    {
        if(!dfn[v]) // v尚未访问，沿DFS树继续搜索
        {
            tarjan(v);
            low[u]=min(low[u],low[v]); // 用子节点能够追溯到的位置更新u
        }
        else if(st[v]) // v仍在栈中，可以参与当前尚未划分完的分量
            low[u]=min(low[u],dfn[v]); // 已访问的点用dfn更新，已出栈的点不再考虑
    }
    if(dfn[u]==low[u]) // u是一个强连通分量在DFS树中的根
    {
        cnt++;
        int v;
        do
        {
            v=stk[top--]; // 从栈顶弹出当前分量的点
            st[v]=0; // 已经确定所属分量，不再留在栈中
            id[v]=cnt; // 同一次弹出的点属于同一个强连通分量
        }while(v!=u); // 弹到u时，这个强连通分量恰好全部取出
    }
}

void scc(int n) // e[1..n]提前建好，其余状态初始为零
{
    for(int i=1;i<=n;i++)
        if(!dfn[i])tarjan(i); // 图可能不连通，每个未访问的点都要搜索
    for(int u=1;u<=n;u++)
    {
        for(auto v:e[u])
        {
            int a=id[u],b=id[v]; // 将原边的两个端点映射为分量编号
            if(a!=b)ne[a].push_back(b); // 只保留分量之间的边，平行边不去重
        }
    }
}
