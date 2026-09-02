#include<bits/stdc++.h>
using namespace std;

#define int long long
#define endl '\n'

const double eps=1e-9;
const double PI=acos(-1.0);

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

struct Circle
{
    Point c;
    double r;
    Circle(){}
    Circle(Point c,double r):c(c),r(r){}
};

// --- 纯函数式向量运算 ---
Vector add(Vector A,Vector B) { return Vector(A.x+B.x,A.y+B.y); }
Vector sub(Vector A,Vector B) { return Vector(A.x-B.x,A.y-B.y); }
Vector mul(Vector A,double p) { return Vector(A.x*p,A.y*p); }

double dot(Vector A,Vector B) { return A.x*B.x+A.y*B.y; }
double cross(Vector A,Vector B) { return A.x*B.y-A.y*B.x; }

// 向量长度与极角
double length(Vector A) { return sqrt(dot(A,A)); }
double angle(Vector A) { return atan2(A.y,A.x); } // 返回极角 [-PI, PI]

// 极坐标转直角坐标 (已知起点O，极长r，极角a)
Point point_from_polar(Point O,double r,double a)
{
    return Point(O.x+r*cos(a),O.y+r*sin(a));
}

// ================= 1. 直线与圆的交点 =================
// 直线由点 A 和 B 确定，返回交点个数，交点存入 p1, p2
int line_circle_intersection(Point A,Point B,Circle C,Point &p1,Point &p2)
{
    Vector v=sub(B,A);
    Vector CA=sub(A,C.c);
    
    // a, b, c 对应二次方程系数，也可理解为投影计算
    double a=dot(v,v);
    double b=2*dot(v,CA);
    double c=dot(CA,CA)-C.r*C.r;
    double delta=b*b-4*a*c;
    
    if(dcmp(delta)<0)return 0; // 相离
    if(dcmp(delta)==0) // 相切
    {
        double t=-b/(2*a);
        p1=p2=add(A,mul(v,t));
        return 1;
    }
    // 相交
    double t1=(-b-sqrt(delta))/(2*a);
    double t2=(-b+sqrt(delta))/(2*a);
    p1=add(A,mul(v,t1));
    p2=add(A,mul(v,t2));
    return 2;
}

// ================= 2. 圆与圆的交点 =================
// 返回交点个数，交点存入 p1, p2
int circle_circle_intersection(Circle C1,Circle C2,Point &p1,Point &p2)
{
    double d=length(sub(C1.c,C2.c));
    if(dcmp(d)==0) 
    {
        if(dcmp(C1.r-C2.r)==0)return -1; // 完全重合，无数个交点
        return 0; // 同心圆但不重合，无交点
    }
    
    if(dcmp(C1.r+C2.r-d)<0)return 0; // 外离
    if(dcmp(fabs(C1.r-C2.r)-d)>0)return 0; // 内含
    
    // 核心：利用余弦定理求圆心角
    double base_angle=angle(sub(C2.c,C1.c)); // 连心线的极角
    // a 为 C1.c 到交点的连线 与 连心线 之间的夹角
    double a=acos((C1.r*C1.r+d*d-C2.r*C2.r)/(2*C1.r*d));
    
    p1=point_from_polar(C1.c,C1.r,base_angle+a);
    p2=point_from_polar(C1.c,C1.r,base_angle-a);
    
    if(dcmp(C1.r+C2.r-d)==0||dcmp(fabs(C1.r-C2.r)-d)==0)return 1; // 恰好相切
    return 2; // 相交
}

// ================= 3. 点到圆的切点 =================
// 从点 P 向圆 C 作切线，返回切点个数，切点存入 p1, p2
int point_circle_tangents(Point P,Circle C,Point &p1,Point &p2)
{
    Vector u=sub(P,C.c);
    double d=length(u);
    
    if(dcmp(d-C.r)<0)return 0; // 点在圆内，无切线
    if(dcmp(d-C.r)==0) // 点在圆上，只有1个切点就是它自己
    {
        p1=p2=P;
        return 1;
    }
    
    // 核心：构建以 C.c 为顶点，C.c->P 为斜边的直角三角形
    double base_angle=angle(u);
    double a=acos(C.r/d); // 圆心角的偏移量
    
    p1=point_from_polar(C.c,C.r,base_angle+a);
    p2=point_from_polar(C.c,C.r,base_angle-a);
    return 2;
}

void solve()
{
    /* 实战调用示例
    Circle C1(Point(0,0), 5);
    Circle C2(Point(8,0), 5);
    Point p1, p2;
    
    int cnt = circle_circle_intersection(C1, C2, p1, p2);
    if(cnt == 2)
    {
        cout << fixed << setprecision(6) << "圆交点1: " << p1.x << " " << p1.y << endl;
        cout << fixed << setprecision(6) << "圆交点2: " << p2.x << " " << p2.y << endl;
    }
    
    Point P(10, 0);
    cnt = point_circle_tangents(P, C1, p1, p2);
    if(cnt == 2)
    {
        cout << "切点1: " << p1.x << " " << p1.y << endl;
        cout << "切点2: " << p2.x << " " << p2.y << endl;
    }
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