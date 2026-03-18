#include<bits/stdc++.h>
using namespace std;
#define int long long
const int N = 1e5+10;

int w[N];
int tr[N<<2];
int add[N<<2];
void pushup(int u)
{
    tr[u]=max(tr[u<<1],tr[u<<1|1]);
}
void pushdown(int u,int l,int r)
{
    if(add[u])
    {
        tr[u<<1]+=add[u];
        tr[u<<1|1]+=add[u];
        add[u<<1]+=add[u];
        add[u<<1|1]+=add[u];
        add[u]=0;
    }
}

void build(int u,int l,int r)
{
    if(l==r)
    {
        tr[u]=w[l];
        return ;
    }
    int mid=l+r>>1;
    build(u<<1,l,mid);
    build(u<<1|1,mid+1,r);
    pushup(u);
}

void modify(int u,int l,int r,int L,int R,int v)
{
    if(L<=l&&r<=R)
    {
        tr[u]+=v;
        add[u]+=v;
        return ;
    }
    pushdown(u,l,r);
    int mid=l+r>>1;
    if(L<=mid)modify(u<<1,l,mid,L,R,v);
    if(R>mid)modify(u<<1|1,mid+1,r,L,R,v);
    pushup(u);
}

int query(int u,int l,int r,int L,int R)
{
    if(l>=L&&r<=R)return tr[u];
    pushdown(u,l,r);
    int mid=l+r>>1;
    int res=-4e18;
    if(L<=mid)res=max(res,query(u<<1,l,mid,L,R));
    if(R>mid)res=max(res,query(u<<1|1,mid+1,r,L,R));
    return res;
}

// //1初始化原数组 1 2 3 4 5
// for(int i=1;i<=n;i++)w[i]=i;
// //2建树
// build(1,1,n);
// //3区间修改：给区间 [2,4] 加上 10
// modify(1,1,n,2,4,10);
// //4区间查询：输出区间 [1,5] 的max
// cout<<query(1,1,n,1,5)<<endl;