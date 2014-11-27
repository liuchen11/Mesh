#ifndef __HEAP_NODE_H__
#define __HEAP_NODE_H__

#include "line.h"
#include <iostream>
using namespace std;

//堆中的单元
class heap_node{
public:
	heap_node(double d,line* b=NULL){
		delta=d;
		back=b;
	}
	~heap_node(){back=NULL;}
	double delta;							//delta值，堆按照delta值进行偏序排序
	line* back;								//指向十字链表中的节点
};

#endif