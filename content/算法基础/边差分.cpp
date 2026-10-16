#include<bits/stdc++.h>
using namespace std;
#define int long long
#define endl '\n'

const int N=2e5+10,L=20;
vector<int>e[N],ord; // 原树、父节点排在子节点之前的遍历顺序
int fa[N][L],dep[N],d[N]; // 倍增祖先、深度、差分标记，汇总后d保存最终增量

void init(int root=1) // 提前建好树，预处理祖先和遍历顺序
{
    ord.clear();
    ord.push_back(root);
    dep[root]=1;
    for(int j=0;j<L;j++)fa[root][j]=0; // 根的祖先为0号哨兵
    for(int i=0;i<(int)ord.size();i++) // 按遍历顺序展开，避免递归遍历长链
    {
        int u=ord[i];
        for(auto v:e[u])
        {
            if(v==fa[u][0])continue;
            fa[v][0]=u;
            dep[v]=dep[u]+1;
            for(int j=1;j<L;j++)fa[v][j]=fa[fa[v][j-1]][j-1];
            ord.push_back(v);
        }
    }
}

int lca(int u,int v) // 求u、v的最近公共祖先
{
    if(dep[u]<dep[v])swap(u,v);
    int dif=dep[u]-dep[v];
    for(int j=L-1;j>=0;j--)
        if(dif&(1LL<<j))u=fa[u][j]; // 先将两点提升到同一深度
    if(u==v)return u;
    for(int j=L-1;j>=0;j--)
    {
        if(fa[u][j]!=fa[v][j])
        {
            u=fa[u][j];
            v=fa[v][j];
        }
    }
    return fa[u][0];
}

void update(int u,int v,int w) // 将路径u到v上的每条边加w
{
    int p=lca(u,v);
    d[u]+=w;
    d[v]+=w; // 将边权记在较深的端点上，两端分别向祖先贡献w
    d[p]-=2*w; // LCA没有通向父亲的路径边，抵消在这里汇合的两份贡献
}

void collect() // 所有修改结束后调用一次，d[u]表示u到父节点的边权增量
{
    for(int i=(int)ord.size()-1;i>0;i--)
    {
        int u=ord[i];
        d[fa[u][0]]+=d[u]; // 从子节点向父节点累加，根不对应任何父边
    }
}

void solve()
{
    int n,m;
    cin>>n>>m; // 点数、路径修改数量，初始边权均为0
    vector<pair<int,int>>edges; // 保存道路输入顺序，最后按这个顺序输出
    for(int i=1;i<n;i++)
    {
        int u,v;
        cin>>u>>v;
        e[u].push_back(v);
        e[v].push_back(u);
        edges.push_back({u,v});
    }
    init();
    while(m--)
    {
        int u,v,w;
        cin>>u>>v>>w;
        update(u,v,w);
    }
    collect();
    for(auto [u,v]:edges)
    {
        int x=dep[u]>dep[v]?u:v; // 每条边的答案存在较深的端点上
        cout<<d[x]<<endl;
    }
}

signed main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    solve();
    return 0;
}
