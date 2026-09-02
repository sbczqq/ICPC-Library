#include<bits/stdc++.h>
using namespace std;

#define int long long
#define endl '\n'

// 最大数据范围，根据题目自行调整，注意不要开太大爆内存
const int N = 5e4 + 10; 
int pri[N], cnt;
bool st[N];
int mu[N], sum_mu[N];

// 1. 预处理：线性筛求莫比乌斯函数及其前缀和 O(N)
void get_mu(int n)
{
    mu[1] = 1;
    for(int i = 2; i <= n; i++)
    {
        if(!st[i])
        {
            pri[cnt++] = i;
            mu[i] = -1; 
        }
        for(int j = 0; pri[j] <= n / i; j++)
        {
            st[pri[j] * i] = 1;
            if(i % pri[j] == 0)
            {
                mu[pri[j] * i] = 0; 
                break;
            }
            mu[pri[j] * i] = -mu[i];
        }
    }
    // 预处理前缀和，配合整除分块 $O(1)$ 求区间和
    for(int i = 1; i <= n; i++) sum_mu[i] = sum_mu[i - 1] + mu[i];
}

// 2. 查询：整除分块计算单次答案 O(sqrt(N))
int query(int n, int m, int k)
{
    if (k == 0) return 0; // 防御性判断
    n /= k; 
    m /= k;
    
    int ans = 0;
    int limit = min(n, m);
    
    // 整除分块核心逻辑：l 为块左端点，r 为块右端点
    for (int l = 1, r; l <= limit; l = r + 1)
    {
        // 计算当前取整结果不变的右边界
        r = min({limit, n / (n / l), m / (m / l)});
        
        // 累加当前块的贡献：块内 mu 的和 * 取整乘积
        ans += (sum_mu[r] - sum_mu[l - 1]) * (n / l) * (m / l);
    }
    return ans;
}

void solve()
{
    int n, m, k;
    // 题目可能不给 k 而是默认求互质 (k=1)，具体视题目输入而定
    cin >> n >> m >> k; 
    cout << query(n, m, k) << endl;
}

signed main()
{
    // 解除流同步，加速 I/O
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);
    
    // 【警告】全局必须只预处理一次！范围传题目可能的最大界限
    get_mu(N - 1); 
    
    int T;
    cin >> T;
    while(T--) solve();
    
    return 0;
}