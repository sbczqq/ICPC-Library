#include<bits/stdc++.h>
using namespace std;

#define int long long
#define endl '\n'

const int MAXN=10010; // 状态的最大规模 (例如单堆石子的最大数量)

int k;                // 允许的操作种类数
int f[110];           // f[i]: 每次允许取走的石子数量集合
int sg[MAXN];         // sg[x]: 状态 x 的 SG 值 (记忆化数组)

// --- 核心：记忆化搜索求 SG 值 ---
int get_sg(int x)
{
    // 如果该状态已经算过，直接查表返回 O(1)
    if(sg[x]!=-1) return sg[x];
    
    // S 集合用来记录当前状态 x 所有合法后继状态的 SG 值
    unordered_set<int> S; 
    
    // 遍历所有合法的下一步操作
    for(int i=1;i<=k;i++)
    {
        int next_state = x - f[i];
        // 如果这个操作合法 (剩下的石子数不能为负)
        if(next_state >= 0) 
        {
            S.insert(get_sg(next_state)); // 递归求子状态的 SG 值并扔进集合
        }
    }
    
    // --- 模拟 mex 运算 ---
    // 从 0 开始往上枚举，找到第一个不在集合 S 中的数
    for(int i=0;;i++)
    {
        if(!S.count(i))
        {
            return sg[x]=i; // 记忆化并返回
        }
    }
}

void solve()
{
    /* 实战调用示例
    // 题目背景：有 n 堆石子，每次从一堆中取石子，取的数量必须在集合 {f[1], f[2] ... f[k]} 中。
    // 问先手是否必胜？
    
    cin>>k;
    for(int i=1;i<=k;i++) cin>>f[i]; // 读入允许取走的石子数集合
    
    // 1. 初始化记忆化数组为 -1 (代表还没算过)
    memset(sg, -1, sizeof sg);
    
    int n;
    cin>>n;
    
    int res = 0; // 记录异或和 SG_total
    for(int i=1;i<=n;i++)
    {
        int x;
        cin>>x; // 读入每一堆初始的石子数
        
        // 2. 利用 SG 定理，把所有独立游戏的 SG 值异或起来
        res ^= get_sg(x);
    }
    
    // 3. 终极判决
    if(res > 0) cout<<"Yes"<<endl; // 先手必胜
    else cout<<"No"<<endl;         // 先手必败
    */
}

signed main()
{
    cin.tie(0);
    ios::sync_with_stdio(0);
    int T=1;
    // cin>>T;
    while(T--) solve();
    return 0;
}