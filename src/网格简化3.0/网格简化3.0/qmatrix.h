#ifndef __QMATRIX_H__
#define __QMATRIX_H__

//ËÄ½×¾ØÕóÀà
class qmatrix{
public:
	qmatrix();
	~qmatrix();
	qmatrix& operator=(const qmatrix& eval);
	qmatrix operator+(const qmatrix& plus);
	qmatrix operator-(const qmatrix& minus);
	qmatrix operator*(double times);
	qmatrix operator/(double div);
	double elem[4][4];
};

#endif