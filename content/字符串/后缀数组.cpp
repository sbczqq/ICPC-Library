#include<bits/stdc++.h>
using namespace std;
#define int long long
#define endl '\n'

const int N=2e5+10;
int sa[N],rk[N],height[N]; // 排名第i的后缀起点、起点为i的后缀排名、相邻排名后缀的最长公共前缀长度
int y[N],tmp[N],cnt[N]; // 按第二关键字排好的起点、新排名、计数排序的桶

void suffix_array(const string &s) // 构建非空字符串的后缀数组，结果下标从1开始
{
    int n=s.size(),m=256; // 字符串长度、当前排名的最大值
    fill(cnt+1,cnt+m+1,0);
    for(int i=1;i<=n;i++)cnt[rk[i]=(unsigned char)s[i-1]+1]++; // 初始按单个字符排名，0留给越界部分
    for(int i=1;i<=m;i++)cnt[i]+=cnt[i-1]; // 桶的前缀和，确定每种排名的结束位置
    for(int i=n;i>=1;i--)sa[cnt[rk[i]]--]=i; // 按第一字符进行计数排序

    for(int k=1;k<n;k<<=1) // 已按前k个字符排序，本轮扩展到前2k个字符
    {
        int p=0;
        for(int i=n-k+1;i<=n;i++)y[++p]=i; // 第二段越界，第二关键字为0，排在最前面
        for(int i=1;i<=n;i++)
            if(sa[i]>k)y[++p]=sa[i]-k; // 利用原sa顺序，得到按第二关键字排好的起点

        fill(cnt+1,cnt+m+1,0);
        for(int i=1;i<=n;i++)cnt[rk[y[i]]]++;
        for(int i=1;i<=m;i++)cnt[i]+=cnt[i-1];
        for(int i=n;i>=1;i--)sa[cnt[rk[y[i]]]--]=y[i]; // 稳定排序第一关键字，得到两关键字的顺序

        p=1;
        tmp[sa[1]]=1;
        for(int i=2;i<=n;i++)
        {
            int a=sa[i-1],b=sa[i]; // 比较排序后相邻的两个起点
            int u=a+k<=n?rk[a+k]:0,v=b+k<=n?rk[b+k]:0; // 两个起点的第二关键字
            if(rk[a]!=rk[b]||u!=v)p++; // 任一关键字不同，就划分一个新排名
            tmp[b]=p;
        }
        for(int i=1;i<=n;i++)rk[i]=tmp[i]; // 用新排名替换旧排名
        if(p==n)break; // 所有后缀已经区分，排序完成
        m=p; // 下一轮计数排序只需处理当前排名范围
    }

    for(int i=1;i<=n;i++)rk[sa[i]]=i; // 最终排名为1..n，兼容只有一个字符的情况
    int h=0;
    height[1]=0; // 排名第一的后缀没有前驱
    for(int i=1;i<=n;i++) // 按起点顺序，线性计算height
    {
        if(rk[i]==1)
        {
            h=0;
            continue;
        }
        if(h)h--; // 起点右移一位，可保留至少h-1个相同字符
        int j=sa[rk[i]-1]; // 当前后缀在字典序中的前一个后缀
        while(i+h<=n&&j+h<=n&&s[i+h-1]==s[j+h-1])h++; // 从已知公共前缀后继续比较
        height[rk[i]]=h;
    }
}

void solve()
{
    string s;
    cin>>s; // 输入一个非空字符串，示例为banana
    suffix_array(s);
    int n=s.size();
    for(int i=1;i<=n;i++)cout<<sa[i]<<" "; // 第一行：按字典序排列后的后缀起点
    cout<<endl;
    for(int i=1;i<=n;i++)cout<<rk[i]<<" "; // 第二行：每个起点对应的后缀排名
    cout<<endl;
    for(int i=1;i<=n;i++)cout<<height[i]<<" "; // 第三行：相邻排名后缀的最长公共前缀长度
    cout<<endl;
}

signed main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    solve();
    return 0;
}
