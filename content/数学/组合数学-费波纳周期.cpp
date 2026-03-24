#include<bits/stdc++.h>
using namespace std;
#define int long long
using PII = pair<int,int>;
int MOD;

PII fib(int n)
{
    if(n==0)return {0,1};
    auto p=fib(n>>1);
    int fk=p.first,fk1=p.second;
    int c=fk*((2*fk1-fk+MOD)%MOD)%MOD;
    int d=(fk*fk%MOD+fk1*fk1%MOD)%MOD;
    if(n&1)return {d,(c+d)%MOD};
    return {c,d};
}

int qpow(int a,int k)
{
    int re=1;
    while(k)
    {
        if(k&1)re=a*re;
        a=a*a;
        k>>=1;
    }
    return re;
}

int lcm(int a,int b)
{
    return a/__gcd(a,b)*b;
}

int peropd(int m)
{
    int re=1,tem=m;
    for(int i=2;i*i<=tem;i++)
    {
        if(tem%i==0)
        {
            int p=i,e=0;
            while(tem%i==0)
            {
                tem/=i;
                e++;
            }
            int cur=qpow(p,e-1);
            if(p==2)cur*=3;
            else if(p==5)cur*=20;
            else if(p%5==1||p%5==4)cur*=(p-1);
            else cur*=2*(p+1);
            re=lcm(re,cur);
        }
    }
    if(tem>1)
    {
        int p=tem;
        int cur=1;
        if(p==2)cur*=3;
        else if(p==5)cur*=20;
        else if(p%5==1||p%5==4)cur*=(p-1);
        else cur*=2*(p+1);
        re=lcm(re,cur);       
    }
    return re;
}
/*使用 求周期P=peropd( MOD)
知道周期后
int n_mod = 0;
    for(char c : n_str) {
        n_mod = (n_mod * 10 + (c - '0')) % P;
    }
    cout<<fib(n_mod).first
*/

