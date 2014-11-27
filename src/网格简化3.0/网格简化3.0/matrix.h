#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <iostream>
using namespace std;

class matrix{
public:
	matrix(int xx=4,int yy=4);
	matrix(const matrix& copy);
	~matrix();
	matrix& operator=(const matrix& eval);
	matrix& operator+(matrix& plus);
	matrix& operator-(matrix& minus);
	matrix& operator*(double time);
	matrix& operator*(matrix& time);
	matrix& operator/(double div);
	matrix& submatrix(int xx,int yy);		//降一阶子矩阵
	matrix& T();							//转置
	double det();							//方阵行列式的值
	matrix& reverse();						//逆矩阵
	double** elem;
	int x,y;
private:
	void destroy();
};

#endif