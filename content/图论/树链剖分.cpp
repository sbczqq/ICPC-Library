#include<bits/stdc++.h>
using namespace std;

#define int long long
#define endl '\n'

const int MAXN=1e5+10;

vector<int> e[MAXN];
int w[MAXN];    // w[i]: 原图中节点 i 的初始权值
int wt[MAXN];   // wt[i]: 映射后，DFS序为 i 的位置对应的权值 (用于线段树建树)

// --- 树链剖分核心数组 ---
int fa[MAXN], dep[MAXN], sz[MAXN], son[MAXN];
int topf[MAXN]; // 节点所在重链的顶端节点 (用于路径跳跃)
int dfn[MAXN];  // 树转数组的时间戳下标
int rnk[MAXN];  // 数组转树的逆映射
int timestamp;

// ================== 1. 树剖初始化 ==================
void dfs1(int u, int f, int d)
{
    fa[u]=f; dep[u]=d; sz[u]=1;
    int max_sz=-1;
    for(int v:e[u])
    {
        if(v==f)continue;
        dfs1(v,u,d+1);
        sz[u]+=sz[v];
        if(sz[v]>max_sz)
        {
            max_sz=sz[v];
            son[u]=v;
        }
    }
}

void dfs2(int u, int t)
{
    dfn[u]=++timestamp;
    rnk[timestamp]=u;
    wt[timestamp]=w[u];
    topf[u]=t;
    
    if(!son[u])return;
    // 【核心】：必须优先走重儿子，保证重链 DFS 序连续
    dfs2(son[u], t); 
    
    // 轻儿子各自成为新重链的顶点
    for(int v:e[u])
    {
        if(v==fa[u]||v==son[u])continue;
        dfs2(v, v); 
    }
}

// ================== 2. 线段树维护 ==================
int sum[MAXN*4], add[MAXN*4];

void pushup(int u){ sum[u]=sum[u<<1]+sum[u<<1|1]; }

void pushdown(int u, int l, int r)
{
    if(add[u])
    {
        int mid=l+r>>1;
        sum[u<<1]+=add[u]*(mid-l+1);
        sum[u<<1|1]+=add[u]*(r-mid);
        add[u<<1]+=add[u];
        add[u<<1|1]+=add[u];
        add[u]=0;
    }
}

void build(int u, int l, int r)
{
    if(l==r){ sum[u]=wt[l]; return; }
    int mid=l+r>>1;
    build(u<<1, l, mid);
    build(u<<1|1, mid+1, r);
    pushup(u);
}

void update_seg(int u, int l, int r, int L, int R, int k)
{
    if(L<=l&&r<=R){ sum[u]+=k*(r-l+1); add[u]+=k; return; }
    pushdown(u, l, r);
    int mid=l+r>>1;
    if(L<=mid)update_seg(u<<1, l, mid, L, R, k);
    if(R>mid)update_seg(u<<1|1, mid+1, r, L, R, k);
    pushup(u);
}

int query_seg(int u, int l, int r, int L, int R)
{
    if(L<=l&&r<=R)return sum[u];
    pushdown(u, l, r);
    int mid=l+r>>1, res=0;
    if(L<=mid)res+=query_seg(u<<1, l, mid, L, R);
    if(R>mid)res+=query_seg(u<<1|1, mid+1, r, L, R);
    return res;
}

// ================== 3. 树上路径与子树操作 ==================
// 路径修改
void update_path(int u, int v, int k, int n)
{
    while(topf[u]!=topf[v])
    {
        if(dep[topf[u]]<dep[topf[v]]) swap(u,v); // 谁深谁往上跳
        update_seg(1, 1, n, dfn[topf[u]], dfn[u], k); // 处理这一截重链
        u=fa[topf[u]]; // 跨越轻边
    }
    if(dep[u]>dep[v]) swap(u,v);
    update_seg(1, 1, n, dfn[u], dfn[v], k); // 处理最后在同一条重链上的部分
}

// 路径查询
int query_path(int u, int v, int n)
{
    int res=0;
    while(topf[u]!=topf[v])
    {
        if(dep[topf[u]]<dep[topf[v]]) swap(u,v);
        res+=query_seg(1, 1, n, dfn[topf[u]], dfn[u]);
        u=fa[topf[u]];
    }
    if(dep[u]>dep[v]) swap(u,v);
    res+=query_seg(1, 1, n, dfn[u], dfn[v]);
    return res;
}

// 子树修改
void update_tree(int u, int k, int n) { update_seg(1, 1, n, dfn[u], dfn[u]+sz[u]-1, k); }

// 子树查询
int query_tree(int u, int n) { return query_seg(1, 1, n, dfn[u], dfn[u]+sz[u]-1); }

void solve()
{
    /* 实战调用示例
    int n,m,root; cin>>n>>m>>root;
    for(int i=1;i<=n;i++) cin>>w[i];
    // ... 建图 e[u].push_back(v); ...
    
    dfs1(root,0,1);
    dfs2(root,root);
    build(1,1,n);
    
    // update_path(u,v,k,n);
    // cout<<query_path(u,v,n)<<endl;
    */
}

signed main()
{
    cin.tie(0); ios::sync_with_stdio(0);
    solve();
    return 0;
}