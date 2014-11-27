#include "matrix.h"
#include <iostream>
using namespace std;

matrix::matrix(int xx,int yy){
	x=xx,y=yy;
	elem=new double*[x];
	for(int i=0;i<x;i++){
		elem[i]=new double[y];
		for(int j=0;j<y;j++)
			elem[i][j]=0;
	}
}

void matrix::destroy(){
	if(elem!=NULL){
		for(int i=0;i<x;i++)
			if(elem[i]!=NULL)
				delete[] elem[i];
		delete[] elem;
	}
}

matrix::matrix(const matrix& copy){
	x=copy.x,y=copy.y;
	elem=new double*[x];
	for(int i=0;i<x;i++){
		elem[i]=new double[y];
		for(int j=0;j<y;j++)
			elem[i][j]=copy.elem[i][j];
	}
}

matrix::~matrix(){
	destroy();
}

matrix& matrix::operator=(const matrix& eval){
	destroy();
	x=eval.x,y=eval.y;
	elem=new double*[x];
	for(int i=0;i<x;i++){
		elem[i]=new double[y];
		for(int j=0;j<y;j++)
			elem[i][j]=eval.elem[i][j];
	}
	return (*this);
}

matrix& matrix::operator+(matrix& plus){
	if(x!=plus.x||y!=plus.y){
		cout<<"the sizes of matrixs don't match"<<endl;
		matrix *r=new matrix(1,1);
		return *r;
	}
	else{
		matrix *r=new matrix(x,y);
		for(int i=0;i<x;i++)
			for(int j=0;j<y;j++){
				r->elem[i][j]=elem[i][j]+plus.elem[i][j];
			}
		return *r;
	}
}

matrix& matrix::operator-(matrix& minus){
	if(x!=minus.x||y!=minus.y){
		cout<<"the sizes of matrixs don't match"<<endl;
		matrix *r=new matrix(1,1);
		return *r;
	}
	else{
		matrix *r=new matrix(x,y);
		for(int i=0;i<x;i++)
			for(int j=0;j<y;j++){
				r->elem[i][j]=elem[i][j]-minus.elem[i][j];
			}
		return *r;
	}
}

matrix& matrix::operator*(double times){
	matrix *r=new matrix(x,y);
	for(int i=0;i<x;i++)
		for(int j=0;j<y;j++){
			r->elem[i][j]=elem[i][j]*times;
		}
	return *r;
}

matrix& matrix::operator*(matrix& times){
	if(y!=times.x){
		cout<<"the sizes of matrixs don't match"<<endl;
		matrix *r=new matrix(1,1);
		return *r;
	}
	else{
		matrix *r=new matrix(x,times.y);
		for(int i=0;i<x;i++)
			for(int j=0;j<times.y;j++){
				for(int p=0;p<y;p++)
					r->elem[i][j]+=elem[i][p]*times.elem[p][j];
			}
		return *r;
	}
}

matrix& matrix::operator/(double div){
	matrix *r=new matrix(x,y);
	for(int i=0;i<x;i++)
		for(int j=0;j<y;j++)
			r->elem[i][j]=elem[i][j]/div;
	return *r;
}

matrix& matrix::submatrix(int xx,int yy){
	if(xx<0||yy<0||xx>=x||yy>=y){
		cout<<"xx or yy is out of range!"<<endl;
		matrix *r=new matrix(1,1);
		return *r;
	}
	else{
		matrix *r=new matrix(x-1,y-1);
		for(int i=0,I=0;i<x;i++){
			if(i==xx)
				continue;
			for(int j=0,J=0;j<y;j++){
				if(j==yy)
					continue;
				r->elem[I][J]=elem[i][j];
				J++;
			}
			I++;
		}
		return *r;
	}
}

double matrix::det(){
	if(x!=y){
		cout<<"it isn't a square matrix"<<endl;
		return 0;
	}
	else if(x==1){
		return elem[0][0];
	}
	else{
		double value=0;
		for(int i=0,j=1;i<x;i++){
			value+=submatrix(0,i).det()*elem[0][i]*j;
			j=-j;
		}
		return value;
	}
}

matrix& matrix::T(){
	matrix *r=new matrix(y,x);
	for(int i=0;i<y;i++)
		for(int j=0;j<x;j++){
			r->elem[i][j]=elem[j][i];
		}
	return *r;
}

matrix& matrix::reverse(){
	if(x!=y){
		cout<<"the matrix is not a square matrix"<<endl;
		matrix *r=new matrix(1,1);
		return *r;
	}
	else{
		double d=this->det();
		if(d<1e-10&&d>-1e-10){
			cout<<"the det of the matrix is 0!"<<endl;
			matrix *r=new matrix(1,1);
			return *r;
		}
		else{
			matrix *r=new matrix(x,y);
			for(int i=0;i<x;i++)
				for(int j=0;j<y;j++){
					if((i+j)%2==0)
						r->elem[i][j]=submatrix(j,i).det()/d;
					else
						r->elem[i][j]=submatrix(j,i).det()*(-1)/d;
				}
			return *r;
		}
	}
}