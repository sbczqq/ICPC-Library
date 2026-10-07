#include<bits/stdc++.h>
using namespace std;

int id;

void solve()
{
    if(id==0)
    {
        // 编写进程 0 的逻辑
    }
    else
    {
        // 编写其他进程的逻辑；需要区分时可继续判断 id
    }
}

int main(int argc,char *argv[])
{
    // 进程编号由启动程序时传入的第一个命令行参数提供
    id=atoi(argv[1]);
    solve();
    return 0;
}
