#ifndef __POINT_H__
#define __POINT_H__

//点类
class point{
public:
	point(double xx,double yy,double zz):x(xx),y(yy),z(zz){
		del=0;
	}
	double x,y,z;				//三维坐标
	int del;					//是否删除
};

#endif