#include<bits/stdc++.h>
using namespace std;

#define int long long
#define endl '\n'

const int MAXN=510; 

vector<int> e[MAXN]; // e[u]: 左侧节点 u 指向的右侧节点集合
int match[MAXN];     // match[v]=u: 右侧节点 v 当前匹配的左侧节点是 u
bool vis[MAXN];      // vis[v]: 右侧节点 v 在本轮寻找中是否被尝试过

// ================== 核心：寻找增广路 ==================
bool find(int u)
{
    for(int v:e[u])
    {
        if(!vis[v])
        {
            vis[v]=true; // 标记为已访问，防止死循环
            // 如果 v 还没有对象，或者 v 的现任能腾出位置去找别人
            if(!match[v]||find(match[v]))
            {
                match[v]=u; // 牵手成功
                return true;
            }
        }
    }
    return false;
}

void solve()
{
    /* 实战调用示例
    int n1,n2,m; 
    cin>>n1>>n2>>m;
    for(int i=1;i<=m;i++)
    {
        int u,v; cin>>u>>v;
        e[u].push_back(v); // 只需要建单向边：左 -> 右
    }
    
    int ans=0;
    for(int i=1;i<=n1;i++)
    {
        memset(vis,0,sizeof vis); // 【极其重要】：每次给新左侧节点找对象前，必须清空 vis！
        if(find(i))ans++;
    }
    cout<<ans<<endl; // 输出最大匹配数
    */
}

signed main()
{
    cin.tie(0);
    ios::sync_with_stdio(0);
    solve();
    return 0;
}