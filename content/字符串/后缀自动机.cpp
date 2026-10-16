#include<bits/stdc++.h>
using namespace std;
#define int long long
#define endl '\n'

const int N=2e5+10;
struct T
{
    int ch[26],len,link,occ; // 字符转移、当前状态的最长长度、后缀链接、出现次数
}tr[N*2]; // 一个状态可能代表多个子串，最多预留约2n个状态
int tot=1,last=1; // 状态总数、整个当前字符串对应的状态，1为根，0表示不存在
int cnt[N],ord[N*2]; // 按len计数的桶、按len从小到大排列的状态编号

void extend(int c) // 在当前字符串末尾添加一个字符，c为0..25
{
    int cur=++tot,p=last; // cur代表新前缀的结束位置，p沿后缀链接向前寻找
    tr[cur].len=tr[last].len+1;
    tr[cur].occ=1; // 新增一个实际结束位置，先在新状态中记录一次
    while(p&&!tr[p].ch[c]) // 当前后缀还不能接字符c，补上通向cur的转移
    {
        tr[p].ch[c]=cur;
        p=tr[p].link; // 继续处理更短的后缀
    }

    if(!p)tr[cur].link=1; // 所有后缀都没有原有转移，后缀链接指向根
    else
    {
        int q=tr[p].ch[c]; // 找到原来已经存在的字符c转移
        if(tr[p].len+1==tr[q].len)
            tr[cur].link=q; // 长度刚好衔接，q就是新状态的后缀链接目标
        else
        {
            int clone=++tot; // 原状态需要拆分，新建克隆状态承接较短的一组子串
            tr[clone]=tr[q]; // 保留原状态的字符转移和后缀链接
            tr[clone].len=tr[p].len+1; // 克隆状态的最长长度改为当前需要的长度
            tr[clone].occ=0; // 克隆只拆分状态，没有新增实际结束位置
            while(p&&tr[p].ch[c]==q)
            {
                tr[p].ch[c]=clone; // 将较短后缀原本通向q的转移改为通向clone
                p=tr[p].link;
            }
            tr[q].link=tr[cur].link=clone; // 原状态和新状态都通过clone连接更短的后缀
        }
    }
    last=cur; // 整个当前字符串对应的新状态
}

void build(const string &s) // 输入非空小写字母串，构建并统计出现次数，支持重新构建
{
    for(int i=1;i<=tot;i++)tr[i]={}; // 清空上次实际用到的状态
    tot=last=1;
    for(char c:s)extend(c-'a'); // 从左到右逐个加入字符

    int n=s.size();
    fill(cnt,cnt+n+1,0);
    for(int i=1;i<=tot;i++)cnt[tr[i].len]++; // 按状态最长长度进行计数排序
    for(int i=1;i<=n;i++)cnt[i]+=cnt[i-1];
    for(int i=tot;i>=1;i--)ord[cnt[tr[i].len]--]=i;
    for(int i=tot;i>=2;i--) // 从长状态向短状态传播，根位于ord[1]，不再向外传播
    {
        int p=ord[i];
        tr[tr[p].link].occ+=tr[p].occ; // 当前串每次出现，它的后缀也会在同一位置结束
    }
}

int query(const string &s) // 查询非空小写字母串的出现次数，允许重叠
{
    int p=1; // 从根开始，依次沿字符转移
    for(char c:s)
    {
        int x=c-'a';
        if(!tr[p].ch[x])return 0; // 没有对应转移，说明这个串不是原串的子串
        p=tr[p].ch[x];
    }
    return tr[p].occ; // 到达的状态中，所有子串具有相同的出现次数
}

int distinct() // 统计不同的非空子串数量
{
    int an=0;
    for(int p=2;p<=tot;p++) // 根代表空串，不参与统计
        an+=tr[p].len-tr[tr[p].link].len; // 当前状态每种合法长度恰好代表一个子串
    return an;
}

void solve()
{
    string s;
    cin>>s; // 输入非空小写字母串，示例为ababa
    build(s);
    cout<<distinct()<<endl; // 第一行输出不同的非空子串数量
    int q;
    cin>>q; // 接下来输入q个要查询的非空小写字母串
    while(q--)
    {
        string t;
        cin>>t;
        cout<<query(t)<<endl; // 输出t在原串中的出现次数，重叠出现也计算
    }
}

signed main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    solve();
    return 0;
}
