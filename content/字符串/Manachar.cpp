#include<bits/stdc++.h>
using namespace std;
using PII = pair<int,int>;
#define int long long
#define endl '\n'
void solve()
{
    string s;
    cin>>s;
    string t="$#";
    for(auto c:s)
    {
        t+=c;
        t+="#";
    }
    t+='^';
    int n=t.size();
    vector<int>p(n+1);
    int C=0,R=0;//中心，最远R
    int mxle=0;
    for(int i=1;i<n-1;i++)
    {
        if(i<=R)p[i]=min(p[2*C-i],R-i+1);
        else p[i]=1;
        while(t[i-p[i]]==t[i+p[i]])p[i]++;
        if(i+p[i]-1>R)
        {
            C=i;
            R=i+p[i]-1;
        }
        if(p[i]-1>mxle)mxle=p[i]-1;
    }
    cout<<mxle<<endl;

}