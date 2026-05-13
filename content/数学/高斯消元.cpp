#include<bits/stdc++.h>
using namespace std;

#define int long long
#define endl '\n'

const double eps=1e-8;
const int N=105; // 方程组的最大未知数个数

double a[N][N];  // 增广矩阵，1~n列为系数，n+1列为常数项
double ans[N];   // 存储唯一解时的答案

// 传入未知数个数 n
// 返回值: 0 表示唯一解, 1 表示无穷多解, -1 表示无解
int gauss(int n)
{
    int r=1; // r 记录当前正在构造对角线的第几行
    for(int c=1;c<=n;c++) // 枚举每一列
    {
        // 1. 找主元：在当前列中，找绝对值最大的行
        int pivot=r;
        for(int i=r+1;i<=n;i++)
        {
            if(fabs(a[i][c])>fabs(a[pivot][c]))pivot=i;
        }
        
        // 如果主元接近 0，说明这一列对应的变量是自由变量，跳过处理下一列
        if(fabs(a[pivot][c])<eps)continue;
        
        // 2. 将主元所在的行交换到当前处理行 r
        for(int j=c;j<=n+1;j++)swap(a[r][j],a[pivot][j]);
        
        // 3. 将其余行的当前列全部消为 0
        for(int i=1;i<=n;i++)
        {
            if(i==r)continue; // 留着自己不消
            if(fabs(a[i][c])>eps) // 常数优化，不为 0 才消
            {
                double factor=a[i][c]/a[r][c];
                for(int j=c;j<=n+1;j++)
                {
                    a[i][j]-=factor*a[r][j];
                }
            }
        }
        r++; // 当前行处理完毕，准星下移
    }
    
    // 4. 判解的情况
    if(r<=n)
    {
        // 说明出现了全零行，逐个检查常数项
        for(int i=r;i<=n;i++)
        {
            if(fabs(a[i][n+1])>eps)return -1; // 0 = 常数，矛盾，无解
        }
        return 1; // 0 = 0，无穷多解
    }
    
    // 5. 唯一解，得出最终答案
    for(int i=1;i<=n;i++)
    {
        ans[i]=a[i][n+1]/a[i][i];
    }
    return 0;
}

void solve()
{
    /* 实战调用示例：
    int n;
    cin>>n;
    for(int i=1;i<=n;i++)
        for(int j=1;j<=n+1;j++)
            cin>>a[i][j];
            
    int res=gauss(n);
    if(res==-1) cout<<"No Solution"<<endl;
    else if(res==1) cout<<"Infinite Group Solutions"<<endl;
    else 
    {
        for(int i=1;i<=n;i++)
            cout<<fixed<<setprecision(2)<<ans[i]<<endl;
    }
    */
}
/*
### 💡 实战小贴士
如果你遇到的题目要求在**模运算（整数域）**下解线性方程组（比如 `a[i][j]` 都是整数且要求模 $998244353$），只需要把这个模板里的：
1. `double` 改回 `int`。
2. 找主元时去掉 `fabs`。
3. 把除法 `factor = a[i][c] / a[r][c]` 替换成**乘法逆元**：`factor = a[i][c] * qmi(a[r][c], MOD-2) % MOD` 即可完美复用！
*/