#include<bits/stdc++.h>
using namespace std;
#define int long long
#define endl '\n'
vector<int>mu,pri,su,st; // mu为莫比乌斯函数，pri存质数，su为mu的前缀和，st标记合数
void init(int n)
{
    mu.assign(n+1,0); // 初始化mu
    st.assign(n+1,0); // 初始化合数标记
    su.assign(n+1,0); // 初始化前缀和，su[0]=0
    pri.clear(); // 清空质数，支持重复调用
    mu[1]=1; // 特殊定义：mu(1)=1
    for(int i=2;i<=n;i++)
    {
        if(!st[i]) // 没有被更小的数筛到，说明i是质数
        {
            pri.push_back(i); // 记录质数，保持从小到大
            mu[i]=-1; // 只有一个质因子，所以mu(i)=-1
        }
        for(auto p:pri) // 用质数p筛出合数i*p
        {
            if(p>n/i)break; // 等价于i*p>n，同时避免乘法溢出
            st[i*p]=1; // 标记i*p为合数
            if(i%p==0) // i已经含有p，再乘p就含有p²
            {
                mu[i*p]=0; // 含有平方质因子，mu为0
                break; // 保证每个合数只由它的最小质因子筛一次
            }
            mu[i*p]=-mu[i]; // 新增一个不同质因子，正负翻转；原来为0则仍为0
        }
    }
    for(int i=1;i<=n;i++)su[i]=su[i-1]+mu[i]; // 区间mu之和为su[r]-su[l-1]
}
int cal(int n,int m) // 统计1<=a<=n、1<=b<=m且gcd(a,b)=1的有序数对
{
    int an=0,lim=min(n,m); // 公约数d不会超过两个范围的较小值
    for(int l=1,r;l<=lim;l=r+1) // 每次处理一段两个商都不变的区间
    {
        int x=n/l,y=m/l; // 当前区间内，两个范围中d的倍数分别有x、y个
        r=min(n/x,m/y); // 两个商分别不变到n/x、m/y，取较小的右端点
        an+=(su[r]-su[l-1])*x*y; // 合并这一段的mu[d]*(n/d)*(m/d)
    }
    return an; // 互质数对最终贡献1，不互质数对的容斥贡献抵消为0
}
void solve()
{
    int n,m,k;
    cin>>n>>m>>k; // 输入正整数n、m、k，要求gcd(a,b)=k
    n/=k; // 令a=k*x，则1<=x<=原来的n/k
    m/=k; // 令b=k*y，原条件等价于gcd(x,y)=1
    if(!n||!m) // 某个范围内没有正的k的倍数
    {
        cout<<0<<endl;
        return;
    }
    init(min(n,m)); // 只需预处理求和中会用到的mu
    cout<<cal(n,m)<<endl; // 输入6 6 1时，输出23
}
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    solve();
}
