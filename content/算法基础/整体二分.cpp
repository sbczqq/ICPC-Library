#include<bits/stdc++.h>
using namespace std;
#define int long long

const int N=2e5+10;
struct P
{
    int val,pos; // 元素值、在原数组中的下标
}a[N];

struct Q
{
    int l,r,k,id; // 查询区间、第k小、原询问编号
}que[N],tmp[N];

int n,tr[N],ans[N];

void add(int x,int v) // 在原数组的下标x处加标记
{
    for(;x<=n;x+=x&-x)tr[x]+=v;
}

int sum(int x)
{
    int res=0;
    for(;x;x-=x&-x)res+=tr[x];
    return res;
}

void solve(int vl,int vr,int ql,int qr) // a[vl..vr]是候选元素，que[ql..qr]是这一组询问
{
    if(ql>qr)return;
    if(vl==vr)
    {
        for(int i=ql;i<=qr;i++)ans[que[i].id]=a[vl].val;
        return;
    }

    int mid=(vl+vr)>>1;
    for(int i=vl;i<=mid;i++)add(a[i].pos,1); // 标记值较小的一半，位置仍用原下标

    int p=ql,t=qr;
    for(int i=ql;i<=qr;i++)
    {
        Q q=que[i];
        int cnt=sum(q.r)-sum(q.l-1); // 查询区间内有多少个被标记的元素
        if(q.k<=cnt)tmp[p++]=q; // 左半边的元素够多，第k小就在左边
        else
        {
            q.k-=cnt; // 跳过左半边的cnt个元素，在右半边找第k-cnt小
            tmp[t--]=q;
        }
    }

    for(int i=vl;i<=mid;i++)add(a[i].pos,-1); // 清掉本层标记，下一层重新标
    for(int i=ql;i<=qr;i++)que[i]=tmp[i];
    solve(vl,mid,ql,p-1);
    solve(mid+1,vr,p,qr); // 右组顺序可以反过来，答案按id输出
}

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);

    int m;
    cin>>n>>m; // 数组长度、询问数
    for(int i=1;i<=n;i++)
    {
        cin>>a[i].val;
        a[i].pos=i;
    }
    sort(a+1,a+n+1,[](P x,P y){return x.val<y.val;}); // 相同值也各占一个位置

    for(int i=1;i<=m;i++)
    {
        cin>>que[i].l>>que[i].r>>que[i].k;
        que[i].id=i;
    }

    solve(1,n,1,m);
    for(int i=1;i<=m;i++)cout<<ans[i]<<'\n';
    return 0;
}
