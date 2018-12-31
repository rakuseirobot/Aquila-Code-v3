/*
 * data_structure.cpp
 *
 * Created: 2018/12/31 12:34:55
 *  Author: shun2
 */ 


#include "data_structure.hpp"


void queue::init(){siz=0;rep(i,max_size)box[i]=np;}
int queue::size(){return siz;}
node* queue::front(){ return box[0]; }
void queue::pop(){
    rep(i,siz){box[i]=box[i+1];}
    siz--;
}
void queue::push(node* x){ box[siz]=x; siz++; }
bl queue::empty(){if(siz==0){return true;}else{return false;}}


void stack::init(){siz=0;rep(i,max_size)box[i]=np;}
int stack::size(){return siz;}
node* stack::top(){return box[siz-1];}
void stack::pop(){box[siz-1]=np;siz--;}
void stack::push(node* x){box[siz]=x;siz++;}
bl stack::empty(){if(siz==0){return true;}else{return false;}}


nodes::nodes(){size=0;now=0;};
node* nodes::make(int x,int y,int z,int flag){
    if(now>=max_size-1){ return np; }else{
        now++;mal[now].dist=1000;mal[now].depth=1000;//init
        mal[now].x=x; mal[now].y=y; mal[now].z=z; mal[now].flag=flag;
        mal[now].color=color::white;
        mal[now].ac=false;
        return &mal[now];
    }
}
bl nodes::full(){ if(now>=max_size-1){ return true; }else{ return false; } }


int max(int x,int y){if(x>y){return x;}else{return y;}}
int min(int x,int y){if(x<y){return x;}else{return y;}}
