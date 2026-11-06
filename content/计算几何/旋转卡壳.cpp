#include<bits/stdc++.h>
using namespace std;
#define int long long

struct po
{
    int x,y;
};

int cro(po a,po b,po c) // AB叉乘AC，是三角形ABC的两倍有向面积
{
    return (b.x-a.x)*(c.y-a.y)-(b.y-a.y)*(c.x-a.x);
}

int dis(po a,po b) // 两点距离平方
{
    int dx=a.x-b.x,dy=a.y-b.y;
    return dx*dx+dy*dy;
}

vector<po> hull(vector<po>&p) // Andrew求逆时针凸包，不保留边上多余的共线点
{
    sort(p.begin(),p.end(),[](po a,po b)
    {
        if(a.x!=b.x)return a.x<b.x;
        return a.y<b.y;
    });
    p.erase(unique(p.begin(),p.end(),[](po a,po b)
    {
        return a.x==b.x&&a.y==b.y;
    }),p.end()); // 去掉重复点

    if(p.empty())return {};
    if(p.size()==1)return {p[0]};
    vector<po>h;
    for(po v:p) // 从左往右建下半边
    {
        while(h.size()>=2&&cro(h[h.size()-2],h.back(),v)<=0)h.pop_back();
        h.push_back(v);
    }

    int len=h.size();
    for(int i=(int)p.size()-2;i>=0;i--) // 从右往左建上半边
    {
        while((int)h.size()>len&&cro(h[h.size()-2],h.back(),p[i])<=0)h.pop_back();
        h.push_back(p[i]);
    }
    h.pop_back(); // 起点在末尾重复了一次
    return h;
}

int diameter(const vector<po>&h) // 旋转卡壳，求凸包上最远点对的距离平方
{
    int n=h.size();
    if(n<=1)return 0;
    if(n==2)return dis(h[0],h[1]); // 全部共线时，凸包只剩两个端点

    int ans=0,k=1;
    for(int i=0;i<n;i++)
    {
        int j=(i+1)%n; // 当前凸包边为i→j
        while(cro(h[i],h[j],h[(k+1)%n])>cro(h[i],h[j],h[k]))
            k=(k+1)%n; // 底边固定，面积越大说明离这条边越远

        ans=max({ans,dis(h[i],h[k]),dis(h[j],h[k])}); // 对面的点和两个边端点都试一次
        int t=(k+1)%n;
        if(cro(h[i],h[j],h[t])==cro(h[i],h[j],h[k]))
            ans=max({ans,dis(h[i],h[t]),dis(h[j],h[t])}); // 对边平行时，两个对面点都要试
    }
    return ans;
}

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n;
    cin>>n; // 坐标绝对值不超过1e9
    vector<po>p(n);
    for(auto &v:p)cin>>v.x>>v.y;
    cout<<diameter(hull(p))<<'\n';
    return 0;
}
