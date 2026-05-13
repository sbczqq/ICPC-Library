#include<bits/stdc++.h>
using namespace std;

#define int long long
#define endl '\n'

const int MAXN=1e6+10;

// --- 1. 求单个数的欧拉函数 O(sqrt(N)) ---
int get_phi(int n)
{
    int res=n;
    for(int i=2;i*i<=n;i++)
    {
        if(n%i==0)
        {
            // 发现质因数 i，利用公式：res = res * (1 - 1/i)
            res=res/i*(i-1); 
            // 榨干这个质因数，保证以后找到的因数必定是质数
            while(n%i==0)n/=i; 
        }
    }
    // n 最终没有被除尽，说明还有一个大于 sqrt(N) 的质因数
    if(n>1)res=res/n*(n-1);
    return res;
}

// --- 2. 线性筛求 1~N 所有的欧拉函数 O(N) ---
int primes[MAXN],cnt;
int phi[MAXN];
bool st[MAXN];

void get_phi_sieve(int n)
{
    phi[1]=1; // 1 的欧拉函数规定为 1
    for(int i=2;i<=n;i++)
    {
        if(!st[i])
        {
            primes[++cnt]=i;
            phi[i]=i-1; // 质数 p 的欧拉函数显然是 p-1
        }
        for(int j=1;primes[j]*i<=n;j++)
        {
            st[primes[j]*i]=true;
            if(i%primes[j]==0)
            {
                // 【核心逻辑 1】：primes[j] 已经是 i 的质因数了。
                // 根据欧拉函数质因数分解公式，新增加的这个 primes[j] 不会引入新的 (1 - 1/p) 项。
                // 所以直接纯乘进去即可。
                phi[primes[j]*i]=phi[i]*primes[j];
                break;
            }
            // 【核心逻辑 2】：primes[j] 是一个新的质因数。
            // 它是质数，由于 i 不包含它，两者互质，利用欧拉函数的积性：phi(a*b) = phi(a)*phi(b)
            // 即 phi[i * p] = phi[i] * phi[p] = phi[i] * (p - 1)
            phi[primes[j]*i]=phi[i]*(primes[j]-1);
        }
    }
}

// --- 3. 扩展欧拉定理 (处理超大指数降幂) ---
int qmi(int a,int k,int p)
{
    int res=1;
    a%=p; // 底数先取模，防爆 long long
    while(k)
    {
        if(k&1)res=res*a%p;
        a=a*a%p;
        k>>=1;
    }
    return res;
}

// 参数说明：底数 a，极大的指数字符串 b，模数 m
int ex_euler(int a,string b,int m)
{
    int p=get_phi(m); // 求出模数的欧拉函数 phi(m)
    int exp=0;
    bool flag=false;  // 标记真实的 b 是否 >= phi(m)
    
    // 边读入边取模，模拟高精度除法
    for(char c:b)
    {
        exp=exp*10+(c-'0');
        if(exp>=p)
        {
            flag=true; // 触发降幂条件
            exp%=p;
        }
    }
    
    // 【最致命的判断】：只有当指数 b 确实大于等于 phi(m) 时，指数才需要加上 phi(m)
    if(flag) exp+=p; 
    
    // 最后套用普通的快速幂即可
    return qmi(a,exp,m); 
}

void solve()
{
    /* 实战调用示例
    // 1. 求单个数
    // cout<<get_phi(12)<<endl; 
    
    // 2. 线性筛
    // get_phi_sieve(1000000); 
    
    // 3. 超大指数降幂: 求 a^b % m
    // int a, m; string b;
    // cin>>a>>m>>b;
    // cout<<ex_euler(a, b, m)<<endl;
    */
}