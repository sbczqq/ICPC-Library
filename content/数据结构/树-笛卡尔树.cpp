#include<bits/stdc++.h>
using namespace std;

const int N = 1e5 + 10;
vector<int>ar(N), ls(N), rs(N);
vector<int>st(N);
int top;
int build(int n)
{
    top = 0;
    for(int i = 1;i <= n;i++)ls[i] = rs[i] = 0;
    for(int i = 1;i <= n;i++)
    {
        int la = 0;
        while(top && ar[st[top]] > ar[i])
        {
            la = st[top];
            top--;
        }
        if(top)rs[st[top]] = i;
        ls[i] = la;
        st[++top] = i;
    }
    return st[1];
}