#include "mesh.h"
#include <iostream>
#include <cmath>
using namespace std;

mesh::mesh(double r,double max){
	pt.clear();tri.clear();heap.clear();
	lx=NULL,ly=NULL,index=NULL,t_table=NULL,qpt=NULL,qface=NULL;
	m_point=0,m_tri=0;
	del_point=0,del_tri=0;
	rate=r,maxdis=max;
}

mesh::~mesh(){
	pt.clear();tri.clear();heap.clear();
	lx=NULL,ly=NULL,index=NULL,t_table=NULL,qpt=NULL,qface=NULL;
	m_point=0,m_tri=0;
}

int mesh::input(string file){
	FILE* fp=fopen(file.c_str(),"r");
	if(fp==NULL){
		cout<<"loading error!"<<endl;
		return 0;
	}
	else{
		char buf[256];
		int linenumber=0;
		while(fscanf(fp,"%s",buf)!=EOF){
			//printf("%d\n",linenumber);
			if(linenumber==92171){
				int oi=0;
				oi++;
			}
			linenumber++;
			switch(buf[0]){
			case '#':
				fgets(buf,sizeof(buf),fp);
				break;
			case 'v':
				switch(buf[1]){
				case '\0':
					double xx,yy,zz;
					if(fscanf(fp,"%lf" "%lf" "%lf",&xx,&yy,&zz)==3){
						m_point++;
						point add(xx,yy,zz);
						pt.push_back(add);
					}
					else{
						cout<<"the number of para is wrong"<<endl;
						return 0;
					}
					break;
				default:
					fgets(buf,sizeof(buf),fp);
					break;
				}
				break;
			case 'f':
				int pp1,pp2,pp3;
				int v,n,t;
				if(fscanf(fp,"%s",buf)!=1){
					cout<<"Wrong Face"<<endl;
					return 0;
				}
				if(strstr(buf,"//")){
					if(sscanf(buf,"%d//%d",&pp1,&n)==2&&fscanf(fp,"%d//%d",&pp2,&n)&&fscanf(fp,"%d//%d",&pp3,&n)){
						m_tri++;
						tri_num add(pp1-1,pp2-1,pp3-1);
						tri.push_back(add);
					}
					else{
						cout<<"Wrong Face"<<endl;
						return 0;
					}
				}
				else if(sscanf(buf,"%d/%d/%d",&v,&t,&n)==3){
					pp1=v;
					if(fscanf(fp,"%d/%d/%d",&pp2,&t,&n)==3&&fscanf(fp,"%d/%d/%d",&pp3,&t,&n)==3){
						m_tri++;
						tri_num add(pp1-1,pp2-1,pp3-1);
						tri.push_back(add);
					}
					else{
						cout<<"Wrong Face!"<<endl;
						return 0;
					}
				}
				else if(sscanf(buf,"%d/%d",&v,&t)==2){
					pp1=v;
					if(fscanf(fp,"%d/%d",&pp2,&t)==2&&fscanf(fp,"%d/%d",&pp3,&t)==2){
						m_tri++;
						tri_num add(pp1-1,pp2-1,pp3-1);
						tri.push_back(add);
					}
					else{
						cout<<"Wrong Face"<<endl;
						return 0;
					}
				}
				else{
					if(sscanf(buf,"%d",&pp1)==1&&fscanf(fp,"%d",&pp2)==1&&fscanf(fp,"%d",&pp3)==1){
						m_tri++;
						tri_num add(pp1-1,pp2-1,pp3-1);
						tri.push_back(add);
					}
					else{
						cout<<"Wrong Face!"<<endl;
						return 0;
					}
				}
				break;
			default:
				fgets(buf,sizeof(buf),fp);
				break;
			}
		}
		for(int i=0;i<m_tri;i++){
			if(tri[i].p1<0||tri[i].p1>=m_point){
				cout<<"exceed the number of points"<<endl;
				return 0;
			}
			if(tri[i].p2<0||tri[i].p2>=m_point){
				cout<<"exceed the number of points"<<endl;
				return 0;
			}
			if(tri[i].p3<0||tri[i].p3>=m_point){
				cout<<"exceed the number of points"<<endl;
				return 0;
			}
		}
		return 1;
	}
}

