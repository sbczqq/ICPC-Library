#include<bits/stdc++.h>
using namespace std;

vector<vector<int>>g,rg; // 原图、反图：边u→v表示u成立就必须让v成立

vector<int> scc(int tot)
{
    vector<int>id(tot+1),cur(tot+1),order,stk;
    vector<char>vis(tot+1);

    for(int s=1;s<=tot;s++) // 第一遍：在原图记录每个点搜完的顺序
    {
        if(vis[s])continue;
        vis[s]=1;
        stk.push_back(s);
        while(!stk.empty())
        {
            int u=stk.back();
            if(cur[u]==(int)g[u].size()) // 出边全部检查完，才记录u
            {
                order.push_back(u);
                stk.pop_back();
                continue;
            }

            int v=g[u][cur[u]++]; // cur[u]记录下一条要检查的出边
            if(vis[v])continue;
            vis[v]=1;
            stk.push_back(v);
        }
    }

    reverse(order.begin(),order.end());
    int cnt=0;
    for(int s:order) // 第二遍：按反过来的顺序，在反图上一组一组找
    {
        if(id[s])continue;
        id[s]=++cnt;
        stk.push_back(s);
        while(!stk.empty())
        {
            int u=stk.back();
            stk.pop_back();
            for(int v:rg[u])
            {
                if(id[v])continue;
                id[v]=cnt; // 这次找到的点都与s属于同一组
                stk.push_back(v);
            }
        }
    }
    return id;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n,m;
    cin>>n>>m; // n个变量，每个只能取0或1；m条条件必须全部满足
    g.resize(2*n+1);
    rg.resize(2*n+1);

    for(int i=1;i<=m;i++)
    {
        int x,a,y,b;
        cin>>x>>a>>y>>b; // 要求“x取a”或“y取b”，至少满足一项

        int x_yes=x+a*n; // 表示“x取a”的点；变量x的0、1分别用x、x+n表示
        int x_no=x+(1-a)*n; // 表示“x不取a”的点
        int y_yes=y+b*n;
        int y_no=y+(1-b)*n;

        g[x_no].push_back(y_yes); // x不取a，就必须让y取b
        g[y_no].push_back(x_yes); // y不取b，就必须让x取a
        rg[y_yes].push_back(x_no); // 同两条边，反向存给第二遍DFS
        rg[x_yes].push_back(y_no);
    }

    vector<int>id=scc(2*n);
    for(int x=1;x<=n;x++)
    {
        if(id[x]==id[x+n]) // x取0和取1互相推出，没法只满足其中一个
        {
            cout<<"IMPOSSIBLE\n";
            return 0;
        }
    }

    cout<<"POSSIBLE\n";
    for(int x=1;x<=n;x++)
    {
        int ans=id[x+n]>id[x]; // 这版分量从前往后编号，选编号较大的取值
        cout<<ans<<" \n"[x==n];
    }
    return 0;
}
