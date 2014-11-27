#ifndef __TRI_NUM_H__
#define __TRI_NUM_H__

#include "point.h"

//面片索引类
class tri_num{
public:
	tri_num(int pp1,int pp2,int pp3):p1(pp1),p2(pp2),p3(pp3){
		a=0,b=0,c=0,d=0;
	}
	void calc(point& a1,point& a2,point& a3);			//计算ax+by+cz+d=0中的系数
	int p1,p2,p3;										//面片三个顶点的代号
	double a,b,c,d;										//平面表达式中a,b,c,d的值
};

#endif