#include<bits/stdc++.h>
using namespace std;

#define int long long
#define endl '\n'

const int MAXN=2e5+10; // 注意：N个矩形有2N条边，数组必须开两倍

// 扫描线结构体
struct Line
{
    int x,y1,y2,mar;
}line[MAXN];

int Y[MAXN];      // 离散化数组
int tr[MAXN<<2];  // 维护区间有效覆盖长度
int cnt[MAXN<<2]; // 维护区间被完全覆盖的次数（Lazy Tag的变体，不需要pushdown）

bool cmp(Line a,Line b)
{
    return a.x<b.x; // 按横坐标从左到右排序
}

// 向上更新区间有效长度
void pushup(int u,int l,int r)
{
    if(cnt[u])tr[u]=Y[r+1]-Y[l]; // 如果被完全覆盖，长度就是真实的物理距离
    else if(l==r)tr[u]=0;        // 叶子节点且未被完全覆盖，长度为0
    else tr[u]=tr[u<<1]+tr[u<<1|1]; // 由左右子节点拼凑
}

// 区间修改 (L, R 为目标离散化区间边界)
void modify(int u,int l,int r,int L,int R,int v)
{
    if(L<=l&&r<=R)
    {
        cnt[u]+=v;
        pushup(u,l,r); // 更新有效长度
        return ;
    }
    int mid=(l+r)>>1;
    if(L<=mid)modify(u<<1,l,mid,L,R,v);
    if(R>mid)modify(u<<1|1,mid+1,r,L,R,v);
    pushup(u,l,r);
}

void solve()
{
    int n;
    cin>>n;
    int tot=0;
    for(int i=1;i<=n;i++)
    {
        int x1,y1,x2,y2;
        cin>>x1>>y1>>x2>>y2; 
        
        line[++tot]={x1,y1,y2,1};  // 左边入边，权值为 1
        Y[tot]=y1;
        line[++tot]={x2,y1,y2,-1}; // 右边出边，权值为 -1
        Y[tot]=y2;
    }
    
    // 离散化 Y 坐标
    sort(Y+1,Y+tot+1);
    int num=unique(Y+1,Y+tot+1)-(Y+1);
    
    // 按 X 坐标排序垂直扫描线
    sort(line+1,line+tot+1,cmp);
    
    int ans=0;
    for(int i=1;i<tot;i++) // 最后一条边不需要再计算面积，所以 i < tot
    {
        // 映射到离散化下标，注意右边界 R 是 lower_bound 找到的下标减 1
        int L=lower_bound(Y+1,Y+num+1,line[i].y1)-Y;
        int R=lower_bound(Y+1,Y+num+1,line[i].y2)-Y-1;
        
        modify(1,1,num-1,L,R,line[i].mar);
        
        // 累加面积：线段树根节点维护的总有效高度 * 扫描线推进的横向距离
        ans+=tr[1]*(line[i+1].x-line[i].x);
    }
    cout<<ans<<endl;
}