#include<bits/stdc++.h>
using namespace std;
#define int long long
#define endl '\n'

const int mod=998244353,g=3;

int qpow(int a,int b) // 快速计算a^b mod mod
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

void ntt(vector<int>&a,int op) // op=1为正变换，op=-1为逆变换，长度为2的幂且不超过2^23
{
    int n=a.size();
    for(int i=1,j=0;i<n;i++) // 将下标调整为二进制位翻转后的顺序
    {
        int k=n>>1;
        for(;j&k;k>>=1)j^=k; // 从高位开始处理进位
        j^=k;
        if(i<j)swap(a[i],a[j]); // 每对位置只交换一次
    }

    for(int len=2;len<=n;len<<=1) // 逐层合并两个较小的变换
    {
        int wn=qpow(g,(mod-1)/len); // 当前层的单位根，满足wn^len=1
        if(op==-1)wn=qpow(wn,mod-2); // 逆变换使用单位根的逆元
        for(int i=0;i<n;i+=len) // 每次处理一个长度为len的区间
        {
            int w=1; // 单位根从0次幂开始
            for(int j=0;j<len/2;j++)
            {
                int u=a[i+j],v=w*a[i+j+len/2]%mod;
                a[i+j]=(u+v)%mod; // 蝶形合并的前半部分
                a[i+j+len/2]=(u-v+mod)%mod; // 蝶形合并的后半部分
                w=w*wn%mod; // 转到单位根的下一次幂
            }
        }
    }

    if(op==-1)
    {
        int inv=qpow(n,mod-2); // 长度n在模意义下的逆元
        for(auto &x:a)x=x*inv%mod; // 逆变换最后除以长度，恢复原系数
    }
}

vector<int>mul(const vector<int>&a,const vector<int>&b) // 两个非空多项式相乘，返回系数模mod的结果
{
    int m=a.size()+b.size()-1,n=1; // 乘积的系数数量、NTT使用的长度
    while(n<m)n<<=1; // 补到不小于m的最小2次幂

    vector<int>x(n),y(n); // 其余位置自动补0
    for(int i=0;i<(int)a.size();i++)x[i]=(a[i]%mod+mod)%mod;
    for(int i=0;i<(int)b.size();i++)y[i]=(b[i]%mod+mod)%mod; // 将负系数也转为合法余数

    ntt(x,1);
    ntt(y,1); // 从系数表示转换为点值表示
    for(int i=0;i<n;i++)x[i]=x[i]*y[i]%mod; // 对应点值相乘
    ntt(x,-1); // 逆变换得到乘积的系数

    x.resize(m); // 去掉补0产生的多余位置
    return x;
}