void mesh::output(string file){
	FILE* fp=fopen(file.c_str(),"w");
	if(fp==NULL){
		cout<<"failed to output"<<endl;
		return;
	}
	int *sub=new int[pt.size()];			//表示该位置之前的无效点的个数(不包括该点)
	sub[0]=0;
	for(int i=0;i<pt.size()-1;i++){
		if(pt[i].del==1)
			sub[i+1]=sub[i]+1;
		else
			sub[i+1]=sub[i];
	}
	for(int i=0;i<m_point;i++)
		if(pt[i].del==0)
			fprintf(fp,"v %f %f %f\n",(float)pt[i].x,(float)pt[i].y,(float)pt[i].z);
	for(int i=0;i<m_tri;i++){
		int point1=tri[i].p1,point2=tri[i].p2,point3=tri[i].p3;
		point1=xy[point1],point2=xy[point2],point3=xy[point3];
		if(point1==point2||point1==point3||point2==point3)
			continue;
		point1=point1-sub[point1]+1;
		point2=point2-sub[point2]+1;
		point3=point3-sub[point3]+1;
		fprintf(fp,"f %d %d %d\n",point1,point2,point3);
	}
	fclose(fp);
	return;
}

double dis(const point &p1,const point &p2){
	double square=(p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y)+(p1.z-p2.z)*(p1.z-p2.z);
	return sqrt(square);
}

