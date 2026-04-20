#include <bits/stdc++.h>
using namespace std;
#define int long long
using PII = pair<int, int>;

const int MAXN = 2e5 + 5;
vector<int> adj[MAXN];
int cur[MAXN]; 
int in_deg[MAXN], out_deg[MAXN];
vector<int> path;

// Hierholzer 核心 DFS
void dfs(int u)
{
    while(cur[u] < adj[u].size())
    {
        int v = adj[u][cur[u]++]; // 边走边删，指针后移
        dfs(v);
    }
    path.push_back(u); // 陷入死胡同，压入答案栈
}

void solve()
{
    int n, m; 
    cin >> n >> m;

    // 1. 多组数据初始化清空
    for(int i = 1; i <= n; i++)
    {
        adj[i].clear();
        cur[i] = in_deg[i] = out_deg[i] = 0;
    }
    path.clear();

    // 2. 读入有向边建图
    for(int i = 1; i <= m; i++)
    {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        out_deg[u]++;
        in_deg[v]++;
    }

    // 3. 字典序最小：对邻接表进行升序排序
    for(int i = 1; i <= n; i++)
    {
        sort(adj[i].begin(), adj[i].end());
    }

    // 4. 判定奇偶点，寻找起点
    int start_node = 1, odd_cnt = 0;
    bool valid = true;

    for(int i = 1; i <= n; i++)
    {
        if(in_deg[i] != out_deg[i])
        {
            odd_cnt++;
            if(out_deg[i] - in_deg[i] == 1) start_node = i; // 路径的唯一合法起点
            else if(in_deg[i] - out_deg[i] != 1) valid = false; // 度数差大于1，直接无解
        }
    }

    // 异常点只能是 0 个(回路) 或 2 个(路径)
    if(!valid || (odd_cnt != 0 && odd_cnt != 2))
    {
        cout << "No\n"; 
        return;
    }

    // 如果是欧拉回路(异常点为0)，没有绝对起点，随便找一个有出度的点作为起点
    if(odd_cnt == 0)
    {
        for(int i = 1; i <= n; i++)
        {
            if(out_deg[i] > 0)
            {
                start_node = i;
                break;
            }
        }
    }

    // 5. 跑核心算法
    dfs(start_node);

    // 6. 连通性判定：如果跑出来的点数不等于边数+1，说明图不连通，有边没走到
    if(path.size() != m + 1)
    {
        cout << "No\n";
        return;
    }

    // 7. 逆序输出即为最终的一笔画路径
    reverse(path.begin(), path.end());
    for(int i = 0; i < path.size(); i++)
    {
        cout << path[i] << " \n"[i == path.size() - 1];
    }
}