#include<bits/stdc++.h>
using namespace std;

#define int long long
#define endl '\n'

const int MAXN=2e5+10;
const int MAXM=MAXN*32; 

int a[MAXN],b[MAXN],num;
int rt[MAXN];  
int lc[MAXM],rc[MAXM],sum[MAXM],tot;       

// --- 1. 离散化 ---
void discrete(int n)
{
    for(int i=1;i<=n;i++)b[i]=a[i];
    sort(b+1,b+n+1);
    num=unique(b+1,b+n+1)-(b+1);
}

// 获取离散化后的排名
int getid(int v)
{
    return lower_bound(b+1,b+num+1,v)-b;
}

// --- 2. 主席树核心 ---
void build(int &u,int l,int r)
{
    u=++tot;
    if(l==r)return;
    int mid=l+r>>1;
    build(lc[u],l,mid);
    build(rc[u],mid+1,r);
}

void update(int pre,int &u,int l,int r,int p)
{
    u=++tot;          
    lc[u]=lc[pre];    
    rc[u]=rc[pre];
    sum[u]=sum[pre]+1;
    if(l==r)return;
    
    int mid=l+r>>1;
    if(p<=mid)update(lc[pre],lc[u],l,mid,p);
    else update(rc[pre],rc[u],mid+1,r,p);
}

int query(int u,int v,int l,int r,int k)
{
    if(l==r)return l; 
    
    int x=sum[lc[v]]-sum[lc[u]]; 
    int mid=l+r>>1;
    
    if(k<=x)return query(lc[u],lc[v],l,mid,k);
    else return query(rc[u],rc[v],mid+1,r,k-x);
}

void solve()
{
    // 实战调用示例
    /*
    int n,q;
    cin>>n>>q;
    for(int i=1;i<=n;i++)cin>>a[i];
    
    discrete(n);          // 1. 离散化
    build(rt[0],1,num);   // 2. 建立空树 (版本0)
    
    // 3. 依次插入生成新版本
    for(int i=1;i<=n;i++)
    {
        update(rt[i-1],rt[i],1,num,getid(a[i]));
    }
    
    // 4. 处理查询
    while(q--)
    {
        int l,r,k;
        cin>>l>>r>>k;
        int rk=query(rt[l-1],rt[r],1,num,k); // 返回的是排名
        cout<<b[rk]<<endl;                   // 映射回原数值
    }
    */
}