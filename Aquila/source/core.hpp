#ifndef CORE_HPP_
#define CORE_HPP_
/*
 * core.hpp
 *
 * Created: 2018/12/20 12:00:00
 *  Author: Emile
 */ 
#include "data_structure.hpp"

nodes mall;

class core{
    node* now;
    node* start;//it may be unnecessary.But now, it is used in dfs.
    int dir;//0::<,1::^,2::>,3::v
    //for status
    //long long unsigned int counter;//dfs's counter.
    node* ans;//dfs's answer.
    int flg;//0 or 1
    //for dfs
    queue q;//for bfs
    /*
     * use ans,flg (also in bfs)
     */
public:
	stack stk;
    core(){
        now=mall.make(100,100,1,1);
        flg=0;
        ans = np;
        start=now;
        dir=0;
    };
    void turn_r(){ dir=(dir+1)%4; };
    void turn_l(){ dir=(dir+4-1)%4; };
    node* r_now(){return now;}
	node* r_start(){return start;}
    void dfs(node* t,int x,int y,int z,int depth){
        if(t!=np){
            if(t->depth>depth+1)t->depth=depth+1;
            if((long long int)(t->flag)!=flg){
                if(t->x==x && t->y==y && t->z==z){ ans=t; }
                t->flag=flg;
                dfs(t->next[0],x,y,z,t->depth);
                dfs(t->next[1],x,y,z,t->depth);
                dfs(t->next[2],x,y,z,t->depth);
                dfs(t->next[3],x,y,z,t->depth);
            }
        }
    };

    node* find(int x,int y,int z){
        ans=np;
        dfs(start,x,y,z,start->depth);
        flg=(flg+1)%2;
        return ans;
    };

    void cn_graph(node* v, node* u){//Connect Nodes on Graph ::vとuをgraph(next[])に関してつなげる
        if(v!=np && u!=np){
            rep(i,4)if(v->next[i]==np){ v->next[i]=u; break; }else if(v->next[i]==u){break;}
            rep(i,4)if(u->next[i]==np){ u->next[i]=v; break; }else if(u->next[i]==v){break;}
        }
    }
    void cn_tree(node* par,node* v){//connect nodes on Tree ::par(ent)とvをtree(back)に関してつなげる
        if(v!=np && par!=np)if(v->back==np)v->back=par;
    }
    void ap_node(node* t,int dire){//append node (cn_graph)
        dire = (dire+dir+3)%4;
        node* u = find(t->x+v::vv[dire][0],t->y+v::vv[dire][1],t->z);
        if(u==np)u = mall.make(t->x+v::vv[dire][0],t->y+v::vv[dire][1],t->z,(flg+1)%2);
        cn_graph(t,u);
    }
    node* ac_next(node* t,int now_dir,int dire,int dist){//(node*,int,int,int)->node* | nullptr(error) 
        int direction = (dire+now_dir+3)%4;
        if(t==np)return np;
        return find(t->x + v::vv[direction][0]*dist , t->y + v::vv[direction][1]*dist , t->z);
    }
    node* ac_next(int dire,int dist){ return ac_next(now,dir,dire,dist); }

	bl ck_conect(node* s,node* t){
		bl ans = false;
		rep(i,4)if(s->next[i]==t){ans = true;break;}
		return ans;
	}

    void go_st(){
        node* next = ac_next(v::front,1);
        if(next==np)ap_node(now,v::front);
        next = ac_next(v::front,1);
        cn_graph(now,next);
        cn_tree(now,next);
        now = next;
    }

    void cl_dist(node* t,int d){//in clear_dist
        if(t!=np){
            t->dist=d;t->flag=flg;
            rep(i,4){
                if(t->next[i]!=np){
                    if(t->next[i]->flag!=flg){cl_dist(t->next[i],d);}
                }else{ break; }
            }
        }
    }

    void clear_dist(){
        cl_dist(now,1000);
        flg=(flg+1)%2;
    }


    void bfs(node* s,node* t){//sを始点にしてtを検索する。
        q.push(s);
        s->dist=0;
        while(!q.empty()){
            node* a = q.front();
            if(a==t){ans = t;}
            rep(i,4){
                node* aa=a->next[i];
                if(aa!=np){
                    aa->dist=min(aa->dist,a->dist+1);
                    if(aa->flag!=flg){ q.push(aa); aa->flag=flg; }
                }else{ break; }
            }
            q.pop();
        }
		ans = np;
        flg=(flg+1)%2;
    }

};

core ta;

#endif