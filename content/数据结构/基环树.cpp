#include<bits/stdc++.h>
using namespace std;
#define int long long

const int N=2e5+10,L=20,INF=1LL<<60;
struct E
{
    int v,w,id; // 邻点、边权、无向边编号
};
vector<E>g[N]; // 无向邻接表，每条边存两个方向
vector<int>ord; // 被剥掉的点，子节点先于父节点
vector<vector<int>>ring; // ring[c]按顺序存第c个环上的点
int deg[N],fa[N][L]; // 剩余度数、树枝上的倍增祖先，fa[u][0]是父节点
int dep[N],root[N]; // 到环的步数、树枝接到的环点
int id[N],pos[N]; // 环点所属的环、从环起点走到这个点的距离
int sum[N],dis[N]; // 每个环的周长、每个点到环的带权距离
int dp[N][2]; // 不选、选当前点时，其树枝上的最多选点数

void add(int u,int v,int w,int i)
{
    g[u].push_back({v,w,i});
    g[v].push_back({u,w,i}); // 同一条边的两个方向共用编号
}

void build(int n) // 先剥树枝，再找环，最后把环的信息传回树枝
{
    ord.clear();
    ring.clear();
    ring.push_back({}); // 环编号从1开始
    for(int u=1;u<=n;u++)
    {
        deg[u]=g[u].size();
        fa[u][0]=dep[u]=root[u]=id[u]=dis[u]=0;
        if(deg[u]==1)ord.push_back(u); // 度数为1，是可以剥掉的叶子
    }
    for(int i=0;i<(int)ord.size();i++)
    {
        int u=ord[i];
        deg[u]=0; // 删掉叶子，剩下的唯一邻点就是它的父节点
        for(auto [v,w,j]:g[u])
        {
            if(!deg[v])continue; // 这个邻点之前已被剥掉
            fa[u][0]=v; // 没被剥掉的唯一邻点，方向朝向环
            dis[u]=w; // 先存父边长度，稍后累加到环的距离
            if(--deg[v]==1)ord.push_back(v); // 去掉一条边后，v也可能成为叶子
        }
    }

    for(int i=1;i<=n;i++)
    {
        if(!deg[i]||id[i])continue; // 只从还没编号的环点开始
        int c=ring.size(),u=i,last=0;
        ring.push_back({});
        sum[c]=0;
        do
        {
            ring[c].push_back(u); // 按实际走过的顺序存环点
            id[u]=c;
            root[u]=u; // 环点已经在环上，入口就是自己
            pos[u]=sum[c]; // 从环起点顺着走到u的距离
            for(auto [v,w,j]:g[u])
            {
                if(!deg[v]||j==last)continue; // 按边编号跳过来边，支持重边
                sum[c]+=w;
                u=v;
                last=j; // 记边编号，下一步不能沿同一条边折返
                break;
            }
        }while(u!=i);
    }

    for(int i=(int)ord.size()-1;i>=0;i--)
    {
        int u=ord[i],v=fa[u][0];
        root[u]=root[v]; // 逆序时，父节点已经知道接在哪个环点
        dep[u]=dep[v]+1; // 到环步数比父节点多1
        dis[u]+=dis[v]; // 父边长度加上父节点到环的距离
    }
}

int jumps(int u) // 第一次走到重复点的步数
{
    return dep[u]+(int)ring[id[root[u]]].size(); // 先到环，再绕一圈
}

void init_lca(int n) // 只在需要查询最短路时调用
{
    for(int j=1;j<L;j++)
        for(int u=1;u<=n;u++)
            fa[u][j]=fa[fa[u][j-1]][j-1]; // 两次跳2^(j-1)步
}

int lca(int u,int v) // 只用于接在同一个环点上的两个点
{
    if(dep[u]<dep[v])swap(u,v);
    int d=dep[u]-dep[v];
    for(int j=L-1;j>=0;j--)
        if(d>>j&1)u=fa[u][j]; // 先把更深的点提到相同深度
    if(u==v)return u;
    for(int j=L-1;j>=0;j--)
    {
        if(fa[u][j]==fa[v][j])continue; // 跳完会相遇，说明这步太大
        u=fa[u][j];
        v=fa[v][j];
    }
    return fa[u][0];
}

