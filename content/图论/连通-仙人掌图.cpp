#include<bits/stdc++.h>
using namespace std;
#define int long long

const int N=2e5+10,L=20;
struct E
{
    int v,w,id; // 邻点、边权、无向边编号
};
vector<E>g[N];
vector<pair<int,int>>ne[N*2]; // 圆方树，只存父节点指向子节点的边

int dfn[N],cur[N],pre[N],from[N],tim; // 访问顺序、当前边、DFS父节点、来边编号
int d[N],pos[N]; // DFS树上到根的距离、在所属父环上的位置
bool on[N]; // on[u]=1表示u到DFS父节点的边在某个环里
int len[N*2],tot; // 方点对应的环长、圆方树总点数
int fa[N*2][L],dep[N*2],bel[N*2],dis[N*2]; // 倍增祖先、深度、所属根、到根最短距离

void make_ring(int a,int b,int w) // 从b回到祖先a的边，与DFS树路组成一个环
{
    int c=++tot;
    len[c]=d[b]-d[a]+w; // 树路a→b的长度，再加上回边的长度
    ne[a].push_back({c,0}); // a是进入这个环的入口，方点挂在a下面

    for(int u=b;u!=a;u=pre[u])
    {
        on[u]=1; // 标记环中的树边，之后不再把它当桥加入
        pos[u]=d[u]-d[a]; // 从a沿DFS树路走到u的距离
        ne[c].push_back({u,min(pos[u],len[c]-pos[u])}); // 环上两边取短的一边
    }
}

void dfs(int s) // 用显式栈找回边，避免长链上递归过深
{
    vector<int>stk={s};
    dfn[s]=++tim;
    while(!stk.empty())
    {
        int u=stk.back();
        if(cur[u]==(int)g[u].size())
        {
            stk.pop_back();
            continue;
        }

        auto [v,w,i]=g[u][cur[u]++];
        if(i==from[u])continue; // 跳过来时的同一条边，不能直接按父节点跳
        if(!dfn[v])
        {
            dfn[v]=++tim;
            pre[v]=u;
            from[v]=i;
            d[v]=d[u]+w;
            stk.push_back(v);
        }
        else if(dfn[v]<dfn[u])
            make_ring(v,u,w); // 只在后代看到祖先时建环，每个环只建一次
    }
}

void build(int n)
{
    tot=n; // 原图点编号1..n，新建方点从n+1开始
    for(int u=1;u<=n;u++)
        if(!dfn[u])dfs(u);

    for(int u=1;u<=n;u++)
        if(pre[u]&&!on[u])
            ne[pre[u]].push_back({u,d[u]-d[pre[u]]}); // 没有入环的树边就是桥

    vector<int>ord;
    for(int u=1;u<=n;u++)
    {
        if(pre[u])continue;
        bel[u]=u;
        ord.push_back(u); // 每个连通块的DFS根都是圆方树的根
    }

    for(int i=0;i<(int)ord.size();i++)
    {
        int u=ord[i];
        for(auto [v,w]:ne[u])
        {
            fa[v][0]=u;
            dep[v]=dep[u]+1;
            bel[v]=bel[u];
            dis[v]=dis[u]+w;
            ord.push_back(v);
        }
    }

    for(int j=1;j<L;j++)
        for(int u=1;u<=tot;u++)
            fa[u][j]=fa[fa[u][j-1]][j-1];
}

int up(int u,int k) // 从u向上走k条圆方树边
{
    for(int j=0;j<L;j++)
        if(k>>j&1)u=fa[u][j];
    return u;
}

int lca(int u,int v)
{
    if(dep[u]<dep[v])swap(u,v);
    u=up(u,dep[u]-dep[v]);
    if(u==v)return u;
    for(int j=L-1;j>=0;j--)
    {
        if(fa[u][j]==fa[v][j])continue;
        u=fa[u][j];
        v=fa[v][j];
    }
    return fa[u][0];
}

int query(int u,int v,int n) // 查询两个原图点之间的最短距离
{
    if(bel[u]!=bel[v])return -1;
    int p=lca(u,v);
    if(p<=n)return dis[u]+dis[v]-2*dis[p]; // LCA是原图点，两边直接在这里汇合

    int a=up(u,dep[u]-dep[p]-1); // u这一侧进入环的点
    int b=up(v,dep[v]-dep[p]-1); // v这一侧进入环的点
    int w=abs(pos[a]-pos[b]); // 环上一边长w，另一边长len[p]-w
    return dis[u]-dis[a]+dis[v]-dis[b]+min(w,len[p]-w);
}

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n,m,q;
    cin>>n>>m>>q; // 点数、边数、询问数
    for(int i=1;i<=m;i++)
    {
        int u,v,w;
        cin>>u>>v>>w; // 无向边，边权非负
        g[u].push_back({v,w,i});
        g[v].push_back({u,w,i});
    }

    build(n);
    while(q--)
    {
        int u,v;
        cin>>u>>v;
        cout<<query(u,v,n)<<'\n';
    }
    return 0;
}
