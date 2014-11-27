#include "tri_num.h"
#include "matrix.h"
#include <cmath>
#include <stdio.h>
using namespace std;

//计算平面方程对应的a,b,c,d值,并将a,b,c归一化处理
void tri_num::calc(point& a1,point &a2,point&a3){
	double x1=a1.x,x2=a2.x,x3=a3.x;
	double y1=a1.y,y2=a2.y,y3=a3.y;
	double z1=a1.z,z2=a2.z,z3=a3.z;
	double det=x1*y2*z3+y1*z2*x3+z1*x2*y3-z1*y2*x3-y1*x2*z3-x1*z2*y3;
	double a11=(y2*z3-z2*y3)/det,a12=-(y1*z3-z1*y3)/det,a13=(y1*z2-z1*y2)/det;
	double a21=-(x2*z3-z2*x3)/det,a22=(x1*z3-z1*x3)/det,a23=-(x1*z2-z1*x2)/det;
	double a31=(x2*y3-y2*x3)/det,a32=-(x1*y3-y1*x3)/det,a33=(x1*y2-y1*x2)/det;
	a=-a11-a12-a13,b=-a21-a22-a23,c=-a31-a32-a33,d=1;
	double square=a*a+b*b+c*c;
	double l=sqrt(square);
	a=a/l,b=b/l,c=c/l,d=d/l;
	return;
}