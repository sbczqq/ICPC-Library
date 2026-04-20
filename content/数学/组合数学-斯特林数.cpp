#include <iostream>
using namespace std;

#define int long long
using PII = pair<int, int>;

const int MOD = 1e9 + 7;
const int MAXN = 2e5 + 5; // 用于阶乘和单项容斥
const int MAX_DP = 5005;  // 用于 O(N^2) 预处理的二维表大小

int fact[MAXN], invFact[MAXN];
int S1[MAX_DP][MAX_DP]; // 第一类斯特林数二维表
int S2[MAX_DP][MAX_DP]; // 第二类斯特林数二维表

int qpow(int a, int b){
    int res = 1;
    while(b){
        if(b & 1)res = res * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }
    return res;
}

void init(){
    // 1. 初始化阶乘与逆元
    fact[0] = 1; invFact[0] = 1;
    for(int i = 1; i < MAXN; i++) fact[i] = fact[i - 1] * i % MOD;
    invFact[MAXN - 1] = qpow(fact[MAXN - 1], MOD - 2);
    for(int i = MAXN - 2; i >= 1; i--) invFact[i] = invFact[i + 1] * (i + 1) % MOD;

    // 2. 初始化斯特林数二维 DP 表
    S1[0][0] = 1;
    S2[0][0] = 1;
    for(int i = 1; i < MAX_DP; i++){
        for(int j = 1; j <= i; j++){
            // 第一类：新开一桌 + 坐前面(i-1)人中任意一人的左边
            S1[i][j] = (S1[i - 1][j - 1] + (i - 1) * S1[i - 1][j] % MOD) % MOD;
            // 第二类：新开一间房 + 挤进前面已有的 j 间房
            S2[i][j] = (S2[i - 1][j - 1] + j * S2[i - 1][j] % MOD) % MOD;
        }
    }
}

int C(int n, int m){
    if(m < 0 || m > n) return 0;
    return fact[n] * invFact[m] % MOD * invFact[n - m] % MOD;
}

// O(k log n) 容斥原理求单个第二类斯特林数 S(n, k)
// 允许 n 极大，k 需要 <= MAXN
int gets2(int n, int k){
    if(k < 0 || k > n) return 0;
    if(n == 0 && k == 0) return 1;
    
    int res = 0;
    for(int i = 0; i <= k; i++){
        int term = C(k, i) * qpow(i, n) % MOD;
        if((k - i) & 1){ // 奇数减
            res = (res - term + MOD) % MOD;
        }else{           // 偶数加
            res = (res + term) % MOD;
        }
    }
    return res * invFact[k] % MOD;
}

void solve(){
    int n, k;
    cin >> n >> k;
    // n, k 较小时可以直接查表 O(1)
    // cout << S2[n][k] << "\n";
    // n 极大时使用单次计算 O(k log n)
    cout << gets2(n, k) << "\n";
}