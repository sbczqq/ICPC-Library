#include<bits/stdc++.h>
using namespace std;

#define int long long
#define endl '\n'

const double eps=1e-9;

// 浮点数比较助手 (消除精度误差)
int dcmp(double x)
{
    if(fabs(x)<eps)return 0;
    return x<0?-1:1;
}

struct Point
{
    double x,y;
    Point(double x=0,double y=0):x(x),y(y){}
};
typedef Point Vector;

// --- 纯函数式向量运算 (拒接重载运算符) ---
Vector add(Vector A,Vector B) { return Vector(A.x+B.x,A.y+B.y); }
Vector sub(Vector A,Vector B) { return Vector(A.x-B.x,A.y-B.y); }
Vector mul(Vector A,double p) { return Vector(A.x*p,A.y*p); }

// 叉积 (Cross Product): A.x*B.y - A.y*B.x
double cross(Vector A,Vector B) { return A.x*B.y-A.y*B.x; }
// 点积 (Dot Product): A.x*B.x + A.y*B.y
double dot(Vector A,Vector B) { return A.x*B.x+A.y*B.y; }

// 判定点 P 是否在线段 AB 上 (包含端点)
bool on_segment(Point P,Point A,Point B)
{
    // 结论：P 在 AB 上 <=> 向量PA与PB共线(叉积为0) 且 P 在 AB 之间(点积<=0)
    Vector PA=sub(A,P);
    Vector PB=sub(B,P);
    return dcmp(cross(PA,PB))==0&&dcmp(dot(PA,PB))<=0;
}

// ================= 核心：万能交点函数全家桶 =================
// type1 和 type2 的含义：0 代表直线, 1 代表射线, 2 代表线段
// P1, P2 定义第一条线（若为射线，P1为起点，方向为 P1->P2）
// P3, P4 定义第二条线（若为射线，P3为起点，方向为 P3->P4）
// res 存储求出的交点坐标
// 返回值：-1 代表平行/共线（无交点或无数交点），1 代表存在唯一交点
int get_intersection(Point P1,Point P2,int type1,Point P3,Point P4,int type2,Point &res)
{
    Vector v1=sub(P2,P1); // 线1的方向向量
    Vector v2=sub(P4,P3); // 线2的方向向量
    
    double denom=cross(v1,v2);
    
    // 1. 分母为 0，说明两方向向量平行或共线
    if(dcmp(denom)==0)return -1; 
    
    // 2. 利用叉积公式计算参数 t1 和 t2
    // 线1: P(t1) = P1 + t1 * v1
    // 线2: P(t2) = P3 + t2 * v2
    Vector P3_P1=sub(P3,P1);
    double t1=cross(P3_P1,v2)/denom;
    double t2=cross(P3_P1,v1)/denom;
    
    // 3. 根据类型校验 t1 的合法性
    if(type1==1&&dcmp(t1)<0)return -1; // 射线，参数 t1 必须 >= 0
    if(type1==2&&(dcmp(t1)<0||dcmp(t1-1)>0))return -1; // 线段，参数 t1 必须在 [0,1]
    
    // 4. 根据类型校验 t2 的合法性
    if(type2==1&&dcmp(t2)<0)return -1; // 射线，参数 t2 必须 >= 0
    if(type2==2&&(dcmp(t2)<0||dcmp(t2-1)>0))return -1; // 线段，参数 t2 必须在 [0,1]
    
    // 5. 校验通过，计算具体交点坐标并赋给 res
    res=add(P1,mul(v1,t1));
    return 1;
}

void solve()
{
    /* 实战调用示例
    Point A(0,0),B(2,2); // 线1
    Point C(0,2),D(2,0); // 线2
    Point p;
    
    // 示例 A: 求线段 AB 和线段 CD 的交点 (传参都为 2)
    if(get_intersection(A,B,2,C,D,2,p)==1)
        cout<<fixed<<setprecision(6)<<"线段交点: "<<p.x<<" "<<p.y<<endl;
    else
        cout<<"线段无交点"<<endl;
        
    // 示例 B: 求从 A 出发经过 B 的【射线】，与从 C 出发经过 D 的【射线】的交点 (传参都为 1)
    if(get_intersection(A,B,1,C,D,1,p)==1)
        cout<<"射线交点: "<<p.x<<" "<<p.y<<endl;
        
    // 示例 C: 求直线 AB 和线段 CD 的交点 (传参分别是 0 和 2)
    if(get_intersection(A,B,0,C,D,2,p)==1)
        cout<<"直线与线段交点: "<<p.x<<" "<<p.y<<endl;
    */
}

signed main()
{
    cin.tie(0);
    ios::sync_with_stdio(0);
    int T=1;
    // cin>>T;
    while(T--) solve();
    return 0;
}
