#include<bits/stdc++.h>>
using namespace std;
using namespace std;
#define int long long
const int MOD = 998244353;
const int N = 4e5+10;

int fa[N],infa[N];


int qmi(int a,int k)
{
    int res=1;
    while(k)
    {
        if(k&1)res=res*a%MOD;
        k>>=1;
        a=a*a%MOD;
    }
    return res;
}
void init()
{
    fa[0]=infa[0]=1;
    for(int i=1;i<N;i++)fa[i]=i*fa[i-1]%MOD;
    infa[N-1]=qmi(fa[N-1],MOD-2);
    for(int i=N-2;i>0;i--)infa[i]=infa[i+1]*(i+1)%MOD;
}

int C(int a,int b)
{
    if(a<0||b<0||b>a)return 0;
    return fa[a]*infa[b]%MOD*infa[a-b]%MOD; 
}

int mulC(int k,int r,vector<int>&cn)
{
    int ans=0;
    for(int i=0;i<(1LL<<k);i++)
    {
        int lr=r;
        int pan=1;
        for(int j=0;j<k;j++)
        {
            if((i<<j)&1)
            {
                lr-=cn[j]+1;
                pan=-pan;
            }
        }
        if(lr<0)continue;
        ans=(ans+pan*C(lr+k-1,k-1)%MOD+MOD)%MOD;
    }
    return ans;
}