#include<bits/stdc++.h>
using namespace std;
#define int long long
#define endl '\n'

const int N=2e5+10;
int a[N],cnt[N],ans[N]; // 原数组、当前区间内每个值的出现次数、每个询问的答案，a中的值作为cnt下标
int len,an; // 分块长度、当前区间内不同数的个数

struct Q
{
    int l,r,id; // 询问区间[l,r]、询问的原始编号
}q[N];

void add(int x) // 将位置x加入当前区间，按题目修改维护逻辑
{
    if(cnt[a[x]]==0)an++; // 加入前没有这个值，不同数个数加1
    cnt[a[x]]++; // 这个值的出现次数加1
}

void del(int x) // 将位置x移出当前区间，按题目修改维护逻辑
{
    cnt[a[x]]--; // 这个值的出现次数减1
    if(cnt[a[x]]==0)an--; // 最后一次出现被移除，不同数个数减1
}

bool cmp(const Q &x,const Q &y) // 调整询问顺序，减少区间端点移动次数
{
    int u=(x.l-1)/len,v=(y.l-1)/len; // 两个询问的左端点所在块，块编号从0开始
    if(u!=v)return u<v; // 不同块按块编号升序排列
    if(u&1)return x.r>y.r; // 同一奇数块内，右端点降序排列
    return x.r<y.r; // 同一偶数块内，右端点升序排列，形成交替移动顺序
}

void mo(int n,int m) // 数组长度n、询问数量m，处理静态数组的离线区间查询
{
    len=max(1LL,(int)sqrt(n)); // 块长取根号n，至少为1
    sort(q+1,q+m+1,cmp); // a[1..n]、q[1..m]提前填好，q[i].id记录原编号
    int l=1,r=0; // 当前维护区间为[l,r]，初始为空，cnt和an需为0
    for(int i=1;i<=m;i++) // 按莫队顺序处理询问
    {
        while(l>q[i].l)add(--l); // 左端点向左移动，先移动再加入新位置
        while(r<q[i].r)add(++r); // 右端点向右移动，先移动再加入新位置
        while(l<q[i].l)del(l++); // 左端点向右移动，先移除旧位置再移动
        while(r>q[i].r)del(r--); // 右端点向左移动，先移除旧位置再移动
        ans[q[i].id]=an; // 当前区间已变为询问区间，按原编号保存答案
    }
}
