#include<bits/stdc++.h>
using namespace std;

const int MAXN=2e5+10;
int tr[MAXN];
int b[MAXN]; // 离散化数组
int tot;     // 去重后的元素总数

// 1. 离散化 (传入原数组和长度)
void di(int *a,int n)
{
    for(int i=1;i<=n;i++)b[i]=a[i];
    sort(b+1,b+n+1);
    tot=unique(b+1,b+n+1)-(b+1);
}

// 2. 获取离散化后的排名 (1-based)
int getid(int v)
{
    return lower_bound(b+1,b+tot+1,v)-b;
}

int low(int x)
{
    return x&-x;
}

// 3. 增加/减少元素个数 (x为离散化后的排名，v为变化量，通常传1或-1)
void add(int x,int v)
{
    // 注意：这里是 +=v，绝对不能写成 +=x
    for(int i=x;i<=tot;i+=low(i))tr[i]+=v;
}

// 4. 查询排名 (查小于等于排名 x 的数字总个数)
int query(int x)
{
    int res=0;
    for(int i=x;i;i-=low(i))res+=tr[i];
    return res;
}

// 5. 树上倍增求第 k 小 (返回的是离散化后的排名 idx)
int kth(int k)
{
    int idx=0;
    for(int i=18;i>=0;i--)
    {
        int nx=idx+(1<<i);
        if(nx<=tot&&tr[nx]<k)
        {
            idx=nx;
            k-=tr[nx];
        }
    }
    return idx+1;
}

// 6. 查前驱 (严格小于 x 的最大元素的排名)
int pre(int x)
{
    // 先查有多少个数严格小于 x
    int cnt=query(getid(x)-1);
    if(cnt==0) return -1; // 不存在前驱
    return kth(cnt);
}

// 7. 查后继 (严格大于 x 的最小元素的排名)
int suf(int x)
{
    // 先查有多少个数小于等于 x
    int cnt=query(getid(x));
    if(cnt==query(tot)) return -1; // 不存在后继
    return kth(cnt+1);
}

/*  使用提示：
pre(x) 和 suf(x) 返回的都是离散化后的排名！
如果你想输出真实的原数值，请在主函数中套一层 b[]：
int ans = b[pre(x)];
*/