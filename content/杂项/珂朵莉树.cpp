#include<bits/stdc++.h>
using namespace std;

struct ODT{
    int l,r;
    mutable int v;//允许我们在不删节点的情况下直接修改 set 内部元素的值
    bool operator<(const ODT& x)const{
        return l<x.l;
    }
};
set<ODT>odt;
auto split(int pos)
{
    auto it=odt.lower_bound({pos,0,0});
    if(it!=odt.end()&&it->l==pos)return it;
    --it;
    int l=it->l,r=it->r;
    int v=it->v;
    odt.erase(it);
    odt.insert({l,pos-1,v});
    return odt.insert({pos,r,v}).first;
}
void assign(int l,int r,int v)
{
    auto itr=split(r+1);
    auto itl=split(l);
    odt.erase(itl,itr);
    odt.insert({l,r,v});
}

void init(int n)
{
    odt.clear();
    odt.insert({1,n,0});
}

// 扩展：区间加法
void add(int l, int r, long long val) {
    auto itr = split(r + 1);
    auto itl = split(l);
    for (auto it = itl; it != itr; ++it) {
        it->v += val; // mutable 发挥作用，直接修改值
    }
}

// 扩展：区间统计 / 暴力遍历
// 统计区间 [l, r] 内有多少个满足某种条件的值
long long query(int l, int r) {
    auto itr = split(r + 1);
    auto itl = split(l);
    long long res = 0;
    for (auto it = itl; it != itr; ++it) {
        // it->r - it->l + 1 就是该碎块的长度
        // 可以在这里做任何暴力统计，比如求和：res += (it->r - it->l + 1) * it->v;
    }
    return res;
}