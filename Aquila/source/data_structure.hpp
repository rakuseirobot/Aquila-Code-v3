#ifndef DATA_STRUECTURE_HPP_
#define DATA_STRUECTURE_HPP_
/*
 *  core.hpp 
 *  this is functions for mapping,algorithms,and so on. It's core of this robot.
 *  Author: Emile
 */ 
#define np nullptr
#define max_size 250
#define rep(i,N) for(int i=0;i<N;i++)
using ll = long long int;
using ci = const int;
using bl = bool;
namespace v{
    //for all ,such as wall,move,and so on...
    ci left =0;
    ci front=1;
    ci right=2;
    ci back=3;
    //direction
    ci unknown = 0;
    ci normal = 1;
    ci black=2;
    ci r_kit=3;
    ci c_point=4;
    ci slope=5;
    ci hisai=6;
    ci start = 7;
    ci kaidan = 8;
	//type
    ci vv[4][2]={{-1,0},{0,1},{1,0},{0,-1}};//vv[dir][0]:=x,[1]:=y
}

namespace color{
	ci white = 0;
	ci gray = 1;
	ci black = 2;
}

struct node{
    uint8_t x=100,y=100,z=1;//coordinate
	uint8_t flag,type;//for bfs,search
	uint8_t color;//for real bfs
    int depth=1000,dist=1000;//
	bl ac=false;//already checked?
    node* next[4]={np};
    node* back=np;
};

class queue{
    int siz;
    node* box[max_size];
public:
    void init(){siz=0;rep(i,max_size)box[i]=np;}
    int size(){return siz;}
    node* front(){ return box[0]; }
    void pop(){
        rep(i,siz){box[i]=box[i+1];}
        siz--;
    }
    void push(node* x){ box[siz]=x; siz++; }
    bl empty(){if(siz==0){return true;}else{return false;}}
};

class stack{
    int siz;
    node* box[max_size];
public:
    void init(){siz=0;rep(i,max_size)box[i]=np;}
    int size(){return siz;}
    node* top(){return box[siz-1];}
    void pop(){box[siz-1]=np;siz--;}
    void push(node* x){box[siz]=x;siz++;}
    bl empty(){if(siz==0){return true;}else{return false;}}
};

class nodes{
    int size,now;
    node mal[max_size];
    bl box[max_size]={0};
public:
    nodes(){size=0;now=0;};
    node* make(int x,int y,int z,int flag){
        if(now>=max_size-1){ return np; }else{
            now++;mal[now].dist=1000;mal[now].depth=1000;//init
            mal[now].x=x; mal[now].y=y; mal[now].z=z; mal[now].flag=flag;
			mal[now].color=color::white;
			mal[now].ac=false;
            return &mal[now];
        }
    }
    bl full(){ if(now>=max_size-1){ return true; }else{ return false; } }
};

int max(int x,int y){if(x>y){return x;}else{return y;}}
int min(int x,int y){if(x<y){return x;}else{return y;}}

#endif
