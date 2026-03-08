#include<bits/stdc++.h>
using namespace std;
const int N = 4;
const int MOD  = 1e9+10;

int a[N][N];
int res[N][N];

void mul(int c[][N],int A[][N],int B[][N])
{
    int tmp[N][N]={0};
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            if(!A[i][j])continue;
            for(int k=0;k<N;k++)
            {
                tmp[i][k]=(tmp[i][k]+A[i][j]*B[j][k])%MOD;
            }
        }
        
    }
    memcpy(c,tmp,sizeof tmp);
}

void qmi(int k)
{
    memset(res,0,sizeof res);
    for(int i=0;i<N;i++)res[i][i]=1;
    while(k)
    {
        if(k&1)mul(res,res,a);
        mul(a,a,a);
        k>>=1;
    }
}
//使用方法
int main()
{
    //1初始化底数，使得A*n=n+1
    a[0][0]=1;a[0][1]=1;
    a[1][0]=1;
    a[1][1]=0;
    qmi(10);
    cout<<res[0][0]<<endl;
    return 0;
}