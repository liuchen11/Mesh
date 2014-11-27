#include "qmatrix.h"

qmatrix::qmatrix(){
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			elem[i][j]=0;
}

qmatrix::~qmatrix(){}

qmatrix& qmatrix::operator=(const qmatrix& eval){
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			elem[i][j]=eval.elem[i][j];
	return *this;
}

qmatrix qmatrix::operator+(const qmatrix& plus){
	qmatrix tmp;
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			tmp.elem[i][j]=elem[i][j]+plus.elem[i][j];
	return tmp;
}


qmatrix qmatrix::operator-(const qmatrix& minus){
	qmatrix tmp;
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			tmp.elem[i][j]=elem[i][j]-minus.elem[i][j];
	return tmp;
}

qmatrix qmatrix::operator*(double times){
	qmatrix tmp;
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			tmp.elem[i][j]=elem[i][j]*times;
	return tmp;
}

qmatrix qmatrix::operator/(double div){
	qmatrix tmp;
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			tmp.elem[i][j]=elem[i][j]/div;
	return tmp;
}