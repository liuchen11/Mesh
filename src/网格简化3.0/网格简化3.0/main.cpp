#include <iostream>
#include <string>
#include "mesh.h"
using namespace std;

int main(int argc,char* argv[]){
	if(argc!=4){
		cout<<"网格简化3.0.exe 输入.obj 输出.obj 简化比"<<endl;
		return -1;
	}
	string rate(argv[3]);
	double r=0,w=0.1;
	int point=0;
	//将第四个参数化为double类型
	for(int i=0;i<rate.size();i++){
		if(rate[i]=='.'){
			if(point==0)
				point=1;
			else{
				cout<<"简化比输入错误！"<<endl;
				return -1;
			}
		}
		else if(point==0){
			if(rate[i]>='0'&&rate[i]<='9')
				r=r*10+rate[i]-'0';
			else{
				cout<<"简化比输入错误！"<<endl;
				return -1;
			}
		}
		else{
			if(rate[i]>='0'&&rate[i]<='9'){
				r=r+w*(rate[i]-'0');
				w=w/10;
			}
			else{
				cout<<"简化比输入错误！"<<endl;
				return -1;
			}
		}
	}
	if(r>1){
		cout<<"简化比输入错误！"<<endl;
		return -1;
	}
	mesh func(r,0.005);
	string infile(argv[1]),outfile(argv[2]);
	func.input(infile);
	func.initialize();
	func.simplify();
	func.output(outfile);
	func.show();
	return 0;
}