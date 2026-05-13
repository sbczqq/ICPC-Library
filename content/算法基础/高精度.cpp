#include<bits/stdc++.h>
using namespace std;

#define int long long
#define endl '\n'

typedef vector<int> VI;

// 判断 A >= B
bool cmp(VI &A, VI &B) {
    if (A.size() != B.size()) return A.size() > B.size();
    for (int i = A.size() - 1; i >= 0; i -- )
        if (A[i] != B[i]) return A[i] > B[i];
    return true;
}

// 1. 高精度加法：C = A + B
VI add(VI &A, VI &B) {
    if (A.size() < B.size()) return add(B, A);
    VI C;
    int t = 0;
    for (int i = 0; i < A.size() || t; i ++ ) {
        if (i < A.size()) t += A[i];
        if (i < B.size()) t += B[i];
        C.push_back(t % 10);
        t /= 10;
    }
    return C;
}

// 2. 高精度减法：C = A - B (需保证 A >= B)
VI sub(VI &A, VI &B) {
    VI C;
    for (int i = 0, t = 0; i < A.size(); i ++ ) {
        t = A[i] - t;
        if (i < B.size()) t -= B[i];
        C.push_back((t + 10) % 10);
        if (t < 0) t = 1;
        else t = 0;
    }
    while (C.size() > 1 && C.back() == 0) C.pop_back(); // 去除前导0
    return C;
}

// 3. 高精度乘低精度：C = A * b
VI mul(VI &A, int b) {
    VI C;
    int t = 0;
    for (int i = 0; i < A.size() || t; i ++ ) {
        if (i < A.size()) t += A[i] * b;
        C.push_back(t % 10);
        t /= 10;
    }
    while (C.size() > 1 && C.back() == 0) C.pop_back();
    return C;
}

// 4. 高精度除以低精度：C = A / b, r 是余数
VI div(VI &A, int b, int &r) {
    VI C;
    r = 0;
    for (int i = A.size() - 1; i >= 0; i -- ) {
        r = r * 10 + A[i];
        C.push_back(r / b);
        r %= b;
    }
    reverse(C.begin(), C.end()); // 除法是从高位开始算的，存入后需翻转回逆序
    while (C.size() > 1 && C.back() == 0) C.pop_back();
    return C;
}

// 5. 高精度乘高精度：C = A * B (模拟竖式)
VI mul_huge(VI &A, VI &B) {
    VI C(A.size() + B.size() + 7, 0);
    for (int i = 0; i < A.size(); i++) {
        for (int j = 0; j < B.size(); j++) {
            C[i + j] += A[i] * B[j];
        }
    }
    int t = 0;
    for (int i = 0; i < C.size(); i++) {
        t += C[i];
        C[i] = t % 10;
        t /= 10;
    }
    while (C.size() > 1 && C.back() == 0) C.pop_back();
    return C;
}

void solve() {
    string s1, s2;
    cin >> s1 >> s2;
    VI A, B;
    for (int i = s1.size() - 1; i >= 0; i -- ) A.push_back(s1[i] - '0');
    for (int i = s2.size() - 1; i >= 0; i -- ) B.push_back(s2[i] - '0');

    // 加法示例
    VI C = add(A, B);
    for (int i = C.size() - 1; i >= 0; i -- ) cout << C[i];
    cout << endl;
}

signed main() {
    cin.tie(0); ios::sync_with_stdio(0);
    solve();
    return 0;
}