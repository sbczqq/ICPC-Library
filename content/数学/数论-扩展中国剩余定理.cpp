#include<bits/stdc++.h>
using namespace std;

#define int long long
#define endl '\n'

const int MAXN=1e5+10;

int a[MAXN]; // 余数数组 (x % m[i] = a[i])
int m[MAXN]; // 模数数组
int n;

// 扩展欧几里得算法 (求 ax + by = gcd(a,b))
int exgcd(int a,int b,int &x,int &y)
{
    if(!b){x=1;y=0;return a;}
    int d=exgcd(b,a%b,y,x);
    y-=a/b*x;
    return d;
}

// ---------------- 1. 普通中国剩余定理 (CRT) ----------------
// 要求：m[1]~m[n] 必须两两互质！
int CRT(int n,int *a,int *m)
{
    int M=1,ans=0;
    for(int i=1;i<=n;i++)M*=m[i];
    for(int i=1;i<=n;i++)
    {
        int Mi=M/m[i]; // 其他所有模数的乘积
        int x,y;
        exgcd(Mi,m[i],x,y); // 求 Mi 在模 m[i] 下的逆元 x
        x=(x%m[i]+m[i])%m[i]; // 保证逆元为正
        
        // ans = (ans + a[i] * Mi * x) % M
        // 为防溢出强转 __int128
        ans=(ans+(int)((__int128)a[i]*Mi%M*x%M))%M; 
    }
    return (ans%M+M)%M;
}

// ---------------- 2. 扩展中国剩余定理 (EXCRT) ----------------
// 通杀所有情况，m[i] 不互质也能做，发生矛盾无解时返回 -1
int EXCRT(int n,int *a,int *m)
{
    int M=m[1],ans=a[1]; // 初始维护第一个方程
    for(int i=2;i<=n;i++)
    {
        int x,y;
        int A=M,B=m[i],C=(a[i]-ans%B+B)%B;
        int d=exgcd(A,B,x,y); // 解方程 Ax + By = C
        
        if(C%d!=0)return -1; // gcd 不能整除 C，说明方程组内部自相矛盾，无解
        
        int bg=B/d;
        // x = x * (C / d) % (B / d)，强转防爆 long long
        x=(int)((__int128)x*(C/d)%bg);
        x=(x%bg+bg)%bg;
        
        // ans = ans + x * M
        ans=(ans+(int)((__int128)x*M));
        M=M/d*B; // M 更新为 lcm(M, m[i])
        ans=(ans%M+M)%M;
    }
    return (ans%M+M)%M;
}

void solve()
{
    /* 实战调用示例
    cin>>n;
    for(int i=1;i<=n;i++)cin>>m[i]>>a[i]; // 注意题目给的读入顺序是 m 还是 a
    int ans=EXCRT(n,a,m);
    cout<<ans<<endl;
    */
}