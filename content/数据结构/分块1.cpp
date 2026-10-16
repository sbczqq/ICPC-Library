#include <bits/stdc++.h>
using namespace std;

#define int long long
#define endl '\n'

const int N = 1e5 + 10, BSI = 400;

int a[N], sora[N], lazy[BSI];
int L[BSI], R[BSI], pos[N];
int n, q, blosize, blonum;
// 重构函数：散块修改后同步 sora 镜像并排序
void rebuild(int id)
{
    for (int i = L[id]; i <= R[id]; i++)sora[i] = a[i];
    sort(sora + L[id], sora + R[id] + 1);
}

// 操作 1：区间增量
void update(int l, int r, int x)
{
    int bl = pos[l], br = pos[r];
    if (bl == br)
    {
        for (int i = l; i <= r; i++) a[i] += x;
        rebuild(bl);
    }
    else
    {
        // 左散块处理
        for (int i = l; i <= R[bl]; i++) a[i] += x;
        rebuild(bl);
        // 中间整块处理标记
        for (int i = bl + 1; i <= br - 1; i++) lazy[i] += x;
        // 右散块处理
        for (int i = L[br]; i <= r; i++) a[i] += x;
        rebuild(br);
    }
}
// 操作 2：区间小于 x 计数
int query(int l, int r, int x)
{
    int bl = pos[l], br = pos[r];
    int ans = 0;
    if (bl == br)
    {
        for (int i = l; i <= r; i++)
        {
            if (a[i] + lazy[bl] < x) ans++;
        }
    }
    else
    {
        // 左右散块暴力匹配
        for (int i = l; i <= R[bl]; i++) if (a[i] + lazy[bl] < x) ans++;
        for (int i = L[br]; i <= r; i++) if (a[i] + lazy[br] < x) ans++;
        // 中间整块二分查找
        for (int i = bl + 1; i <= br - 1; i++)
        {
            int target = x - lazy[i];
            ans += lower_bound(sora + L[i], sora + R[i] + 1, target) - (sora + L[i]);
        }
    }
    return ans;
}

void solve()
{
    if (!(cin >> n >> q)) return;
    blosize = sqrt(n);
    blonum = (n - 1) / blosize + 1;
    for (int i = 1; i <= n; i++)
    {
        cin >> a[i];
        pos[i] = (i - 1) / blosize + 1;
        sora[i] = a[i];
    }
    // 初始化边界并对块内进行首次预排序
    for (int i = 1; i <= blonum; i++)
    {
        L[i] = (i - 1) * blosize + 1;
        R[i] = min(i * blosize, n);
        sort(sora + L[i], sora + R[i] + 1);
    }

    while (q--)
    {
        int op, l, r, x;
        cin >> op >> l >> r >> x;
        if (op == 1) update(l, r, x);
        else cout << query(l, r, x) << endl;
    }
}