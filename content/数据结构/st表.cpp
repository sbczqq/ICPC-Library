#include<bits/stdc++.h>
using namespace std;

const int N = 2e5+10;
const int logN = 22;//LOG(n)
int f[N][logN],logn[N];

void pre()
{

    logn[1]=0;
    logn[2]=1;
    for(int i=3;i<N;i++)logn[i]=logn[i/2]+1;
}

void solve()
{
    int n;
    cin>>n;
    for(int i=1;i<=n;i++)cin>>f[i][0];
    pre();
    for(int j=1;(1<<j)<=n;j++)
    {
        for(int i=1;i+(1<<j)-1<=n;i++)
        {
            f[i][j]=max(f[i][j-1],f[i+(1<<(j-1))][j-1]);
        }
    }
    int l,r;
    cin>>l>>r;
    int s=logn[r-l+1];
    cout<<max(f[l][s],f[r-(1<<s)+1][s])<<endl;
}