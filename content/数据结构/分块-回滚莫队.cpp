#include<bits/stdc++.h>
using namespace std;
#define int long long
#define endl '\n'

const int N=2e5+10;
int a[N],cnt[N],ans[N]; // 原数组、出现次数、询问答案，a中的值作为cnt下标
int len,an; // 分块长度、当前区间内某个数的最大出现次数

struct Q
{
    int l,r,id; // 询问区间[l,r]、询问原编号
}q[N];

void add(int x) // 加入位置x，最大出现次数只会增加
{
    cnt[a[x]]++;
    an=max(an,cnt[a[x]]); // 更新当前最大出现次数
}

void del(int x) // 回滚位置x，只恢复出现次数
{
    cnt[a[x]]--; // an通过之前保存的值恢复，不在这里重新计算
}

bool cmp(const Q &x,const Q &y)
{
    int u=(x.l-1)/len,v=(y.l-1)/len;
    if(u!=v)return u<v; // 左端点按块编号排序
    return x.r<y.r; // 同一块内右端点升序，保证右端点只向右移动
}

void mo(int n,int m) // a[1..n]、q[1..m]提前填好，cnt初始为0
{
    len=max(1LL,(int)sqrt(n));
    sort(q+1,q+m+1,cmp);
    int last=-1,ed=0,r=0; // 上一个块编号、当前块右边界、永久区间右端点
    for(int i=1;i<=m;i++)
    {
        int u=(q[i].l-1)/len; // 当前询问左端点所在块
        if(u!=last) // 换块后，重新建立永久区间
        {
            while(r>ed)del(r--); // 清空上一块的永久区间[ed+1,r]
            an=0; // 当前区间已清空，答案重新初始化
            ed=min(n,(u+1)*len); // 当前块的右边界
            r=ed; // 永久区间从[ed+1,ed]这个空区间开始
            last=u;
        }

        while(r<q[i].r)add(++r); // 永久加入块右侧的位置，同一块内不回退
        int re=an; // 保存永久区间的答案，供临时加入后恢复
        int t=min(ed,q[i].r); // 同块询问只加到右端点，跨块询问加到块边界

        for(int j=t;j>=q[i].l;j--)add(j); // 临时加入块内部分，与永久区间组成询问区间
        ans[q[i].id]=an; // 保存当前询问的答案
        for(int j=q[i].l;j<=t;j++)del(j); // 按加入的逆序回滚块内部分
        an=re; // 恢复永久区间的答案，准备下一个询问
    }
}
