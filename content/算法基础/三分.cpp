#include<bits/stdc++.h>
using namespace std;

#define int long long
#define endl '\n'

using LD = long double;

const LD eps = 1e-8;

// 目标函数 (以求单峰函数最大值为例)
LD f_d(LD x)
{
    // return ...
    return 0; 
}

int f_i(int x)
{
    // return ...
    return 0;
}

// ---------------- 1. 浮点数极值 (偏移中点法) ----------------
LD ts_d(LD l, LD r)
{
    while(r-l>eps)
    {
        LD m=(l+r)/2.0;
        LD lm=m-eps/3.0, rm=m+eps/3.0;
        
        // 若左侧低于右侧，说明上升，极大值在右侧
        // 若求极小值，把 < 改成 >
        if(f_d(lm)<f_d(rm)) l=m;
        else r=m;
    }
    return l;
}


// ---------------- 2. 整数极值 (二分斜率法) ----------------
int ts_i(int l, int r)
{
    while(l<r)
    {
        int m=l+r>>1;
        // 直接比较 m 和 m+1 (相当于整数的微小偏移)
        // f(m) < f(m+1) 说明处于上升期，极值还在右边
        // 若求极小值，把 < 改成 >
        if(f_i(m)<f_i(m+1)) l=m+1;
        else r=m; // 下降期或平缓期，极值在 m 或 m 的左边
    }
    return l; // 返回极大值对应的 x 坐标
}