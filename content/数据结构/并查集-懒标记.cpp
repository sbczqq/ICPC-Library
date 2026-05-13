#include<bits/stdc++.h>
using namespace std;

#define int long long
#define endl '\n'

const int MAXN=2e5+10;

int fa[MAXN],d[MAXN]; // d[x] 维护 x 到 fa[x] 的权值

// 1. 初始化
void init(int n)
{
    for(int i=1;i<=n;i++)
    {
        fa[i]=i;
        d[i]=0; // 初始到自身的权值为 0
    }
}

// 2. 查 (带权路径压缩)
int find(int x)
{
    if(fa[x]==x)return x;
    int root=find(fa[x]); // 先递归找到根
    d[x]+=d[fa[x]];       // 懒标记下传：将父亲到根的权值累加到当前节点
    return fa[x]=root;    // 路径压缩
}

// 3. 并 (合并 x 和 y，并已知 x 到 y 的权值为 v)
// 即满足：d_actual[x] - d_actual[y] = v
void merge(int x,int y,int v)
{
    int fx=find(x),fy=find(y);
    if(fx!=fy)
    {
        fa[fx]=fy;
        // 向量推导：d[fx] = d[y] + v - d[x]
        d[fx]=d[y]+v-d[x];
    }
}

/* 💡 使用提示：
计算 x 和 y 的相对权值（距离）：
if(find(x) == find(y)) 
    int dist = d[x] - d[y];
*/