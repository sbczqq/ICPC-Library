#include<bits/stdc++.h>
using namespace std;
#define int long long
#define endl '\n'

const int N=2e5+10,INF=1LL<<60;

struct E
{
    int v,cap,cost; // 终点、剩余容量、单位流量费用
};

vector<E>e; // 正反向边成对存储，反向边编号为id^1
vector<int>g[N]; // g[u]存从u出发的边编号
int dis[N],pre[N]; // 源点到各点的最小费用、最短路上进入各点的边编号
bool st[N]; // 点是否在SPFA队列中

void add(int u,int v,int cap,int cost) // 添加容量为cap、单位费用为cost的有向边
{
    int id=e.size();
    e.push_back({v,cap,cost}); // 正向边用于发送流量
    e.push_back({u,0,-cost}); // 反向边用于退回流量，同时退回对应费用
    g[u].push_back(id);
    g[v].push_back(id^1);
}

bool spfa(int n,int s,int t) // 在残量网络中寻找单位费用最小的增广路
{
    fill(dis+1,dis+n+1,INF);
    fill(st+1,st+n+1,0);
    queue<int>q;
    dis[s]=0;
    q.push(s);
    st[s]=1;

    while(!q.empty())
    {
        int u=q.front();
        q.pop();
        st[u]=0; // 出队后，距离再次变小时可以重新入队

        for(auto id:g[u])
        {
            int v=e[id].v;
            if(e[id].cap==0)continue; // 没有剩余容量，不能继续发送流量
            if(dis[v]>dis[u]+e[id].cost)
            {
                dis[v]=dis[u]+e[id].cost; // 找到到达v的更小费用
                pre[v]=id; // 记录进入v的边，之后沿路径回溯
                if(!st[v])
                {
                    q.push(v);
                    st[v]=1; // 已在队列中的点不重复加入
                }
            }
        }
    }
    return dis[t]!=INF; // 汇点可达，说明还能继续增广
}

pair<int,int>mcmf(int n,int s,int t) // 点编号1..n，源汇不同，初始残量网络无负费用环
{
    int flo=0,cost=0; // 累计流量、累计费用
    while(spfa(n,s,t))
    {
        int f=INF; // 本次增广量为路径上的最小剩余容量
        for(int u=t;u!=s;u=e[pre[u]^1].v) // 反向边的终点就是路径上的前一个点
            f=min(f,e[pre[u]].cap);

        for(int u=t;u!=s;u=e[pre[u]^1].v)
        {
            int id=pre[u];
            e[id].cap-=f; // 正向边消耗容量
            e[id^1].cap+=f; // 反向边增加容量，允许后续调整已有流量
        }

        flo+=f;
        cost+=f*dis[t]; // 本次费用=增广量×路径单位费用
    }
    return {flo,cost}; // 无法继续增广时，得到最大流及其最小费用
}