int dist(int u,int v)
{
    int a=root[u],b=root[v];
    if(id[a]!=id[b])return -1; // 不在同一个连通块
    if(a==b)return dis[u]+dis[v]-2*dis[lca(u,v)]; // 在同一棵树上，直接走树路
    int d=abs(pos[a]-pos[b]); // 环上两条路线的长度为d和周长减d
    return dis[u]+dis[v]+min(d,sum[id[a]]-d);
}

int calc(const vector<int>&a,int take) // take表示环上第一个点选不选
{
    int f=take?-INF:dp[a[0]][0],h=take?dp[a[0]][1]:-INF; // f不选、h选当前点，-INF表示不允许
    for(int i=1;i<(int)a.size();i++)
    {
        int u=a[i],nf=max(f,h)+dp[u][0],nh=f+dp[u][1]; // 当前不选时，前一个点可选可不选
        f=nf;
        h=nh; // 当前点选了，前一个点就只能不选
    }
    return take?f:max(f,h); // 第一个点选了，最后一个点必须不选
}

int maxset(int n) // 相邻点不能同时选，返回最多能选的点数
{
    for(int u=1;u<=n;u++)dp[u][0]=0,dp[u][1]=1; // 先只算自己：不选为0，选为1
    for(int u:ord) // 顺着剥叶顺序，子节点先把结果交给父节点
    {
        int v=fa[u][0];
        dp[v][0]+=max(dp[u][0],dp[u][1]); // 父节点不选，子节点可选可不选
        dp[v][1]+=dp[u][0]; // 父节点选了，子节点不能选
    }
    int ans=0;
    for(int c=1;c<(int)ring.size();c++)
        ans+=max(calc(ring[c],0),calc(ring[c],1)); // 枚举环上第一个点选或不选
    return ans;
}

void solve_jump(int n) // 题型1：每点有一个固定下一站，求从各起点出发第几步首次到达已见过的点
{
    for(int u=1;u<=n;u++) // 输入 n 个下一站，按点编号排列；出发时就算见过起点
    {
        int v;
        cin>>v; // 如 n=5，下一站是 2 3 2 3 4
        add(u,v,1,u); // 每次跳转单独存边，用于找环；1→2 和 2→1 是两条边
    }
    build(n); // 例：5→4→3→2→3，第4步首次重复，等于到环2步加环长2
    for(int u=1;u<=n;u++)cout<<jumps(u)<<" \n"[u==n]; // 按起点顺序输出步数，上例为 3 2 2 3 4
}

void solve_dist(int n) // 题型2：道路双向，给定两个点，求最短路线的总长度
{
    for(int i=1;i<=n;i++) // 输入 n 条边 u v w，边权非负，每个连通块恰有一个环
    {
        int u,v,w;
        cin>>u>>v>>w; // 例：环上 1—2、2—3 各长1，1—3 长10
        add(u,v,w,i); // 再接树枝 3—4、4—5，分别长2和3
    }
    build(n);
    init_lca(n); // 接在同一个环点时，直接用LCA算树路，不必先走到环

    int q;
    cin>>q; // 读询问次数
    while(q--)
    {
        int u,v;
        cin>>u>>v; // 每组是起点和终点；例：1→5 要比较环上长2和长10的两条路
        cout<<dist(u,v)<<'\n'; // 上例 1→5 为 2+2+3=7，4→5 为3；不连通输出-1
    }
}

void solve_set(int n) // 题型3：每条边的两个端点最多选一个，求最多能选几个点
{
    for(int i=1;i<=n;i++) // 输入 n 条无向边 u v，每个点价值为1
    {
        int u,v;
        cin>>u>>v; // 例：三角形 1—2—3—1，接一条树枝 3—4—5
        add(u,v,1,i); // 只关心两个点是否相邻，边权填1即可
    }
    build(n); // 树枝先做选或不选的DP，再处理环上的首尾相邻
    cout<<maxset(n)<<'\n'; // 上例三角形最多选1个，4和5最多选1个，选1和4可得到2
}

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);

    int type,n;
    cin>>type>>n; // 1：首次重复，2：最短路，3：最多选点
    if(type==1)solve_jump(n);
    else if(type==2)solve_dist(n);
    else if(type==3)solve_set(n);
    return 0;
}
