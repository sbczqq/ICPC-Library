#include<bits/stdc++.h>
using namespace std;
void solve()
{
    int n;
    cin>>n;
    int ans=0;
    for(int l=0,r;l<=n;l=r+1)
    {
        r=n/(n/l);
        ans+=(r-l+1)*(n/l);
        l=r+1;
    }
}