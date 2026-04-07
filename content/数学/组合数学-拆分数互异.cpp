#include <iostream>
#include <cstring>
using namespace std;

#define int long long
using PII = pair<int, int>;

const int MOD = 1000007;
const int MAXN = 50005;

// 使用滚动数组压缩 k 维度，pd[i][j&1] 表示将 i 拆为 j 个互异部分的方案数
int pd[MAXN][2];

void solve(){
    int n;
    if(!(cin >> n))exit(0);

    memset(pd, 0, sizeof(pd));
    pd[0][0] = 1;
    int ans = 0;

    // 互异分拆的极端情况是 1+2+...+k <= n。
    // 当 n=50000 时，k 的最大值约为 315，所以循环到 350 即可。
    for(int j = 1;j < 350;++j){
        // 每次进入新的一层 j，先清空当前层
        for(int i = 0;i <= n;++i)pd[i][j & 1] = 0;

        for(int i = 0;i <= n;++i){
            if(i - j >= 0){
                // pd[i-j][j&1]: 抽底后原本最小的数依然 >=1 (共 j 堆)
                // pd[i-j][(j-1)&1]: 抽底后原本最小的数变成 0 (剩 j-1 堆)
                pd[i][j & 1] = (pd[i - j][j & 1] + pd[i - j][(j - 1) & 1]) % MOD;
            }
        }
        // 将恰好拆成 j 个互异部分的方案数累加到最终答案
        ans = (ans + pd[n][j & 1]) % MOD;
    }
    cout << ans << "\n";
}