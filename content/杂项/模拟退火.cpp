#include<bits/stdc++.h>
using namespace std;

mt19937 rng(time(0)); // 用当前时间作为种子，初始化一次

int randint(int l,int r) // 随机取一个整数，范围 [l,r]
{
    return uniform_int_distribution<int>(l,r)(rng);
}

double randreal(double l,double r) // 随机取一个小数，范围 [l,r)
{
    return uniform_real_distribution<double>(l,r)(rng);
}

double calc(double x)
{
    return (x-3)*(x-3); // 要最小化的函数，最小值在 x=3 处
}

double sa(double x) // 从 x 出发，返回找到的最好位置
{
    double best=x;
    for(double t=100;t>1e-6;t*=0.99)
    {
        double y=x+randreal(-t,t); // 在 x 附近试一个新位置
        double d=calc(y)-calc(x); // d>0 表示新位置更差
        if(d<=0||randreal(0,1)<exp(-d/t))x=y; // 不变差就接受，变差时按概率接受
        if(calc(x)<calc(best))best=x;
    }
    return best;
}

int main()
{
    vector<int>a={1,2,3,4,5};
    shuffle(a.begin(),a.end(),rng); // 原地打乱 a
    for(int v:a)cout<<v<<" ";
    cout<<'\n';

    cout<<randint(1,10)<<'\n'; // 随机取 1 到 10 的整数，能取到 10
    cout<<randreal(0,1)<<'\n'; // 随机取 [0,1) 内的小数
    cout<<a[randint(0,(int)a.size()-1)]<<'\n'; // 随机选一个下标，取出元素

    bool ok=randreal(0,1)<0.3; // 约 30% 的概率为 true
    cout<<boolalpha<<ok<<'\n';

    cout<<fixed<<setprecision(6)<<sa(0)<<'\n'; // 从 0 开始找，结果应接近 3
    return 0;
}
