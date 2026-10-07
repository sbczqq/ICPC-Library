#include<bits/stdc++.h>
using namespace std;
#define int long long
#define endl '\n'
const int INF=1LL<<60;

struct Dinic
{
    struct E
    {
        int v,cap; // 终点、剩余容量
    };
    vector<E>e; // 正向边与反向边成对存储，编号互为 id^1
    vector<vector<int>>g; // g[u] 存储从 u 出发的边编号
    vector<int>dep,cur; // BFS 层数、DFS 当前搜索到的邻接表位置
    Dinic(int n)
    {
        g.resize(n+1); // 点编号为 1~n
        dep.resize(n+1);
        cur.resize(n+1);
    }
    void add(int u,int v,int ca) // 添加容量为 ca 的有向边 u->v
    {
        int id=e.size();
        e.push_back({v,ca}); // 正向边初始剩余容量为 ca
        e.push_back({u,0}); // 反向边用于退回已发送的流量
        g[u].push_back(id);
        g[v].push_back(id^1);
    }
    bool bfs(int s,int t) // 在残量网络上建立分层图
    {
        fill(dep.begin(),dep.end(),-1); // -1 表示尚未到达
        queue<int>Q;
        dep[s]=0;
        Q.push(s);
        while(Q.size())
        {
            int u=Q.front();
            Q.pop();
            for(auto id:g[u])
            {
                int v=e[id].v;
                if(e[id].cap==0||dep[v]!=-1)continue; // 只沿有剩余容量的边访问新点
                dep[v]=dep[u]+1;
                Q.push(v);
            }
        }
        return dep[t]!=-1; // 汇点可达时继续增广
    }
    int dfs(int u,int t,int f) // 从 u 向汇点最多发送 f 单位流量
    {
        if(u==t)return f;
        int us=0; // 本次实际发送的流量
        for(int &i=cur[u];i<(int)g[u].size();i++) // 当前弧优化，记录已搜索到的位置
        {
            int id=g[u][i];
            int v=e[id].v;
            if(e[id].cap==0||dep[u]+1!=dep[v])continue; // 只沿分层图的下一层前进
            int re=dfs(v,t,min(f-us,e[id].cap)); // 发送量受剩余需求和边容量限制
            if(re==0)continue;
            e[id].cap-=re; // 消耗正向边的剩余容量
            e[id^1].cap+=re; // 增加反向边的剩余容量
            us+=re; // 累计本次发送量
            if(us==f)break; // 已发送完 f，保留当前弧位置供下次使用
        }
        return us;
    }
    int maxflo(int s,int t) // 求源点 s 到汇点 t 的最大流
    {
        if(s==t)return 0;
        int an=0;
        while(bfs(s,t)) // 每轮重新建立分层图
        {
            fill(cur.begin(),cur.end(),0); // 新的分层图从头搜索邻接表
            while(1)
            {
                int re=dfs(s,t,INF);
                if(re==0)break; // 当前分层图的阻塞流已求完
                an+=re;
            }
        }
        return an;
    }
};