void mesh::initialize(){
	if(m_point==0||m_tri==0){
		cout<<"the mesh hasn't initialized!"<<endl;
		return;
	}
	lx=new line*[m_point],ly=new line*[m_point];
	t_table=new vector<int>[m_point],index=new vector<int>[m_point];
	qpt=new qmatrix[m_point],qface=new qmatrix[m_tri];
	xy=new int[m_point];
	for(int i=0;i<m_point;i++){
		t_table[i].clear();
		index[i].clear();
		index[i].push_back(i);
		lx[i]=NULL,ly[i]=NULL;
		xy[i]=i;
	}
	for(int i=0;i<m_tri;i++){
		int index1=tri[i].p1,index2=tri[i].p2,index3=tri[i].p3;		//三个顶点
		point pp1=pt[index1],pp2=pt[index2],pp3=pt[index3];
		tri[i].calc(pp1,pp2,pp3);									//计算a,b,c,d;
		double aa=tri[i].a,bb=tri[i].b,cc=tri[i].c,dd=tri[i].d;		//更新面的Q值
		qface[i].elem[0][0]=aa*aa,qface[i].elem[0][1]=aa*bb,qface[i].elem[0][2]=aa*cc,qface[i].elem[0][3]=aa*dd;
		qface[i].elem[1][0]=bb*aa,qface[i].elem[1][1]=bb*bb,qface[i].elem[1][2]=bb*cc,qface[i].elem[1][3]=bb*dd;
		qface[i].elem[2][0]=cc*aa,qface[i].elem[2][1]=cc*bb,qface[i].elem[2][2]=cc*cc,qface[i].elem[2][3]=cc*dd;
		qface[i].elem[3][0]=dd*aa,qface[i].elem[3][1]=dd*bb,qface[i].elem[3][2]=dd*cc,qface[i].elem[3][3]=dd*dd;
		qpt[index1]=qpt[index1]+qface[i];							//更新点的Q值
		qpt[index2]=qpt[index2]+qface[i];
		qpt[index3]=qpt[index3]+qface[i];
		line *a1=new line(index1,index2),*a2=new line(index1,index3),*a3=new line(index2,index3);
		a1->length=dis(pp1,pp2),a2->length=dis(pp1,pp3),a3->length=dis(pp2,pp3);//计算距离
		//建立十字链表,lx为横向,ly为纵向
		int s1=a1->first,s2=a2->first,s3=a3->first;
		int l1=a1->second,l2=a2->second,l3=a3->second;
		int appeared=0;
		line *xx=lx[s1],*yy=ly[l1];
		if(xx==NULL){
			lx[s1]=a1;
		}
		else{
			while(xx!=NULL){
				if(xx->second>a1->second){
					a1->down=lx[s1];
					lx[s1]->up=a1;
					lx[s1]=a1;
					break;
				}
				else if(xx->second==a1->second){
					delete a1;
					appeared=1;
					break;
				}
				else{
					if(xx->down==NULL){
						xx->down=a1;
						a1->up=xx;
						break;
					}
					else if(xx->down->second>a1->second){
						a1->down=xx->down;
						a1->up=xx;
						xx->down->up=a1;
						xx->down=a1;
						break;
					}
					else
						xx=xx->down;
				}
			}
		}
		if(appeared==1)
			appeared=0;
		else if(yy==NULL){
			ly[l1]=a1;
		}
		else{
			while(yy!=NULL){
				if(yy->first>a1->first){
					a1->right=ly[l1];
					ly[l1]->left=a1;
					ly[l1]=a1;
					break;
				}
				else if(yy->first==a1->first)
					break;
				else{
					if(yy->right==NULL){
						a1->left=yy;
						yy->right=a1;
						break;
					}
					else if(yy->right->first>a1->first){
						a1->right=yy->right;
						a1->left=yy;
						yy->right->left=a1;
						yy->right=a1;
						break;
					}
					else
						yy=yy->right;
				}
			}
		}
		xx=lx[s2],yy=ly[l2];
		if(xx==NULL){
			lx[s2]=a2;
		}
		else{
			while(xx!=NULL){
				if(xx->second>a2->second){
					a2->down=lx[s2];
					lx[s2]->up=a2;
					lx[s2]=a2;
					break;
				}
				else if(xx->second==a2->second){
					delete a2;
					appeared=1;
					break;
				}
				else{
					if(xx->down==NULL){
						xx->down=a2;
						a2->up=xx;
						break;
					}
					else if(xx->down->second>a2->second){
						a2->down=xx->down;
						a2->up=xx;
						xx->down->up=a2;
						xx->down=a2;
						break;
					}
					else
						xx=xx->down;
				}
			}
		}
		if(appeared==1)
			appeared=0;
		else if(yy==NULL){
			ly[l2]=a2;
		}
		else{
			while(yy!=NULL){
				if(yy->first>a2->first){
					a2->right=ly[l2];
					ly[l2]->left=a2;
					ly[l2]=a2;
					break;
				}
				else if(yy->first==a2->first)
					break;
				else{
					if(yy->right==NULL){
						yy->right=a2;
						a2->left=yy;
						break;
					}
					else if(yy->right->first>a2->first){
						a2->right=yy->right;
						a2->left=yy;
						yy->right->left=a2;
						yy->right=a2;
						break;
					}
					else
						yy=yy->right;
				}
			}
		}
		xx=lx[s3],yy=ly[l3];
		if(xx==NULL){
			lx[s3]=a3;
		}
		else{
			while(xx!=NULL){
				if(xx->second>a3->second){
					a3->down=lx[s3];
					lx[s3]->up=a3;
					lx[s3]=a3;
					break;
				}
				else if(xx->second==a3->second){
					delete a3;
					appeared=1;
					break;
				}
				else{
					if(xx->down==NULL){
						xx->down=a3;
						a3->up=xx;
						break;
					}
					else if(xx->down->second>a3->second){
						a3->down=xx->down;
						a3->up=xx;
						xx->down->up=a3;
						xx->down=a3;
						break;
					}
					else
						xx=xx->down;
				}
			}
		}
		if(appeared==1)
			appeared=0;
		else if(yy==NULL){
			ly[l3]=a3;
		}
		else{
			while(yy!=NULL){
				if(yy->first>a3->first){
					a3->right=ly[l3];
					ly[l3]->left=a3;
					ly[l3]=a3;
					break;
				}
				else if(yy->first==a3->first)
					break;
				else{
					if(yy->right==NULL){
						yy->right=a3;
						a3->left=yy;
						break;
					}
					else if(yy->right->first>a3->first){
						a3->right=yy->right;
						a3->left=yy;
						yy->right->left=a3;
						yy->right=a3;
						break;
					}
					else
						yy=yy->right;
				}
			}
		}
	}
}

void _swap(heap_node& a1,heap_node& a2){
	heap_node tmp=a1;
	a1=a2;
	a2=tmp;
}

