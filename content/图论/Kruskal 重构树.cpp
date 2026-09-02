#include<bits/stdc++.h>
using namespace std;

#define int long long
#define endl '\n'
#define iint  __int128_t
using PII = pair<int,int>;
using PSI= pair<string,int>;
using tup= tuple<int,int,int>;

// 注意：空间必须开 2*N
const int N = 2e5+10; 
const int INF = 1e9;
int fe[N], va[N], de[N], f[N][20];
vector<int> g[N];
int find(int x) { 
    return fe[x] == x ? x : fe[x] = find(fe[x]); 
}

void dfs(int u, int fa) {
    de[u] = de[fa] + 1;
    f[u][0] = fa;
    for(int i = 1; i < 20; i++) f[u][i] = f[f[u][i-1]][i-1];
    for(int v : g[u]) {
        if(v != fa) dfs(v, u);
    }
}

int lca(int a, int b) {
    if(de[a] < de[b]) swap(a, b);
    for(int i = 19; i >= 0; i--)
        if(de[f[a][i]] >= de[b]) a = f[a][i];
    if(a == b) return a;
    for(int i = 19; i >= 0; i--)
        if(f[a][i] != f[b][i]) a = f[a][i], b = f[b][i];
    return f[a][0];
}

void solve() {
    int n, m;
    if(!(cin >> n >> m)) return;
    
    // 1. 初始化
    for(int i = 0; i <= 2 * n; i++) {
        g[i].clear();
        fe[i] = i; 
        va[i] = de[i] = 0;
        for(int j = 0; j < 20; j++) f[i][j] = 0;
    }
    
    // 2. 读边
    vector<tup> ar(m);
    for(int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        ar[i] = {w, u, v};
    }
    
    // 3. 排序 (求最小边权最大用 rbegin 降序，最大边权最小用 begin 升序)
    sort(ar.rbegin(), ar.rend());
    
    // 4. Kruskal 建树
    int id = n;
    for(auto [w, u, v] : ar) {
        int fu = find(u), fv = find(v);
        if(fu != fv) {
            va[++id] = w;
            fe[fu] = fe[fv] = id; // 旧根连向新根
            g[id].push_back(fu);
            g[id].push_back(fv);
        }
    }
    
    // 5. 森林 DFS 预处理
    for(int i = 1; i <= id; i++) {
        if(find(i) == i) dfs(i, 0);
    }
    
    // 6. 处理询问
    int q;
    cin >> q;
    while(q--) {
        int x, y;
        cin >> x >> y;
        if(find(x) != find(y)) cout << -1 << endl;
        else cout << va[lca(x, y)] << endl;
    }
}