const int N = 1e7 + 10;
int pri[N], cnt;
bool st[N];
void getpri(int n)
{
    for(int i = 2;i <= n;i++)
    {
        if(!st[i])pri[cnt++] = i;
        for(int j = 0;pri[j] <= n / i;j++)
        {
            st[pri[j] * i] = 1;
            if(i % pri[j] == 0)break;
        }
    }
}