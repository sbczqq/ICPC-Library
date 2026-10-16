#include<bits/stdc++.h>
using namespace std;
#define int long long
#define endl '\n'

const int N=2e5+10,L=20;
vector<pair<int,int>>e[N]; // 原无向图，存{终点,边编号}
vector<int>ne[N*2]; // 圆方树，1..n为圆点，n+1..tot为方点
int dfn[N],low[N],tim,stk[N],top,tot,ec; // Tarjan访问顺序、追溯值、时间戳、点栈、栈顶、总点数、边数
int fa[N*2][L],dep[N*2],bel[N*2]; // 倍增祖先、深度、所在圆方树的根编号

void add(int u,int v) // 添加无向边，两个方向共用边编号
{
    ec++;
    e[u].push_back({v,ec});
    e[v].push_back({u,ec});
}

void tarjan(int u,int from) // 划分点双连通分量，同时建立圆方树
{
    dfn[u]=low[u]=++tim;
    stk[++top]=u;
    for(auto [v,id]:e[u])
    {
        if(id==from)continue; // 只跳过进入边，保留其他重边
        if(!dfn[v])
        {
            tarjan(v,id);
            low[u]=min(low[u],low[v]);
            if(low[v]>=dfn[u]) // 一个点双在u处划分完成
            {
                tot++; // 新建方点，表示这个点双
                int x;
                do
                {
                    x=stk[top--];
                    ne[tot].push_back(x);
                    ne[x].push_back(tot); // 点双内的圆点连接到对应方点
                }while(x!=v);
                ne[tot].push_back(u);
                ne[u].push_back(tot); // u也属于这个点双，但暂时留在栈中
            }
        }
        else low[u]=min(low[u],dfn[v]);
    }
}

void dfs(int u,int from,int root) // 预处理圆方树的深度、祖先和所属连通块
{
    bel[u]=root;
    dep[u]=dep[from]+1;
    fa[u][0]=from;
    for(int j=1;j<L;j++)fa[u][j]=fa[fa[u][j-1]][j-1]; // 计算向上跳2^j步到达的祖先
    for(auto v:ne[u])
        if(v!=from)dfs(v,u,root);
}

void build(int n)
{
    tot=n; // 保留原图点编号，方点从n+1开始编号
    for(int i=1;i<=n;i++)
    {
        if(!dfn[i])
        {
            tarjan(i,0);
            top--; // 当前连通块只剩DFS根节点留在栈中，将它移除
        }
    }
    for(int i=1;i<=n;i++)
        if(!bel[i])dfs(i,0,i); // 原图可能不连通，分别预处理每棵圆方树
}

int lca(int u,int v) // 查询同一棵树中两个点的最近公共祖先
{
    if(dep[u]<dep[v])swap(u,v);
    int d=dep[u]-dep[v];
    for(int j=L-1;j>=0;j--)
        if(d&(1LL<<j))u=fa[u][j]; // 先将较深的点提升到同一深度
    if(u==v)return u;
    for(int j=L-1;j>=0;j--)
    {
        if(fa[u][j]!=fa[v][j]) // 两点同时向上跳，保持在公共祖先的下方
        {
            u=fa[u][j];
            v=fa[v][j];
        }
    }
    return fa[u][0];
}

int dis(int u,int v) // 圆方树上两点之间的边数
{
    return dep[u]+dep[v]-2*dep[lca(u,v)];
}

bool check(int u,int v,int x) // 关闭原图路口x后，u和v是否仍能互相到达
{
    if(u==x||v==x)return 0; // 起点或终点已经关闭
    if(bel[u]!=bel[v])return 0; // 两个地点原本就不连通
    if(bel[x]!=bel[u])return 1; // 关闭其他连通块的路口，不影响这两个地点
    return dis(u,x)+dis(x,v)!=dis(u,v); // x在树上路径中才是必经点，关闭后才会断开
}

void solve()
{
    int n,m,q;
    cin>>n>>m>>q; // 路口数、双向道路数、询问数
    for(int i=1;i<=m;i++)
    {
        int u,v;
        cin>>u>>v;
        add(u,v);
    }
    build(n); // 一次建树，之后每个询问互相独立
    while(q--)
    {
        int u,v,x;
        cin>>u>>v>>x; // 判断关闭路口x后，地点u和v是否仍然连通
        cout<<(check(u,v,x)?"YES":"NO")<<endl;
    }
}

signed main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    solve();
    return 0;
}
