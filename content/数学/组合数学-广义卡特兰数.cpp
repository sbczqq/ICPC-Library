#include<bits/stdc++.h>
using namespace std;
#define int long long
#define endl '\n'

const int mod=1e9+7;
vector<int>fac,inv; // 阶乘、阶乘的逆元

int qpow(int a,int b)
{
    int an=1;
    while(b)
    {
        if(b&1)an=an*a%mod;
        a=a*a%mod;
        b>>=1;
    }
    return an;
}

void init(int n) // 预处理到最大k*n，预处理范围小于mod
{
    fac.assign(n+1,1);
    inv.assign(n+1,1);
    for(int i=1;i<=n;i++)fac[i]=fac[i-1]*i%mod;
    inv[n]=qpow(fac[n],mod-2);
    for(int i=n;i>=1;i--)inv[i-1]=inv[i]*i%mod;
}

int C(int n,int m)
{
    if(m<0||m>n)return 0;
    return fac[n]*inv[m]%mod*inv[n-m]%mod;
}

int catalan(int n,int k=2) // 广义卡特兰数，n>=0、k>=2，提前预处理到k*n
{
    return C(k*n,n)*qpow((k-1)*n+1,mod-2)%mod; // C(k*n,n)/((k-1)*n+1)，k=2即普通卡特兰数
}
