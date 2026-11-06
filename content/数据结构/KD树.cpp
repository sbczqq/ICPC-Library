#include<bits/stdc++.h>
using namespace std;
#define int long long

const int N=2e5+10;
struct P
{
    int x,y;
}a[N];

struct T
{
    int l,r; // 左右儿子，0表示空
    int lx,rx,ly,ry; // 子树所有点的x、y坐标范围
}tr[N];

void pull(int u) // 用当前点和两棵子树更新包围矩形
{
    auto &t=tr[u];
    t.lx=t.rx=a[u].x;
    t.ly=t.ry=a[u].y;
    for(int v:{t.l,t.r})
    {
        if(!v)continue;
        t.lx=min(t.lx,tr[v].lx);
        t.rx=max(t.rx,tr[v].rx);
        t.ly=min(t.ly,tr[v].ly);
        t.ry=max(t.ry,tr[v].ry);
    }
}

int build(int l,int r,int d=0) // d=0按x划分，d=1按y划分
{
    if(l>r)return 0;
    int mid=(l+r)>>1;
    nth_element(a+l,a+mid,a+r+1,[d](const P &p,const P &q)
    {
        return d?p.y<q.y:p.x<q.x;
    }); // 只找中位数，两边不需要排好序

    tr[mid].l=build(l,mid-1,d^1);
    tr[mid].r=build(mid+1,r,d^1);
    pull(mid);
    return mid; // a[mid]是当前节点存的点
}

int dis(P a,P b) // 两点距离平方
{
    int dx=a.x-b.x,dy=a.y-b.y;
    return dx*dx+dy*dy;
}

int box(int u,P p) // p到子树包围矩形的最短距离平方
{
    if(!u)return LLONG_MAX;
    int dx=max({0LL,tr[u].lx-p.x,p.x-tr[u].rx}); // x在范围内时，横向距离为0
    int dy=max({0LL,tr[u].ly-p.y,p.y-tr[u].ry});
    return dx*dx+dy*dy;
}

int query(int u,P p,int best=LLONG_MAX)
{
    best=min(best,dis(a[u],p)); // 先拿当前节点的点更新答案
    int l=tr[u].l,r=tr[u].r;
    int dl=box(l,p),dr=box(r,p);
    if(dl>dr)
    {
        swap(l,r);
        swap(dl,dr); // 先搜离查询点更近的矩形
    }

    if(dl<best)best=query(l,p,best);
    if(dr<best)best=query(r,p,best); // 用更新后的best判断另一边还需不需要搜
    return best;
}

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n,q;
    cin>>n>>q; // n>=1，坐标绝对值不超过1e9
    for(int i=1;i<=n;i++)cin>>a[i].x>>a[i].y;
    int rt=build(1,n);
    while(q--)
    {
        P p;
        cin>>p.x>>p.y;
        cout<<query(rt,p)<<'\n'; // 输出最近距离的平方
    }
    return 0;
}