void mesh::heap_adjust(int num){
	if(num<0||num>=heap.size()){
		cout<<"the position in this heap doesn't exist!"<<endl;
		return;
	}
	if(num>0&&heap[(num-1)/2].delta>heap[num].delta){			//比父亲节点要小
		while(num>0){
			if(heap[(num-1)/2].delta>heap[num].delta){
				heap[num].back->index=(num-1)/2;
				heap[(num-1)/2].back->index=num;
				_swap(heap[num],heap[(num-1)/2]);
				num=(num-1)/2;
			}
			else
				break;
		}
	}
	else if(num*2+1<heap.size()){
		while(num*2+1<heap.size()){
			if(num*2+2<heap.size()){
				if(heap[num*2+1].delta<heap[num].delta&&heap[num*2+1].delta<heap[num*2+2].delta){
					heap[num].back->index=num*2+1;
					heap[num*2+1].back->index=num;
					_swap(heap[num],heap[num*2+1]);
					num=num*2+1;
				}
				else if(heap[num*2+2].delta<heap[num].delta&&heap[num*2+2].delta<heap[num*2+1].delta){
					heap[num].back->index=num*2+2;
					heap[num*2+2].back->index=num;
					_swap(heap[num],heap[num*2+2]);
					num=num*2+2;
				}
				else
					break;
			}
			else{
				if(heap[num*2+1].delta<heap[num].delta){
					heap[num].back->index=num*2+1;
					heap[num*2+1].back->index=num;
					_swap(heap[num],heap[num*2+1]);
					break;
				}
				else
					break;
			}
		}
	}
}

