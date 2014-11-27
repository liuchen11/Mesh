#ifndef __MESH_H__
#define __MESH_H__

#include "point.h"
#include "line.h"
#include "tri_num.h"
#include "heap_node.h"
#include "qmatrix.h"
#include <vector>
#include <string>
#include <iostream>
using namespace std;

class mesh{
public:
	mesh(double r,double max);			//构造函数,r为压缩比,max为最长距离
	~mesh();							//析构函数
	int input(string file);				//读入obj文件
	void output(string file);			//输出信息至obj文件
	void initialize();					//初始化,建立边的十字链表，顶点到面的倒排列表索引
	void heap_adjust(int num);			//对堆进行局部调整
	void find();						//找出小于maxdis的边加入堆中
	int del_a_point();					//合并一组顶点
	void simplify();					//按照一定比例进行网格简化
	vector<point> pt;					//点的集合
	vector<tri_num> tri;				//面索引的集合
	line **lx,**ly;						//十字链表的坐标轴
	vector<int>* t_table;				//点对面的倒排列表
	qmatrix *qpt,*qface;				//面和点对应的矩阵
	vector<heap_node> heap;				//堆
	int m_point,m_tri;					//总点数和总边数
	int del_point,del_tri;				//删去的总点数和总边数
	double rate,maxdis;					//简化率和最大边长度
	//friend void draw();					//绘制
	void show();						//显示
};



#endif