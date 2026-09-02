#include<bits/stdc++.h>
using namespace std;

#define int long long
#define endl '\n'

const int MAXN=3e6+10; 

// --- 御用三模数与原根 ---
const int P[3] = {998244353, 1004535809, 469762049};
const int G = 3;

// --- 预处理 CRT 需要的逆元常数 ---
// inv1: P[0] 在模 P[1] 下的逆元
// inv12: (P[0] * P[1]) 在模 P[2] 下的逆元
int inv1, inv12;

int a_ori[MAXN], b_ori[MAXN]; // 原始数组
int a[3][MAXN], b[3][MAXN];   // 3 个模数下的分身数组
int rev[MAXN];
int tot, bit;

// --- 带有模数的快速幂 ---
int qmi(int a, int k, int mod)
{
    int res=1;
    a%=mod;
    while(k)
    {
        if(k&1) res=res*a%mod;
        a=a*a%mod;
        k>>=1;
    }
    return res;
}

// --- 改造版 NTT (增加 mod 参数) ---
void ntt(int *a, int type, int mod)
{
    for(int i=0;i<tot;i++) 
        if(i<rev[i]) swap(a[i],a[rev[i]]);
        
    for(int mid=1;mid<tot;mid<<=1)
    {
        // 质数原根是 G，逆元是 qmi(G, mod-2, mod)
        int w1=qmi(type==1 ? G : qmi(G,mod-2,mod), (mod-1)/(mid*2), mod);
        
        for(int i=0;i<tot;i+=mid*2)
        {
            int wk=1;
            for(int j=0;j<mid;j++,wk=wk*w1%mod)
            {
                int x=a[i+j];
                int y=wk*a[i+j+mid]%mod;
                a[i+j]=(x+y)%mod;
                a[i+j+mid]=(x-y+mod)%mod;
            }
        }
    }
}

// --- 核心：CRT 合并 3 个模数下的结果 ---
int CRT(int a1, int a2, int a3, int target_mod)
{
    // 1. 合并前两个方程 (模 P0 和 P1)
    // x12 是在模 P0*P1 下的真实答案，规模在 10^18 内，long long 不会爆
    int x = (a2 - a1) % P[1];
    if(x < 0) x += P[1];
    int x12 = (a1 + x * inv1 % P[1] * P[0]); 
    
    // 2. 合并第三个方程 (将 x12 与 P2 合并)
    int k = (a3 - x12 % P[2]) % P[2];
    if(k < 0) k += P[2];
    k = k * inv12 % P[2];
    
    // 3. 得到绝对真实的原始答案！(极大概率超过 1e18，必须上 __int128)
    __int128 P0_mul_P1 = (__int128)P[0] * P[1];
    __int128 ans = (__int128)k * P0_mul_P1 + x12;
    
    // 4. 对题目要求的刁钻模数取模
    return ans % target_mod;
}

void solve()
{
    int n, m, target_mod;
    // n 和 m 分别是最高次幂，target_mod 是题目要求的任意模数
    cin>>n>>m>>target_mod; 
    
    for(int i=0;i<=n;i++) cin>>a_ori[i];
    for(int i=0;i<=m;i++) cin>>b_ori[i];
    
    // 【第一步】：拷贝出 3 份分身
    for(int i=0;i<=n;i++) a[0][i] = a[1][i] = a[2][i] = a_ori[i];
    for(int i=0;i<=m;i++) b[0][i] = b[1][i] = b[2][i] = b_ori[i];
    
    // 【第二步】：补齐 2 的次幂长度，预处理位逆序数组 rev
    tot=1; bit=0;
    while(tot<=n+m) { tot<<=1; bit++; }
    for(int i=0;i<tot;i++) rev[i]=(rev[i>>1]>>1)|((i&1)<<(bit-1));
    
    // 【第三步】：预处理 CRT 的常数逆元
    inv1 = qmi(P[0], P[1]-2, P[1]);
    inv12 = qmi(P[0] * P[1] % P[2], P[2]-2, P[2]);
    
    // 【第四步】：平行时空 3 线并发 NTT
    for(int k=0; k<3; k++)
    {
        ntt(a[k], 1, P[k]); // 正变换
        ntt(b[k], 1, P[k]);
        
        for(int i=0; i<tot; i++) a[k][i] = a[k][i] * b[k][i] % P[k]; // O(N) 乘法
        
        ntt(a[k], -1, P[k]); // 逆变换
        
        // 逆变换除以 tot (乘以 tot 在当前模数下的逆元)
        int inv_tot = qmi(tot, P[k]-2, P[k]);
        for(int i=0; i<=n+m; i++) a[k][i] = a[k][i] * inv_tot % P[k];
    }
    
    // 【第五步】：万剑归宗，利用 CRT 融合后直接输出真实值对 target_mod 取模的结果
    for(int i=0; i<=n+m; i++)
    {
        int final_ans = CRT(a[0][i], a[1][i], a[2][i], target_mod);
        cout << final_ans << " ";
    }
    cout<<endl;
}