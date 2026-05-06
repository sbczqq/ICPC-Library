#include<bits/stdc++.h>
using namespace std;

#define int long long
#define endl '\n'

const int MAXB=62; // 值域上限，1e18 对应 62

int d[65]; // 线性基主数组
int p[65]; // 重构后的数组
int cnt;   // 重构后的有效基底个数
bool zer;  // 标记原序列能否异或出 0

// 1. 插入元素
void insert(int x)
{
    for(int i=MAXB;i>=0;i--)
    {
        if(!((x>>i)&1))continue; 
        if(!d[i])
        {
            d[i]=x;
            return ;
        }
        x^=d[i];
    }
    zer=1; // 如果被完全消掉，说明能异或出0
}

// 2. 判断 x 能否被线性基表示
bool check(int x)
{
    if(x==0)return zer;
    for(int i=MAXB;i>=0;i--)
    {
        if((x>>i)&1)x^=d[i]; 
    }
    return x==0;
}

// 3. 查最大异或和
int qmax()
{
    int res=0;
    for(int i=MAXB;i>=0;i--)
    {
        res=max(res,res^d[i]);
    }
    return res;
}

// 4. 查最小异或和 (非空子集)
int qmin()
{
    if(zer)return 0; // 如果能异或出0，最小就是0
    for(int i=0;i<=MAXB;i++)
    {
        if(d[i])return d[i]; // 找到最底层的基底就是大于0的最小值
    }
    return 0;
}

// 5. 线性基重构 (求第 k 小的前置操作)
void rebuild()
{
    cnt=0;
    for(int i=MAXB;i>=0;i--)
    {
        for(int j=i-1;j>=0;j--)
        {
            if((d[i]>>j)&1)d[i]^=d[j];
        }
    }
    for(int i=0;i<=MAXB;i++)
    {
        if(d[i])p[cnt++]=d[i];
    }
}

// 6. 查第 k 小异或和 (必须先 rebuild)
int kth(int k)
{
    if(zer)k--;
    if(k==0)return 0;
    if(k>=(1LL<<cnt))return -1; // 1LL防止爆int
    int res=0;
    for(int i=0;i<cnt;i++)
    {
        if((k>>i)&1)res^=p[i];
    }
    return res;
}