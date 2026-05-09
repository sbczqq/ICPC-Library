#include<bits/stdc++.h>
using namespace std;

#define int long long
#define endl '\n'

// MAXN 必须开到 N + 最大删除操作次数
const int MAXN=5e5+10; 

int fa[MAXN]; // 实际构建树结构的虚拟节点数组
int id[MAXN]; // 真实节点 x 映射到的虚拟节点编号
int sz[MAXN]; // 维护集合大小
int tot;      // 虚拟节点总数

// 1. 初始化
void init(int n)
{
    tot=n; 
    // 注意：必须初始化到 MAXN，因为后续会不断分配新节点
    for(int i=1;i<MAXN;i++) 
    {
        fa[i]=i;
        sz[i]=1;
        id[i]=i; 
    }
}

// 2. 查 (带路径压缩，注意传进来的是虚拟节点)
int find(int x)
{
    return fa[x]==x?x:fa[x]=find(fa[x]);
}

// 3. 并
void merge(int x,int y)
{
    // 操作的必须是 x 和 y 当前对应的虚拟节点
    int fx=find(id[x]),fy=find(id[y]);
    if(fx!=fy)
    {
        fa[fx]=fy;
        sz[fy]+=sz[fx];
    }
}

// 4. 删 (把 x 从当前集合中剥离，变成孤立点)
void del(int x)
{
    int fx=find(id[x]);
    sz[fx]--;        // 原本所在的集合大小减 1
    
    id[x]=++tot;     // 给 x 分配一个新的虚拟节点作为新身份
    fa[id[x]]=id[x]; // 新身份自成一派
    sz[id[x]]=1;     // 新身份所在集合大小为 1
}