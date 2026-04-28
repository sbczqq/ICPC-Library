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

// 求多边形面积 (传入按顺序排好顶点的 vector)
double pore(const vector<po>& ar)
{
    if(ar.size() < 3) return 0;
    
    int x = 0; 
    for(int i = 1; i < ar.size() - 1; ++i)
    {
        // 以 ar[0] 为原点进行剖分，累加有向面积
        x += cro(ar[0], ar[i], ar[i+1]);
    }
    
    return abs(x) / 2.0; 
}