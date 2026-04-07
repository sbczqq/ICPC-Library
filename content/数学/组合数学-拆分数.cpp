#include <iostream>
#include <cstring>
using namespace std;

#define int long long
using PII = pair<int, int>;

const int MOD = 1000007;
const int MAXN = 10005;
const int MAXK = 1005;

int p[MAXN][MAXK]; // p[i][j] 表示将数字 i 拆成恰好 j 个部分的方案数

// 全局预处理，时间复杂度 O(N * K)
void init(){
    memset(p, 0, sizeof(p));
    p[0][0] = 1;
    for(int i = 1;i < MAXN;++i){
        for(int j = 1;j < MAXK;++j){
            if(i - j >= 0){
                // p[i-j][j]: 所有部分 >= 2
                // p[i-1][j-1]: 至少有一个部分是 1
                p[i][j] = (p[i - j][j] + p[i - 1][j - 1]) % MOD;
            }
        }
    }
}

void solve(){
    int n, k;
    // 多组输入处理
    if(cin >> n >> k){
        cout << p[n][k] << "\n";
    }
    else{
        exit(0);
    }
}