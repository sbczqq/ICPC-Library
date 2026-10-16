#include<bits/stdc++.h>
using namespace std;
#define int long long
#define endl '\n'
const int M=20;
int w[1LL<<M]; // 开始存原始数值，结束存每个下标的所有子集之和
void solve()
{
    int m;
    cin>>m; // 二进制位数，一共有2^m个下标，m<=M
    for(int k=0;k<(1LL<<m);k++)cin>>w[k]; // 初始时，每个位置只有自己的数值
    for(int j=0;j<m;j++) // 逐位处理：本轮允许删掉第j位的1
    {
        for(int k=(1LL<<m)-1;k>=0;k--) // 遍历所有下标，完成这一轮更新
        {
            if((k&(1LL<<j))!=0) // 第j位是1，才有保留和删除两种情况
            {
                w[k]+=w[k^(1LL<<j)]; // w[k]已有保留这一位的情况，再加入删除这一位的情况
            }
        }
    }
    for(int k=0;k<(1LL<<m);k++)cout<<w[k]<<" "; // 所有位处理完，输出每个下标的子集和
    cout<<endl;
}
signed main()
{
    cin.tie(0);
    ios::sync_with_stdio(0);
    solve();
    return 0;
}
