
#include<bits/stdc++.h>
using namespace std;

#define int long long
#define endl '\n'

const int MOD=1e9+7;
const int MAXN=2e6+10; // 空间注意开到 n+m 的上限

int fact[MAXN];    // fact[i]: i! % MOD
int infact[MAXN];  // infact[i]: (i!)^(-1) % MOD

// --- 快速幂求逆元 ---
int qmi(int a,int k,int p)
{
    int res=1;
    a%=p;
    while(k)
    {
        if(k&1)res=res*a%p;
        a=a*a%p;
        k>>=1;
    }
    return res;
}

// --- 预处理阶乘与阶乘逆元 O(N) ---
void init_comb(int n)
{
    fact[0]=infact[0]=1;
    for(int i=1;i<=n;i++)
    {
        fact[i]=fact[i-1]*i%MOD;
    }
    
    // 根据费马小定理，求 n! 的逆元
    infact[n]=qmi(fact[n],MOD-2,MOD);
    
    // 倒推预处理所有的阶乘逆元：1/(i-1)! = 1/i! * i
    for(int i=n-1;i>=1;i--)
    {
        infact[i]=infact[i+1]*(i+1)%MOD;
    }
}

// --- O(1) 组合数查询 ---
int C(int n,int m)
{
    // 不合法判断，极为重要
    if(n<0||m<0||n<m)return 0;
    
    // C(n, m) = n! / (m! * (n-m)!)
    int res=fact[n]*infact[m]%MOD*infact[n-m]%MOD;
    return res;
}

void solve()
{
    /* 实战调用示例 
    // 必须在主程序最开头初始化，范围开到可能出现的最大下标 (n+m)
    init_comb(2000000); 
    
    // 示例 1: 求 sum(i=0 to k) C(n, i) * C(m, k-i)
    int n=1000, m=2000, k=500;
    // 暴力求和复杂度 O(K) 甚至 O(N)，化简后直接 O(1) 秒杀：
    int ans1 = C(n + m, k); 
    cout << ans1 << endl;
    
    // 示例 2: 求 sum(i=0 to n) C(n, i)^2
    // 结论：等于 C(2n, n)
    int n2=100000;
    int ans2 = C(2 * n2, n2);
    cout << ans2 << endl;
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