#include<bits/stdc++.h>
using namespace std;
#define int long long
#define endl '\n'

const int N = 1e5 + 10;

struct po {
    int x, y;
};

po p[N];
int st[N], top = 0; // st 栈存放凸包上的点在原数组中的下标

// 叉积: 向量 AB 叉乘 向量 AC
int cro(po a, po b, po c)
{
    return (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
}

// 距离平方: 用于极角相同时的排序
int dis(po a, po b)
{
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

// 极角排序比较函数
bool cmp(po a, po b)
{
    int cc = cro(p[1], a, b);
    if(cc > 0) return true;  // a 在 b 的顺时针前方（相对 p[1] 的极角更小）
    if(cc == 0) return dis(p[1], a) < dis(p[1], b); // 共线时，距离近的排前面
    return false;
}

void graham(int n)
{
    top = 0;
    // 点数 <= 3 时，全部都在凸包上
    if(n <= 3)
    {
        for(int i = 1; i <= n; i++) st[++top] = i;
        return;
    }
    
    // 1. 找 y 最小（且 x 最小）的点作为基准点
    int mn = 1;
    for(int i = 2; i <= n; i++)
    {
        if(p[i].y < p[mn].y || (p[i].y == p[mn].y && p[i].x < p[mn].x)) mn = i;
    }
    swap(p[1], p[mn]); // 将基准点换到第一个位置
    
    // 2. 将剩余点按相对 p[1] 的极角进行逆时针排序
    sort(p + 2, p + n + 1, cmp);
    
    // 3. 栈初始化，先压入前两个点
    st[++top] = 1;
    st[++top] = 2;
    
    // 4. 扫描后续点，维护单调栈
    for(int i = 3; i <= n; i++)
    {
        // 核心：若栈顶两点与新点构成的路径向右转或共线 (叉积 <= 0)，则栈顶出局
        while(top >= 2 && cro(p[st[top - 1]], p[st[top]], p[i]) <= 0)
        {
            top--;
        }
        st[++top] = i;
    }
}

/* 避坑指南：
 * 1. 共线处理：如果题目要求凸包边上的点也全部保留，遇到 cro == 0 时的出栈判定条件和排序逻辑需要特判修改。
 * 2. 坐标极值：如果点的坐标绝对值 >= 1e9，叉积计算过程中会爆 long long，必须转 __int128。
 */
