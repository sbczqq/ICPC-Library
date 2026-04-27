#include<bits/stdc++.h>
using namespace std;
#define int long long
using PII = pair<int,int>;

const int MAXN = 5e5 + 5; // 节点总数极限，通常为：字符串个数 * 字符串最大长度

struct Trie {
    int nex[MAXN][26]; // nex[p][c]：节点 p 的字符 c 连向的子节点编号
    int st[MAXN];      // 状态标记：0=非结尾/不存在，1=存在且未访问，>1=已访问
    int id = 1;        // 节点分配器，0为根节点，从1开始分配新节点

    // 插入字符串
    void insert(const string& s)
    {
        int p = 0;
        for(char ch : s)
        {
            int c = ch - 'a';
            if(!nex[p][c]) nex[p][c] = id++;
            p = nex[p][c];
        }
        st[p] = 1; // 标记该节点为某个字符串的结尾
    }

    // 查询字符串状态
    string find(const string& s)
    {
        int p = 0;
        for(char ch : s)
        {
            int c = ch - 'a';
            if(!nex[p][c]) return "WRONG"; // 连前缀都匹配不上
            p = nex[p][c];
        }
        
        // 匹配到了目标节点，根据状态标记判断
        if(st[p] == 0) return "WRONG";     // 是别人的前缀，但自身不是完整单词
        else if(st[p] == 1) 
        { 
            st[p]++; 
            return "OK";                   // 首次合法访问
        }
        else return "REPEAT";              // 合法但重复访问
    }
};

/* 核心调用与拓展逻辑：
 * 1. 实例化与使用：
 * Trie tr;
 * tr.insert(s);
 * cout << tr.find(s) << '\n';
 * * 2. 多组数据清空防 TLE 优化：
 * 绝不能 memset 整个 nex 数组（极大可能会超时）。
 * 只需按分配出的 id 数量进行精准清空：
 * void clear() {
 * for(int i = 0; i < id; i++) {
 * for(int c = 0; c < 26; c++) nex[i][c] = 0;
 * st[i] = 0;
 * }
 * id = 1;
 * }
 */