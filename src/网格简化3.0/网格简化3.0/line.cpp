#include "line.h"
#include <iostream>
using namespace std;

line::line(int a1,int a2):combine(0,0,0){
	if(a1<a2){
		first=a1;
		second=a2;
	}
	else{
		first=a2;
		second=a1;
	}
	left=NULL,up=NULL;
	right=NULL,down=NULL;
	index=-1;
	length=0;
}

line::~line(){}

//空间中的两个点的归并，归并后的点取其中点
double line::calc(point& p1,point& p2,qmatrix& q1,qmatrix& q2){
	qmatrix q=q1+q2;
	combine.x=(p1.x+p2.x)/2,combine.y=(p1.y+p2.y)/2,combine.z=(p1.z+p2.z)/2;
	double xx=combine.x,yy=combine.y,zz=combine.z;
	double result=q.elem[0][0]*xx*xx+q.elem[0][1]*xx*yy+q.elem[0][2]*xx*zz+q.elem[0][3]*xx*1
				 +q.elem[1][0]*yy*xx+q.elem[1][1]*yy*yy+q.elem[1][2]*yy*zz+q.elem[1][3]*yy*1
				 +q.elem[2][0]*zz*xx+q.elem[2][1]*zz*yy+q.elem[2][2]*zz*zz+q.elem[2][3]*zz*1
				 +q.elem[3][0]*1*xx+q.elem[3][1]*1*yy+q.elem[3][2]*1*zz+q.elem[3][3]*1*1;
	return result;
}