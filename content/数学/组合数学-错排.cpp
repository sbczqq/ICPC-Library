#include<bits/stdc++.h>
using namespace std;
#define int long long
const int N = 1e6+10;
const int MOD = 1e9+7;
int d[N];
void pre()
{
    d[1]=0,d[2]=1;
    for(int i=3;i<N;i++)d[i]=(i-1)*(d[i-1]+d[i-2])%MOD;
}