#include <iostream>
using namespace std;

#define int long long
using PII = pair<int, int>;

const int MOD = 1e9 + 7;
const int MAXN = 2e5 + 5; // 计算 C_n 需要用到 2n 的阶乘，注意开两倍大小

int fact[MAXN], invFact[MAXN];

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
    fact[0] = 1;invFact[0] = 1;
    for(int i = 1;i < MAXN;i++)fact[i] = fact[i - 1] * i % MOD;
    invFact[MAXN - 1] = qpow(fact[MAXN - 1], MOD - 2);
    for(int i = MAXN - 2;i >= 1;i--)invFact[i] = invFact[i + 1] * (i + 1) % MOD;
}

int C(int n, int m){
    if(m<0 || m>n)return 0;
    return fact[n] * invFact[m] % MOD * invFact[n - m] % MOD;
}

int get_catalan(int n){
    if(n < 0)return 0;
    return C(2 * n, n) * qpow(n + 1, MOD - 2) % MOD;
}
// O(1) 求广义卡特兰数：n 个操作A，m 个操作B，保证任意时刻 A的出现次数 >= B的出现次数
int get_generalized_catalan(int n, int m){
    if(n < m) return 0; // B操作比A操作多，绝对无法满足限制

    // Ans = C(n+m, m) - C(n+m, m-1)
    int total = C(n + m, m);
    int invalid = C(n + m, m - 1);

    // 注意取模时的减法防负数处理
    return (total - invalid + MOD) % MOD;
}
void solve(){
    int n;
    cin >> n;
    cout << get_catalan(n) << "\n";
}

signed main(){
    ios::sync_with_stdio(false);cin.tie(nullptr);
    init();
    int t = 1;
    cin >> t;
    while(t--)solve();
    return 0;
}