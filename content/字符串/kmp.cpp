#include<bits/stdc++.h>
using namespace std;

void solve()
{
    string s,p;
    cin>>s>>p;
    int n=s.size(),m=p.size();
    vector<int>ne(m+1);
    s=" "+s;
    p=" "+p;
    for(int i=2,j=0;i<=m;i++)
    {
        while(j&&p[i]!=p[j+1])j=ne[j];
        if(p[i]==p[j+1])j++;
        ne[i]=j;
    }
    int cn=0;
    for(int i=1,j=0;i<=n;i++)
    {
        while(j&&s[i]!=p[j+1])j=ne[j];
        if(s[i]==p[j+1])j++;
        if(j==m)
        {
            cn++;
            j=ne[j];
        }
    }
}