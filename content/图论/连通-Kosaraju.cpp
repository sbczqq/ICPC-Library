#include<bits/stdc++.h>
using namespace std;
#define int long long
#define endl '\n'

const int N=2e5+10;
vector<int>e[N],re[N],ne[N]; // 原有向图、反图、缩点后的有向无环图
int stk[N],top,id[N],cnt; // 按DFS完成顺序存点、记录数量、所属分量编号、分量数量
bool st[N]; // 第一遍DFS中，点是否已经访问

void add(int u,int v) // 添加有向边u->v，同时建立反图
{
    e[u].push_back(v);
    re[v].push_back(u); // 反图将原图的边方向全部反转
}

void dfs1(int u) // 第一遍在原图上搜索，记录DFS完成顺序
{
    st[u]=1;
    for(auto v:e[u])
        if(!st[v])dfs1(v); // 先处理所有尚未访问的后继
    stk[++top]=u; // 所有出边处理完后记录u，得到后序顺序
}

void dfs2(int u) // 第二遍在反图上搜索，划分当前强连通分量
{
    id[u]=cnt; // 标记所属分量，同时表示第二遍已经访问
    for(auto v:re[u])
        if(!id[v])dfs2(v); // 沿反图搜索，未归属的可达点属于当前分量
}

void scc(int n) // 用add提前建图，其余状态初始为零
{
    for(int i=1;i<=n;i++)
        if(!st[i])dfs1(i); // 原图可能不连通，每个未访问的点都要搜索

    for(int i=top;i>=1;i--) // 按第一遍DFS完成顺序的逆序处理
    {
        int u=stk[i];
        if(!id[u]) // 尚未归属任何分量，从u开始划分新分量
        {
            cnt++;
            dfs2(u); // 本次在反图中访问到的点构成一个强连通分量
        }
    }

    for(int u=1;u<=n;u++) // 遍历原图的边，建立缩点后的新图
    {
        for(auto v:e[u])
        {
            int a=id[u],b=id[v]; // 将原点映射为所属分量
            if(a!=b)ne[a].push_back(b); // 只保留分量之间的边，平行边不去重
        }
    }
}
