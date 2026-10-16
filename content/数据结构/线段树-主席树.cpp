#include<bits/stdc++.h>
using namespace std;
#define int long long
#define endl '\n'

const int N=2e5+10;
int rt[N],tot; // rt[i]表示前i个数形成的版本，tot为已创建的节点数

struct T
{
    int ls,rs,sum; // 左儿子、右儿子、当前值域内的数的个数
}tr[N*20]; // 每次插入只新建根到叶子的一条链，零号节点表示空树

int update(int pre,int l,int r,int x) // 在旧版本pre上插入值x，返回新版本的根
{
    int p=++tot; // 新建节点，避免修改旧版本
    tr[p]=tr[pre]; // 复制旧节点的信息，未修改的子树继续共用
    tr[p].sum++; // 插入一个数，当前值域内的数量加1
    if(l==r)return p; // 到达值x对应的叶子，结束插入

    int mid=(l+r)>>1;
    if(x<=mid) // x在左半值域，只新建左侧路径
        tr[p].ls=update(tr[pre].ls,l,mid,x);
    else // x在右半值域，只新建右侧路径
        tr[p].rs=update(tr[pre].rs,mid+1,r,x);

    return p; // 返回新节点，供上一层连接
}

int query(int u,int v,int l,int r,int k) // 查询版本v减去版本u后，第k小的值
{
    if(l==r)return l; // 值域只剩一个值，它就是答案

    int mid=(l+r)>>1;
    int cnt=tr[tr[v].ls].sum-tr[tr[u].ls].sum; // 两个版本相减，得到查询区间落在左半值域的数量

    if(k<=cnt) // 左半值域已有至少k个数，第k小在左侧
        return query(tr[u].ls,tr[v].ls,l,mid,k);
    else // 跳过左侧cnt个数，寻找右侧第k-cnt小
        return query(tr[u].rs,tr[v].rs,mid+1,r,k-cnt);
}
