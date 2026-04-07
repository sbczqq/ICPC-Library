#include<bits/stdc++.h>
using namespace std;
#define int long long
#define endl '\n'
// 将边权 w 放在第一个位置，利用 tuple 默认字典序实现自带按边权排序
using Edge = tuple<int, int, int>;
void solve()
{
    int n, m;
    cin >> n >> m;
    vector<Edge> edges(m);
    vector<int> p(n + 1); // 并查集父节点数组 (原 fe)
    // lambda 并查集寻根函数（路径压缩）
    function<int(int)> find = [&](int x)->int
        {
            if(p[x] != x)p[x] = find(p[x]);
            return p[x];
        };

    // 1. 初始化并查集
    for(int i = 1;i <= n;i++)p[i] = i;
    // 2. 读入边，存入顺序为 {边权, 端点u, 端点v}
    for(int i = 0;i < m;i++)
    {
        int u, v, w;
        cin >> u >> v >> w;
        edges[i] = { w,u,v };
    }

    // 3. 按边权从小到大排序 (tuple 默认比对第一个元素)
    sort(edges.begin(), edges.end());

    int res = 0; // 记录最小生成树总边权 (原 ans)
    int cnt = 0; // 记录成功加入生成树的边数

    // 4. Kruskal 贪心加边
    for(int i = 0;i < m;i++)
    {
        auto [w, u, v] = edges[i]; // C++17 结构化绑定拆包
        u = find(u), v = find(v);
        if(u != v)
        {
            p[u] = v;
            res += w;
            cnt++; // 成功连边，计数器加 1
        }
    }

    // 5. 判断连通性
    // 原代码通过再跑一遍 O(N) 循环判所有点祖先是否相同。
    // 更优解法：一棵 n 个点的树必定刚好有 n-1 条边，直接判断 cnt 即可。
    if(cnt < n - 1)
    {
        cout << "orz" << endl; // 无法连通的情况
    }
    else
    {
        cout << res << endl;
    }
}