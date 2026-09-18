#include<bits/stdc++.h>
using namespace std;

#define int long long
#define endl '\n'

// 合并后的点：相同三元组只保留一个节点
struct Node
{
    int x,y,z;
    int cnt; // 当前三元组出现次数
    int ans; // 其他三元组对当前点的贡献
};

// 原始点：id 用于最后恢复输入顺序
struct Point
{
    int x,y,z,id;
};

vector<Node> a;
vector<int> p; // CDQ 中按第二维排序时保存下标
vector<int> tr; // 树状数组，维护第三维前缀和
int m; // 树状数组大小

// ================== 1. 树状数组 ==================

void add(int x,int v)
{
    for(;x<=m;x+=x&-x)
        tr[x]+=v;
}

// 查询第三维 <= x 的点数
int ask(int x)
{
    int ans=0;
    for(;x;x-=x&-x)
        ans+=tr[x];
    return ans;
}

// 当前 CDQ 层结束后，使用负值撤销 add 的修改
void clear_bit(int x,int v)
{
    add(x,-v);
}

// ================== 2. 排序比较 ==================

// 初始按照 (x,y,z) 字典序排序
bool cmp_xyz(const Point &u,const Point &v)
{
    if(u.x!=v.x)return u.x<v.x;
    if(u.y!=v.y)return u.y<v.y;
    if(u.z!=v.z)return u.z<v.z;
    return u.id<v.id;
}

// CDQ 合并时按照第二维 y 排序
bool cmp_y(int x,int y)
{
    if(a[x].y!=a[y].y)return a[x].y<a[y].y;
    if(a[x].z!=a[y].z)return a[x].z<a[y].z;
    return x<y;
}

// ================== 3. CDQ 分治 ==================

void cdq(int l,int r)
{
    if(l>=r)return;

    int mid=(l+r)>>1;
    cdq(l,mid);

    // 左右区间分别按 y 排序，p[i] 保存合并点编号
    for(int i=l;i<=r;i++)p[i]=i;
    sort(p.begin()+l,p.begin()+mid+1,cmp_y);
    sort(p.begin()+mid+1,p.begin()+r+1,cmp_y);

    int j=l;
    for(int k=mid+1;k<=r;k++)
    {
        int id=p[k];

        // 第一维由 CDQ 区间保证，第二维用双指针处理
        while(j<=mid&&a[p[j]].y<=a[id].y)
        {
            int q=p[j++];
            add(a[q].z,a[q].cnt);
        }

        // 树状数组再限制第三维 z <= a[id].z
        a[id].ans+=ask(a[id].z);
    }

    // 树状数组必须按加入时的权值撤销，不能直接清空节点
    for(int k=l;k<j;k++)
    {
        int q=p[k];
        clear_bit(a[q].z,a[q].cnt);
    }

    cdq(mid+1,r);
}

void solve()
{
    // 方便以后改成多组数据时复用全局模板
    a.clear();
    p.clear();
    tr.clear();

    int n;
    if(!(cin>>n))return;

    vector<Point> q(n);
    for(int i=0;i<n;i++)
    {
        cin>>q[i].x>>q[i].y>>q[i].z;
        q[i].id=i;
    }

    // 1. 按三维字典序排序，并将相同点合并
    sort(q.begin(),q.end(),cmp_xyz);
    vector<int> belong(n);

    for(int i=0;i<n;)
    {
        int j=i+1;
        while(j<n&&q[j].x==q[i].x&&q[j].y==q[i].y&&q[j].z==q[i].z)
            j++;

        int id=a.size();
        a.push_back({q[i].x,q[i].y,q[i].z,j-i,0});
        for(int k=i;k<j;k++)belong[q[k].id]=id;

        i=j;
    }

    // 2. 离散化第三维
    vector<int> z;
    for(Node &v:a)z.push_back(v.z);
    sort(z.begin(),z.end());
    z.erase(unique(z.begin(),z.end()),z.end());

    for(Node &v:a)
        v.z=lower_bound(z.begin(),z.end(),v.z)-z.begin()+1;

    // 3. CDQ 计算不同三元组之间的贡献
    m=z.size();
    tr.assign(m+1,0);
    p.resize(a.size());
    if(!a.empty())cdq(0,a.size()-1);

    // 当前点的重复点也满足三维偏序，需要补上 cnt
    for(int i=0;i<n;i++)
        cout<<a[belong[i]].ans+a[belong[i]].cnt<<endl;
}

signed main()
{
    cin.tie(0);
    ios::sync_with_stdio(0);

    int T=1;
    // cin>>T;
    while(T--)solve();

    return 0;
}
