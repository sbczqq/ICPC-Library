#include<bits/stdc++.h>
using namespace std;
#define int long long

void solve()
{
    int n,m;
    cin>>n>>m;
    vector<vector<int>>adj(n+1);
    vector<int>deg(n+1); // 出度减入度
    for(int i=1;i<=m;i++)
    {
        int u,v;
        cin>>u>>v;
        adj[u].push_back(v);
        deg[u]++;
        deg[v]--;
    }

    int s=0,t=0,first=0; // 路径起点、终点、第一个有出边的点
    for(int u=1;u<=n;u++)
    {
        if(!first&&!adj[u].empty())first=u;
        if(deg[u]==0)continue;
        if(deg[u]==1&&!s)s=u;
        else if(deg[u]==-1&&!t)t=u;
        else
        {
            cout<<"No\n"; // 度差不合法，或起点、终点不唯一
            return;
        }
    }
    if(!s)s=first?first:1; // 回路从最小的有出边点开始；没有边就输出1

    for(int u=1;u<=n;u++)sort(adj[u].rbegin(),adj[u].rend()); // 倒序排，最小邻点在末尾

    vector<int>stk,path;
    stk.reserve(m+1);
    path.reserve(m+1);
    stk.push_back(s);
    while(!stk.empty())
    {
        int u=stk.back();
        if(!adj[u].empty())
        {
            int v=adj[u].back();
            adj[u].pop_back(); // 每条有向边只取一次
            stk.push_back(v);
        }
        else
        {
            path.push_back(u); // 没有剩余出边，回退时记录
            stk.pop_back();
        }
    }

    if((int)path.size()!=m+1)
    {
        cout<<"No\n"; // 还有边没有走到
        return;
    }
    for(int i=m;i>=0;i--)cout<<path[i]<<" \n"[i==0]; // 逆序输出
}
