#include<bits/stdc++.h>
using namespace std;
using PII = pair<int,int>;
#define int long long
const int MOD = 1e9+10;

PII fib(int n)
{
    if(n==0)return {0,1};
    auto p=fib(n>>1);
    int fk=p.first,fk1=p.second;
    int c=(2*fk1*fk%MOD-fk*fk%MOD+MOD)%MOD;
    int d=(fk*fk%MOD+fk1*fk1%MOD)%MOD;
    if(n&1)return {d,(c+d)%MOD};
    return {c,d};
}
//使用 fin(20)代表求第20项