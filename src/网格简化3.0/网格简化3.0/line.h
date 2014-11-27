#ifndef __LINE_H__
#define __LINE_H__

#include "point.h"
#include "qmatrix.h"

class line{
public:
	line(int a1=-1,int a2=-1);											//构造函数
	~line();															//析构函数
	double calc(point& p1,point& p2,qmatrix& q1,qmatrix& q2);			//计算两点的delta值以及最佳合并顶点
	int first,second,index;												//两点的坐标,在堆中的索引值
	double length;														//长度
	point combine;														//合并后的点
	line *left,*right,*up,*down;										//十字链表中的上下左右邻居
};

#endif