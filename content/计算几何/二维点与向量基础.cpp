#include<bits/stdc++.h>
using namespace std;
#define int long long
#define endl '\n'

struct po {
    int x, y;
};

// 叉积: 向量AB × 向量AC (>0逆时针, <0顺时针, =0共线)
int cro(po a, po b, po c)
{
    return (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
}

// 点积: 向量AB · 向量AC (>0锐角, <0钝角, =0垂直)
int dot(po a, po b, po c)
{
    return (b.x - a.x) * (c.x - a.x) + (b.y - a.y) * (c.y - a.y);
}

// 距离平方: 省略 sqrt 防止浮点误差
int dis(po a, po b)
{
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

/* 避坑指南：
 * 若坐标绝对值达 1e9，叉积/点积相乘会爆 long long。
 * 此时必须将返回值和内部强转为 __int128。
 */
