#include<bits/stdc++.h>
using namespace std;

#define int long long
#define endl '\n'

const int MAXN=1e5+10;

vector<int> e[MAXN]; // 存树的边
int col[MAXN];       // col[i]: 节点 i 的颜色
int sz[MAXN];        // sz[i]: 以 i 为根的子树大小
int son[MAXN];       // son[i]: 节点 i 的重儿子编号
int ans[MAXN];       // ans[i]: 存储节点 i 的最终查询答案

// --- 全局统计变量 ---
int cnt[MAXN]; // cnt[c]: 颜色 c 出现的次数桶
int maxc;      // maxc: 当前子树中出现次数最多的颜色的【频次】
int sum;       // sum: 当前子树中所有达到最大频次的【颜色编号之和】
int Son;       // Son: 标记当前的重儿子，在暴力统计时遇到它直接跳过，避免重复计算

// --- 1. 第一遍 DFS：寻找重儿子 ---
void dfs1(int u,int fa)
{
    sz[u]=1;
    for(int v:e[u])
    {
        if(v==fa)continue;
        dfs1(v,u);
        sz[u]+=sz[v];
        // 如果子节点的 sz 大于当前记录的重儿子的 sz，则更新重儿子
        if(sz[v]>sz[son[u]]) son[u]=v;
    }
}

// --- 2. 暴力统计/清除函数 ---
// val = 1 表示增加贡献，val = -1 表示清除贡献
void add(int u,int fa,int val)
{
    cnt[col[u]]+=val; // 更新颜色桶
    
    // 如果是增加贡献，顺便维护我们要求的答案 maxc 和 sum
    if(val>0)
    {
        if(cnt[col[u]]>maxc) 
        {
            maxc=cnt[col[u]]; // 频次打破了记录，更新最高频次
            sum=col[u];       // 最高频次的颜色变了，sum 直接被新颜色覆盖
        }
        else if(cnt[col[u]]==maxc) 
        {
            sum+=col[u];      // 频次并列第一，累加颜色编号
        }
    }
    
    // 递归处理子树
    for(int v:e[u])
    {
        // 【核心剪枝】：如果 v 是重儿子，说明它已经在桶里了，绝对不能再暴力算一遍！
        if(v==fa||v==Son)continue; 
        add(v,u,val);
    }
}

// --- 3. 第二遍 DFS：树上启发式合并 ---
// keep: 标记当前节点 u 是否是它父亲的重儿子 (1:保留贡献, 0:清空贡献)
void dfs2(int u,int fa,bool keep)
{
    // 第一步：先处理所有的【轻儿子】 (查完后清除它们的贡献)
    for(int v:e[u])
    {
        if(v==fa||v==son[u])continue;
        dfs2(v,u,0); // keep = 0，查完即毁
    }
    
    // 第二步：处理唯一的【重儿子】 (查完后保留它的贡献)
    if(son[u])
    {
        dfs2(son[u],u,1); // keep = 1，查完保留
        Son=son[u];       // 标记当前重儿子，防止下一步 add 时把它重复算进去
    }
    
    // 第三步：此时桶里只有重儿子的信息。
    // 我们暴力把当前节点 u 和所有轻儿子的信息加进桶里，凑齐整棵子树的信息
    add(u,fa,1); 
    
    Son=0; // 【必须重置】用完就扔，防止影响退栈后它父亲的计算
    
    // 第四步：记录当前子树的答案
    ans[u]=sum; 
    
    // 第五步：如果当前节点 u 是它父亲的【轻儿子】，那它查完后也必须挥刀自宫，清空全家老小
    if(!keep)
    {
        add(u,fa,-1); // val = -1 撤销计数
        maxc=sum=0;   // 【必须重置】全局极值变量
    }
}

void solve()
{
    /* 实战调用示例
    int n; cin>>n;
    for(int i=1;i<=n;i++)cin>>col[i];
    for(int i=1;i<n;i++)
    {
        int u,v; cin>>u>>v;
        e[u].push_back(v);
        e[v].push_back(u);
    }
    
    dfs1(1,0);   // 第一遍找出重儿子
    dfs2(1,0,1); // 第二遍启发式合并得出答案
    
    for(int i=1;i<=n;i++) cout<<ans[i]<<" ";
    cout<<endl;
    */
}