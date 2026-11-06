#include<bits/stdc++.h>
using namespace std;

const int N=2e5+10;
struct T
{
    int l,r,val,sz; // 左右儿子、当前值、子树大小
    unsigned pri; // 随机优先级
    bool rev; // 子树还有翻转标记没有下传
}tr[N];
int rt,tot;
mt19937 rng(time(0));

int newnode(int x)
{
    int u=++tot;
    tr[u]={0,0,x,1,(unsigned)rng(),false};
    return u;
}

void pushup(int u)
{
    tr[u].sz=tr[tr[u].l].sz+tr[tr[u].r].sz+1;
}

void flip(int u)
{
    if(!u)return;
    swap(tr[u].l,tr[u].r); // 翻转整棵子树，先交换左右两边
    tr[u].rev^=1; // 两边内部也要翻，先记下来；翻两次正好抵消
}

void pushdown(int u)
{
    if(!tr[u].rev)return;
    flip(tr[u].l);
    flip(tr[u].r);
    tr[u].rev=0; // 自己的左右儿子早已交换，这里只把标记传给儿子
}

pair<int,int> split(int u,int k) // 按位置拆成前k个和剩下的，返回两棵树的根
{
    if(!u)return {0,0};
    pushdown(u); // 先把翻转传下去，再按当前顺序拆
    int left=tr[tr[u].l].sz;
    if(k<=left)
    {
        auto [a,b]=split(tr[u].l,k);
        tr[u].l=b; // 左子树没拆走的部分，仍在u的左边
        pushup(u);
        return {a,u};
    }

    auto [a,b]=split(tr[u].r,k-left-1); // 左子树和u已经占了left+1个
    tr[u].r=a;
    pushup(u);
    return {u,b};
}

int merge(int a,int b) // 把序列a接在序列b前面，按随机优先级选根
{
    if(!a||!b)return a?a:b;
    if(tr[a].pri<tr[b].pri)
    {
        pushdown(a);
        tr[a].r=merge(tr[a].r,b);
        pushup(a);
        return a;
    }

    pushdown(b);
    tr[b].l=merge(a,tr[b].l);
    pushup(b);
    return b;
}

void reverse_range(int l,int r)
{
    auto [a,bc]=split(rt,l-1); // 拆出[1,l-1]
    auto [b,c]=split(bc,r-l+1); // 再拆出[l,r]，c是剩余后缀
    flip(b);
    rt=merge(merge(a,b),c); // 按原来三段的顺序拼回去
}

void print(int u) // 中序遍历就是当前序列顺序
{
    if(!u)return;
    pushdown(u);
    print(tr[u].l);
    cout<<tr[u].val<<' ';
    print(tr[u].r);
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n,m;
    cin>>n>>m; // 初始序列为1..n，接下来m次翻转
    for(int i=1;i<=n;i++)rt=merge(rt,newnode(i));
    while(m--)
    {
        int l,r;
        cin>>l>>r; // 当前序列中的位置，从1开始
        reverse_range(l,r);
    }
    print(rt);
    cout<<'\n';
    return 0;
}
