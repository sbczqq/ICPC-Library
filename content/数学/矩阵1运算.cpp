
#include<bits/stdc++.h>
using namespace std;

#define int long long
#define endl '\n'

const int MOD=1e9+7;
const int SZ=105; // 矩阵最大边长

int qmi(int a,int k)
{
    int res=1;
    while(k)
    {
        if(k&1)res=res*a%MOD;
        a=a*a%MOD;
        k>>=1;
    }
    return res;
}

struct Mat
{
    int r,c; // r:行数, c:列数
    int m[SZ][SZ];
    
    Mat(int r=0,int c=0):r(r),c(c)
    {
        memset(m,0,sizeof(m));
    }
    void eye() // 生成单位矩阵 (必须在方阵下调用)
    {
        for(int i=1;i<=r;i++)m[i][i]=1;
    }
};

// 1. 矩阵加法 (要求 A.r == B.r && A.c == B.c)
Mat add(Mat a,Mat b)
{
    Mat res(a.r,a.c);
    for(int i=1;i<=a.r;i++)
        for(int j=1;j<=a.c;j++)
            res.m[i][j]=(a.m[i][j]+b.m[i][j])%MOD;
    return res;
}

// 2. 矩阵减法 (要求 A.r == B.r && A.c == B.c)
Mat sub(Mat a,Mat b)
{
    Mat res(a.r,a.c);
    for(int i=1;i<=a.r;i++)
        for(int j=1;j<=a.c;j++)
            res.m[i][j]=(a.m[i][j]-b.m[i][j]+MOD)%MOD;
    return res;
}

// 3. 矩阵乘法 (要求 A.c == B.r，结果维度为 A.r * B.c)
Mat mul(Mat a,Mat b)
{
    Mat res(a.r,b.c);
    for(int i=1;i<=a.r;i++)
        for(int k=1;k<=a.c;k++)
        {
            if(!a.m[i][k])continue; // 常数级剪枝
            for(int j=1;j<=b.c;j++)
                res.m[i][j]=(res.m[i][j]+a.m[i][k]*b.m[k][j])%MOD;
        }
    return res;
}

// 4. 矩阵求逆 (要求 A 必须是方阵 A.r == A.c)
Mat inv(Mat a)
{
    int n=a.r;
    Mat res(n,n);
    res.eye(); 
    
    for(int i=1;i<=n;i++)
    {
        int pivot=i;
        for(int j=i;j<=n;j++)if(a.m[j][i]){pivot=j;break;}
        
        if(!a.m[pivot][i])return Mat(0,0); // 不可逆返回空矩阵
        
        if(pivot!=i)
        {
            for(int j=1;j<=n;j++)
            {
                swap(a.m[i][j],a.m[pivot][j]);
                swap(res.m[i][j],res.m[pivot][j]);
            }
        }
        
        int iv=qmi(a.m[i][i],MOD-2);
        for(int j=1;j<=n;j++)
        {
            a.m[i][j]=a.m[i][j]*iv%MOD;
            res.m[i][j]=res.m[i][j]*iv%MOD;
        }
        
        for(int j=1;j<=n;j++)
        {
            if(i==j||!a.m[j][i])continue;
            int factor=a.m[j][i];
            for(int k=1;k<=n;k++)
            {
                a.m[j][k]=(a.m[j][k]-factor*a.m[i][k])%MOD;
                if(a.m[j][k]<0)a.m[j][k]+=MOD;
                
                res.m[j][k]=(res.m[j][k]-factor*res.m[i][k])%MOD;
                if(res.m[j][k]<0)res.m[j][k]+=MOD;
            }
        }
    }
    return res;
}

/* 💡 使用提示：
计算 (A + B) * C：
Mat ans = mul(add(A, B), C);

计算 A / B：
Mat ans = mul(A, inv(B));
*/