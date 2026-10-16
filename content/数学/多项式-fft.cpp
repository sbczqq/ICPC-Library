#include<bits/stdc++.h>
using namespace std;
#define int long long
#define endl '\n'

using cp=complex<double>;
const double pi=acos(-1);

void fft(vector<cp>&a,int op) // op=1为正变换，op=-1为逆变换，数组长度必须是2的幂
{
    int n=a.size();
    for(int i=1,j=0;i<n;i++) // 将下标调整为二进制位翻转后的顺序
    {
        int k=n>>1;
        for(;j&k;k>>=1)j^=k; // 从高位开始处理进位
        j^=k;
        if(i<j)swap(a[i],a[j]); // 每对位置只交换一次
    }

    for(int len=2;len<=n;len<<=1) // 从长度2开始，逐层合并两个较小的变换
    {
        double ang=2*pi/len*op;
        cp wn(cos(ang),sin(ang)); // 当前层的单位根
        for(int i=0;i<n;i+=len) // 每次处理一个长度为len的区间
        {
            cp w(1,0); // 单位根从0次幂开始
            for(int j=0;j<len/2;j++)
            {
                cp u=a[i+j],v=w*a[i+j+len/2]; // 左半部分、乘上单位根后的右半部分
                a[i+j]=u+v; // 蝶形合并的前半部分
                a[i+j+len/2]=u-v; // 蝶形合并的后半部分
                w*=wn; // 转到单位根的下一次幂
            }
        }
    }

    if(op==-1)
        for(auto &x:a)x/=n; // 逆变换最后除以长度，恢复原系数
}

vector<int>mul(const vector<int>&a,const vector<int>&b) // 两个非空整数系数多项式相乘
{
    int m=a.size()+b.size()-1,n=1; // 乘积的系数数量、FFT使用的长度
    while(n<m)n<<=1; // 补到不小于m的最小2次幂，避免循环卷积混叠

    vector<cp>x(n),y(n); // 其余位置自动补0
    for(int i=0;i<(int)a.size();i++)x[i]=a[i];
    for(int i=0;i<(int)b.size();i++)y[i]=b[i];

    fft(x,1);
    fft(y,1); // 将两个多项式从系数表示转换为点值表示
    for(int i=0;i<n;i++)x[i]*=y[i]; // 点值对应相乘，得到乘积的点值表示
    fft(x,-1); // 逆变换得到乘积的系数

    vector<int>ans(m);
    for(int i=0;i<m;i++)ans[i]=llround(x[i].real()); // 四舍五入为整数，支持负系数
    return ans;
}