void mesh::find(){					//选出符合要求的line,组建成堆
	for(int i=0;i<m_point;i++){
		line* present=lx[i];
		while(present!=NULL){
			if(present->length<maxdis){
				int pt1=present->first,pt2=present->second;
				pt1=xy[pt1],pt2=xy[pt2];
				double _delta=present->calc(pt[pt1],pt[pt2],qpt[pt1],qpt[pt2]);
				//加入一个节点
				heap.push_back(heap_node(_delta,present));
				present->index=heap.size()-1;
				heap_adjust(heap.size()-1);
			}
			present=present->down;
		}
	}
}
/*
int mesh::del_a_point(){					//删除成功返回删除的面数，删除失败返回-1
	int n1=heap[0].back->first,n2=heap[0].back->second;
	n1=xy[n1],n2=xy[n2];
	if(n1==n2){
		heap[0].back->index=-1;
		heap[heap.size()-1].back->index=0;
		_swap(heap[0],heap[heap.size()-1]);
		heap.pop_back();
		heap_adjust(0);
		return -1;
	}
	else{
		vector<int> del_face;										//删除的面的非合并点
		del_face.clear();
		//取代,更新点阵的基础信息
		int index1=heap[0].back->first,index2=heap[0].back->second;
		index1=xy[index1],index2=xy[index2];						//index1，index2实际存在的点的编号
		point old1=pt[index1],old2=pt[index2];						//旧的点信息
		pt[index1]=heap[0].back->combine;							//保留index1删除index2
		pt[index2].del=1;
		heap[heap.size()-1].back->index=0;
		_swap(heap[0],heap[heap.size()-1]);
		heap.pop_back();
		heap_adjust(0);
		//更新面的信息
		//消除旧的面index1、index2以及周边顶点的影响
		for(int i=0;i<index[index1].size();i++){
			int pos=index[index1][i];
			for(int j=0;j<t_table[pos].size();j++){
				int number=t_table[pos][j];
				int point1=tri[number].p1,point2=tri[number].p2,point3=tri[number].p3;
				point1=xy[point1],point2=xy[point2],point3=xy[point3];
				if(point1!=point2&&point1!=point3&&point2!=point3){
					qpt[point1]=qpt[point1]-qface[number];
					qpt[point2]=qpt[point2]-qface[number];
					qpt[point3]=qpt[point3]-qface[number];
				}
			}
		}
		for(int i=0;i<index[index2].size();i++){
			int pos=index[index2][i];
			for(int j=0;j<t_table[pos].size();j++){
				int number=t_table[pos][j];
				int point1=tri[number].p1,point2=tri[number].p2,point3=tri[number].p3;
				point1=xy[point1],point2=xy[point2],point3=xy[point3];
				if(point1!=index1&&point2!=index1&&point3!=index1&&point1!=point2&&point1!=point3&&point2!=point3){
					qpt[point1]=qpt[point1]-qface[number];
					qpt[point2]=qpt[point2]-qface[number];
					qpt[point3]=qpt[point3]-qface[number];
				}
				else if(point1==index1||point2==index1||point3==index1){
					int todel=-1;
					if(point1!=index1&&point1!=index2)
						todel=point1;
					else if(point2!=index1&&point2!=index2)
						todel=point2;
					else if(point3!=index1&&point3!=index2)
						todel=point3;
					for(int p=0;;p++){
						if(p==del_face.size()){
							del_face.push_back(todel);
							break;
						}
						else if(del_face[p]==todel)
							break;
					}
				}
			}
		}
		//合并列表
		for(int i=0;i<index[index2].size();i++){
			int pos=index[index2][i];
			xy[pos]=index1;
		}
		int size=index[index2].size();
		for(int i=0;i<size;i++){
			int add=index[index2][index[index2].size()-1];
			index[index1].push_back(add);
			index[index2].pop_back();
		}
		//加上新的面,更新点的信息
		for(int i=0;i<index[index1].size();i++){
			int pos=index[index1][i];
			for(int j=0;j<t_table[pos].size();j++){
				int number=t_table[pos][j];
				int point1=tri[number].p1,point2=tri[number].p2,point3=tri[number].p3;
				point1=xy[point1],point2=xy[point2],point3=xy[point3];					//重复的面
				if(point1!=point2&&point1!=point3&&point2!=point3){
					tri[number].calc(pt[point1],pt[point2],pt[point3]);
					double aa=tri[number].a,bb=tri[number].b,cc=tri[number].c,dd=tri[number].d;
					qface[number].elem[0][0]=aa*aa,qface[number].elem[0][1]=aa*bb,qface[number].elem[0][2]=aa*cc,qface[number].elem[0][3]=aa*dd;
					qface[number].elem[1][0]=bb*aa,qface[number].elem[1][1]=bb*bb,qface[number].elem[1][2]=bb*cc,qface[number].elem[1][3]=bb*dd;
					qface[number].elem[2][0]=cc*aa,qface[number].elem[2][1]=cc*bb,qface[number].elem[2][2]=cc*cc,qface[number].elem[2][3]=cc*dd;
					qface[number].elem[3][0]=dd*aa,qface[number].elem[3][1]=dd*bb,qface[number].elem[3][2]=dd*cc,qface[number].elem[3][3]=dd*dd;
					qpt[point1]=qpt[point1]+qface[number];
					qpt[point2]=qpt[point2]+qface[number];
					qpt[point3]=qpt[point3]+qface[number];
				}
			}
		}
		//更新边的信息,更新相关节点,小于delta的加入堆
		for(int i=0;i<index[index1].size();i++){			//删除不必要的节点
			int pos=index[index1][i];
			line* present=lx[pos];
			while(present!=NULL){
				int px=present->first,py=present->second;
				px=xy[px],py=xy[py];
				if(px!=py){
					present->length=dis(pt[px],pt[py]);
					double d=present->calc(pt[px],pt[py],qpt[px],qpt[py]);
					int _number=present->index;
					if(_number<0&&d<maxdis){
						//加入新顶点
						heap.push_back(heap_node(d,present));
						present->index=heap.size()-1;
						heap_adjust(heap.size()-1);
					}
					else if(_number>=0){
						heap[_number].delta=d;
						heap_adjust(_number);
					}
				}
				present=present->down;
			}
			present=ly[pos];
			while(present!=NULL){
				int px=present->first,py=present->second;
				px=xy[px],py=xy[py];
				if(px!=py){
					present->length=dis(pt[px],pt[py]);
					double d=present->calc(pt[px],pt[py],qpt[px],qpt[py]);
					int _number=present->index;
					if(_number<0&&d<maxdis){
						//加入新顶点
						heap.push_back(heap_node(d,present));
						present->index=heap.size()-1;
						heap_adjust(heap.size()-1);
					}
					else if(_number>=0){
						heap[_number].delta=d;
						heap_adjust(_number);
					}
				}
				present=present->right;
			}
		}
		return del_face.size();
	}
}
*/
int mesh::del_a_point(){
	int index1=heap[0].back->first,index2=heap[0].back->second;
	index1=xy[index1],index2=xy[index2];
	if(index1==index2){
		heap[0].back->index=-1;
		heap[heap.size()-1].back->index=0;
		_swap(heap[0],heap[heap.size()-1]);
		heap.pop_back();
		heap_adjust(0);
		return -1;
	}
	else{
		point old1=pt[index1],old2=pt[index2];						//旧的点信息
		pt[index1]=heap[0].back->combine;							//保留index1删除index2
		pt[index2].del=1;
		heap[heap.size()-1].back->index=0;
		_swap(heap[0],heap[heap.size()-1]);
		heap.pop_back();
		heap_adjust(0);
		//消除旧的面对index1,index2以及周边面的影响
		for(int i=0;i<t_table[index1].size();i++){
			int number=t_table[index1][i];
			int point1=tri[number].p1,point2=tri[number].p2,point3=tri[number].p3;
			point1=xy[point1],point2=xy[point2],point3=xy[point3];
			if(point1!=point2&&point1!=point3&&point2!=point3){
				qpt[point1]=qpt[point1]-qface[number];
				qpt[point2]=qpt[point2]-qface[number];
				qpt[point3]=qpt[point3]-qface[number];
			}
		}
		for(int i=0;i<t_table[index2].size();i++){
			int number=t_table[index2][i];
			int point1=tri[number].p1,point2=tri[number].p2,point3=tri[number].p3;
			point1=xy[point1],point2=xy[point2],point3=xy[point3];
			if(point1!=index1&&point2!=index1&&point3!=index1&&point1!=point2&&point1!=point3&&point2!=point3){
				qpt[point1]=qpt[point1]-qface[number];
				qpt[point2]=qpt[point2]-qface[number];
				qpt[point3]=qpt[point3]-qface[number];
			}
		}
		//合并面列表
		for(int i=0;i<t_table[index2].size();i++){
			int number=t_table[index2][i];
			if(tri[number].p1==index2)
				tri[number].p1=index1;
			if(tri[number].p2==index2)
				tri[number].p2=index1;
			if(tri[number].p3==index2)
				tri[number].p3=index1;
			int point1=tri[number].p1,point2=tri[number].p2,point3=tri[number].p3;
			if(point1==point2||point1==point3||point2==point3)
				continue;
			for(int j=0;;j++){
				if(j==t_table[index1].size()){
					t_table[index1].push_back(number);
					break;
				}
				int _number=t_table[index1][j];
				int pt1=tri[_number].p1,pt2=tri[_number].p2,pt3=tri[_number].p3;
				if((pt1==point1&&pt2==point2&&pt3==point3)||(pt1==point1&&pt2==point3&&pt3==point2)||(pt1==point2&&pt2==point1&&pt3==point3)||(pt1==point2&&pt2==point3&&pt3==point1)||(pt1==point3&&pt2==point1&&pt3==point2)||(pt1==point3&&pt2==point2&&pt3==point1))
					break;
			}
		}
		xy[index2]=index1;
		t_table[index2].clear();
		cout<<"add"<<endl;
		//加上新的面，更新面信息
		for(int i=0;i<t_table[index1].size();i++){
			int number=t_table[index1][i];
			int point1=tri[number].p1,point2=tri[number].p2,point3=tri[number].p3;
			point1=xy[point1],point2=xy[point2],point3=xy[point3];
			if(point1!=point2&&point1!=point3&&point2!=point3){
				tri[number].calc(pt[point1],pt[point2],pt[point3]);
				double aa=tri[number].a,bb=tri[number].b,cc=tri[number].c,dd=tri[number].d;
				qface[number].elem[0][0]=aa*aa,qface[number].elem[0][1]=aa*bb,qface[number].elem[0][2]=aa*cc,qface[number].elem[0][3]=aa*dd;
				qface[number].elem[1][0]=bb*aa,qface[number].elem[1][1]=bb*bb,qface[number].elem[1][2]=bb*cc,qface[number].elem[1][3]=bb*dd;
				qface[number].elem[2][0]=cc*aa,qface[number].elem[2][1]=cc*bb,qface[number].elem[2][2]=cc*cc,qface[number].elem[2][3]=cc*dd;
				qface[number].elem[3][0]=dd*aa,qface[number].elem[3][1]=dd*bb,qface[number].elem[3][2]=dd*cc,qface[number].elem[3][3]=dd*dd;
				qpt[point1]=qpt[point1]+qface[number];
				qpt[point2]=qpt[point2]+qface[number];
				qpt[point3]=qpt[point3]+qface[number];
			}
		}
		//更新边的信息，更新堆里相关节点，小于delta的加入堆
		line *pre1x=lx[index1],*pre1y=ly[index1],*pre2x=lx[index2],*pre2y=ly[index2];		//index1比index2小
		//抽出
		while(pre2x!=NULL){
			if(pre2x->right!=NULL)
				pre2x->right->left=pre2x->left;
			if(pre2x->left!=NULL)
				pre2x->left->right=pre2x->right;
			pre2x->first=index1;
			pre2x=pre2x->down;
		}
		while(pre2y!=NULL){
			if(pre2y->up!=NULL)
				pre2y->up->down=pre2y->down;
			if(pre2y->down!=NULL)
				pre2y->down->up=pre2y->up;
			pre2y->second=index1;
			if(pre2y->second<pre2y->first){
				int tmp=pre2y->first;
				pre2y->first=pre2y->second;
				pre2y->second=tmp;
			}
			pre2y=pre2y->right;
		}
		cout<<"begin"<<endl;
		pre2x=lx[index2],pre2y=ly[index2];	//此处可加速
		lx[index2]=NULL,ly[index2]=NULL;
		while(pre2x!=NULL){
			line* t=pre2x->down;
			pre2x->down=NULL,pre2x->left=NULL,pre2x->right=NULL,pre2x->up=NULL;
			int s=pre2x->first,l=pre2x->second;
			line *tx=lx[s],*ty=ly[l];
			int appeared=0;
			if(tx==NULL){
				lx[s]=pre2x;
			}
			else{
				while(tx!=NULL){
					if(tx->second>pre2x->second){
						pre2x->down=lx[s];
						lx[s]->up=pre2x;
						lx[s]=pre2x;
						break;
					}
					else if(tx->second==pre2x->second){
						int position=pre2x->index;
						if(position>=0){
							heap[heap.size()-1].back->index=position;
							_swap(heap[position],heap[heap.size()-1]);
							heap.pop_back();
							heap_adjust(position);
						}
						if(pre2x==lx[s]){
							lx[s]=t;
							cout<<"-------------------------------------------"<<endl;
						}
						//delete pre2x;			//
						appeared=1;
						break;
					}
					else{
						if(tx->down==NULL){
							tx->down=pre2x;
							pre2x->up=tx;
							break;
						}
						else if(tx->down->second>pre2x->second){
							pre2x->down=tx->down;
							pre2x->up=tx;
							tx->down->up=pre2x;
							tx->down=pre2x;
							break;
						}
						else
							tx=tx->down;
					}
				}
			}
			if(appeared==1)
				appeared=1;
			else if(ty==NULL){
				ly[l]=pre2x;
			}
			else{
				while(ty!=NULL){
					if(ty->first>pre2x->first){
						pre2x->right=ly[l];
						ly[l]->left=pre2x;
						ly[l]=pre2x;
						break;
					}
					else if(ty->first==pre2x->first)
						break;
					else{
						if(ty->right==NULL){
							ty->right=pre2x;
							pre2x->left=ty;
							break;
						}
						else if(ty->right->first>pre2x->first){
							pre2x->right=ty->right;
							pre2x->left=ty;
							ty->right->left=pre2x;
							ty->right=pre2x;
							break;
						}
						else
							ty=ty->right;
					}
				}
			}
			pre2x=t;
		}
		while(pre2y!=NULL){
			line* t=pre2y->right;
			pre2y->down=NULL,pre2y->left=NULL,pre2y->right=NULL,pre2y->up=NULL;
			int s=pre2y->first,l=pre2y->second;
			line *tx=lx[s],*ty=ly[l];
			int appeared=0;
			if(tx==NULL){
				lx[s]=pre2y;
			}
			else{
				while(tx!=NULL){
					if(tx->second>pre2y->second){
						pre2y->down=lx[s];
						lx[s]->up=pre2y;
						lx[s]=pre2y;
						break;
					}
					else if(tx->second==pre2y->second){
						int position=pre2y->index;
						if(position>=0){
							heap[heap.size()-1].back->index=position;
							_swap(heap[position],heap[heap.size()-1]);
							heap.pop_back();
							heap_adjust(position);
						}
						if(pre2y==lx[s]){
							lx[s]=pre2y->down;
							//cout<<"-------------------------------------------"<<endl;
						}
						//delete pre2y;			//
						appeared=1;
						break;
					}
					else{
						if(tx->down==NULL){
							tx->down=pre2y;
							pre2y->up=tx;
							break;
						}
						else if(tx->down->second>pre2y->second){
							pre2y->down=tx->down;
							pre2y->up=tx;
							tx->down->up=pre2y;
							tx->down=pre2y;
							break;
						}
						else
							tx=tx->down;
					}
				}
			}
			if(appeared==1)
				appeared=1;
			else if(ty==NULL){
				ly[l]=pre2y;
			}
			else{
				while(ty!=NULL){
					if(ty->first>pre2y->first){
						pre2y->right=ly[l];
						ly[l]->left=pre2y;
						ly[l]=pre2y;
						break;
					}
					else if(ty->first==pre2y->first)
						break;
					else{
						if(ty->right==NULL){
							ty->right=pre2y;
							pre2y->left=ty;
							break;
						}
						else if(ty->right->first>pre2y->first){
							pre2y->right=ty->right;
							pre2y->left=ty;
							ty->right->left=pre2y;
							ty->right=pre2y;
							break;
						}
						else
							ty=ty->right;
					}
				}
			}
			pre2y=t;
		}
		//调整堆节点的值
		line *ix=lx[index1],*iy=ly[index1];
		while(ix!=NULL){
			int point1=ix->first,point2=ix->second;
			cout<<"point1:"<<point1<<" point2:"<<point2<<endl;
			double d=ix->calc(pt[point1],pt[point2],qpt[point1],qpt[point2]);
			int _number=ix->index;
			cout<<heap.size()<<"  "<<_number<<endl;
			if(_number<0&&d<maxdis){
				heap.push_back(heap_node(d,ix));
				ix->index=heap.size()-1;
				heap_adjust(heap.size()-1);
			}
			else if(_number>=0){
				heap[_number].delta=d;
				heap_adjust(_number);
			}
			ix=ix->down;
		}
		while(iy!=NULL){
			int point1=iy->first,point2=iy->second;
			cout<<"point1:"<<point1<<" point2:"<<point2<<endl;
			double d=iy->calc(pt[point1],pt[point2],qpt[point1],qpt[point2]);
			int _number=iy->index;
			if(_number<0&&d<maxdis){
				heap.push_back(heap_node(d,iy));
				iy->index=heap.size()-1;
				heap_adjust(heap.size()-1);
			}
			else if(_number>=0){
				heap[_number].delta=d;
				heap_adjust(_number);
			}
			iy=iy->right;
		}
		return 0;
	}
}


void mesh::simplify(){
	while(m_point-del_point>=(double)m_point*rate){			//以删除的面作为结束标志
		if(heap.size()==0){
			maxdis*=2;
			find();
		}
		else{
			printf("%lf\n",rate);
			printf("%d-%d\n",m_point,del_point);
			//cout<<del_tri<<endl;
			int result=del_a_point();
			if(result>=0){
				del_point++;
				del_tri+=result;
			}
		}
	}
}

void mesh::check(){
	for(int i=0;i<20;i++){
		cout<<i<<"/  ";
		line* present=lx[i];
		while(present!=NULL){
			cout<<present->second<<"  ";
			present=present->down;
		}
		cout<<endl;
	}
	for(int i=0;i<20;i++){
		cout<<i<<"/  ";
		line* present=ly[i];
		while(present!=NULL){
			cout<<present->first<<"  ";
			present=present->right;
		}
		cout<<endl;
	}
}