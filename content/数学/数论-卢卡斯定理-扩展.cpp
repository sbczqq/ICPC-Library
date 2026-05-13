#include<bits/stdc++.h>
using namespace std;

#define int long long
#define endl '\n'

// --- 通用工具函数 ---
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

void exgcd(int a,int b,int &x,int &y)
{
    if(!b){x=1;y=0;return;}
    exgcd(b,a%b,y,x);
    y-=a/b*x;
}

int inv(int a,int p)
{
    int x,y;
    exgcd(a,p,x,y);
    return (x%p+p)%p; // 保证逆元为正
}


// ================= 1. 普通卢卡斯定理 (模数 p 必须是质数) =================
// 求解组合数 C(a, b) % p
int C(int a,int b,int p)
{
    if(a<b)return 0;
    int res=1;
    for(int i=1,j=a;i<=b;i++,j--)
    {
        res=res*j%p;
        res=res*qmi(i,p-2,p)%p; // 质数求逆元直接用费马小定理
    }
    return res;
}

int lucas(int a,int b,int p)
{
    if(a<p&&b<p)return C(a,b,p);
    return C(a%p,b%p,p)*lucas(a/p,b/p,p)%p;
}
/* 💡 提示：如果有多组询问且 p <= 1e5，请把 C 函数改成预处理阶乘和阶乘逆元以做到 O(1) 查询。 */


// ================= 2. 扩展卢卡斯定理 (模数 P 可以是合数) =================
// 求解 n! % pk 孤立的部分 (即剔除掉所有 p 的倍数)
int fac(int n,int p,int pk)
{
    if(!n)return 1;
    int res=1;
    // 循环节部分
    for(int i=1;i<=pk;i++)
        if(i%p)res=res*i%pk;
    res=qmi(res,n/pk,pk);
    // 余数部分
    for(int i=1;i<=n%pk;i++)
        if(i%p)res=res*i%pk;
    return res*fac(n/p,p,pk)%pk; // 递归处理提取出来的 p 的倍数
}

// 求解组合数 C(n, m) % pk (pk = p^k)
int exC(int n,int m,int p,int pk)
{
    if(n<m)return 0;
    int f1=fac(n,p,pk),f2=fac(m,p,pk),f3=fac(n-m,p,pk);
    int cnt=0;
    // 勒让德定理：计算抠出来的 p 的总个数
    for(int i=n;i;i/=p)cnt+=i/p;
    for(int i=m;i;i/=p)cnt-=i/p;
    for(int i=n-m;i;i/=p)cnt-=i/p;
    
    // 互质部分相乘，因为此时互质，可以用 exgcd 求逆元
    return f1*inv(f2,pk)%pk*inv(f3,pk)%pk*qmi(p,cnt,pk)%pk;
}

int exlucas(int n,int m,int P)
{
    int res=0,tmp=P;
    // 唯一分解定理拆解模数 P
    for(int i=2;i*i<=tmp;i++)
    {
        if(tmp%i==0)
        {
            int pk=1;
            while(tmp%i==0)pk*=i,tmp/=i;
            // 求出在 pk 模下的答案 a
            int a=exC(n,m,i,pk);
            // CRT 中国剩余定理合并
            int Mi=P/pk;
            res=(res+a*Mi%P*inv(Mi,pk)%P)%P;
        }
    }
    // 扫尾处理最后一个大于 sqrt(P) 的质因子
    if(tmp>1)
    {
        int a=exC(n,m,tmp,tmp);
        int Mi=P/tmp;
        res=(res+a*Mi%P*inv(Mi,tmp)%P)%P;
    }
    return res;
}

void solve()
{
    /* 实战调用示例
    int n,m,p;
    cin>>n>>m>>p;
    // 如果 p 是质数
    // cout<<lucas(n,m,p)<<endl;
    // 如果 p 可能是合数
    // cout<<exlucas(n,m,p)<<endl;
    */
}