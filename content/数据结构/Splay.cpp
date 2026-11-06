#include<bits/stdc++.h>
using namespace std;
#define int long long

const int N=2e5+10;
struct T
{
    int ch[2],fa; // 左右儿子、父节点
    int val,cnt,sz; // 节点值、重复次数、子树元素总数
}tr[N]; // 0号是空节点，删除后不回收编号
int rt,tot; // 根节点编号、已分配节点数

void pushup(int u)
{
    tr[u].sz=tr[tr[u].ch[0]].sz+tr[tr[u].ch[1]].sz+tr[u].cnt;
}

int dir(int u)
{
    return tr[tr[u].fa].ch[1]==u; // 左儿子返回0，右儿子返回1
}

void rot(int u) // 把u旋到父节点f的位置
{
    int f=tr[u].fa,g=tr[f].fa,d=dir(u),v=tr[u].ch[d^1];
    if(g)tr[g].ch[dir(f)]=u; // 祖父原来连着f，现在改为连u
    tr[u].fa=g;
    tr[f].ch[d]=v; // 中间子树交给f，保持大小顺序
    if(v)tr[v].fa=f;
    tr[u].ch[d^1]=f; // f降下来，成为u的儿子
    tr[f].fa=u;
    pushup(f); // 先更新降下去的f，再更新u
    pushup(u);
}

void splay(int u) // 把u旋到根
{
    while(tr[u].fa)
    {
        int f=tr[u].fa,g=tr[f].fa;
        if(g)rot(dir(u)==dir(f)?f:u); // 同向先转父节点，反向先转u
        rot(u);
    }
    rt=u;
}

void insert(int x) // 插入一个x
{
    int u=rt,f=0;
    while(u&&tr[u].val!=x)
    {
        f=u;
        u=tr[u].ch[x>tr[u].val]; // 小往左，大往右
    }
    if(u)tr[u].cnt++; // 找到相同的值，只增加次数
    else
    {
        u=++tot;
        tr[u]={{0,0},f,x,1,1};
        if(f)tr[f].ch[x>tr[f].val]=u; // 把新节点挂到父节点下面
    }
    pushup(u);
    splay(u);
}

void erase(int x) // 删除一个x，不存在时不删
{
    int u=rt,f=0;
    while(u&&tr[u].val!=x)
    {
        f=u;
        u=tr[u].ch[x>tr[u].val];
    }
    if(!u)
    {
        if(f)splay(f); // 没找到也把最后访问的节点旋到根
        return;
    }

    splay(u); // 先把要删的节点旋到根
    if(tr[u].cnt>1)
    {
        tr[u].cnt--; // 还有重复值，保留这个节点
        pushup(u);
        return;
    }

    int l=tr[u].ch[0],r=tr[u].ch[1];
    if(!l)
    {
        rt=r; // 没有左子树，由右子树接管
        if(r)tr[r].fa=0;
        return;
    }

    tr[l].fa=0; // 左子树与待删节点断开
    while(tr[l].ch[1])l=tr[l].ch[1]; // 找左子树的最大值
    splay(l); // 把它旋到根，此时根没有右儿子
    tr[rt].ch[1]=r; // 把原来的右子树接上
    if(r)tr[r].fa=rt;
    pushup(rt);
}

int rankof(int x) // 严格小于x的元素数加1
{
    int u=rt,f=0,ans=1;
    while(u)
    {
        f=u;
        if(x>=tr[u].val)ans+=tr[tr[u].ch[0]].sz; // 左子树里的值都比x小
        if(x==tr[u].val)break;
        if(x>tr[u].val)ans+=tr[u].cnt; // 当前值的所有副本也比x小
        u=tr[u].ch[x>tr[u].val];
    }
    if(f)splay(f);
    return ans;
}

int kth(int k) // 查第k小，保证k合法
{
    int u=rt;
    while(1)
    {
        int l=tr[tr[u].ch[0]].sz;
        if(k<=l)u=tr[u].ch[0]; // 第k小在左子树
        else if(k>l+tr[u].cnt)
        {
            k-=l+tr[u].cnt; // 跳过左子树和当前值，再去右子树找
            u=tr[u].ch[1];
        }
        else
        {
            splay(u);
            return tr[u].val; // 第k小就是当前值
        }
    }
}

int pre(int x) // 前驱：严格小于x的最大值，保证存在
{
    int u=rt,last=0,ans=0;
    while(u)
    {
        last=u;
        if(tr[u].val<x)
        {
            ans=u; // 当前值可以当答案
            u=tr[u].ch[1]; // 往右找，看有没有更大的合法值
        }
        else u=tr[u].ch[0]; // 当前值太大或等于x，往左找
    }
    if(last)splay(last);
    return tr[ans].val;
}

int nxt(int x) // 后继：严格大于x的最小值，保证存在
{
    int u=rt,last=0,ans=0;
    while(u)
    {
        last=u;
        if(tr[u].val>x)
        {
            ans=u; // 当前值可以当答案
            u=tr[u].ch[0]; // 往左找，看有没有更小的合法值
        }
        else u=tr[u].ch[1]; // 当前值太小或等于x，往右找
    }
    if(last)splay(last);
    return tr[ans].val;
}

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n;
    cin>>n; // 操作数，初始树为空
    while(n--)
    {
        int op,x;
        cin>>op>>x;
        if(op==1)insert(x); // 插入x
        else if(op==2)erase(x); // 删除一个x
        else if(op==3)cout<<rankof(x)<<'\n'; // 查x的排名
        else if(op==4)cout<<kth(x)<<'\n'; // 查第x小
        else if(op==5)cout<<pre(x)<<'\n'; // 查x的前驱
        else if(op==6)cout<<nxt(x)<<'\n'; // 查x的后继
    }
    return 0;
}
