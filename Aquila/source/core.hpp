#ifndef CORE_HPP_
#define CORE_HPP_
/*
 * core.hpp
 * Created: 2018/12/20 12:00:00
 *  Author: Emile
 */ 
#include "data_structure.hpp"

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
    AVLtree at; //use ans,flg (also in bfs)
public:
	nodes mall;
	stack stk;
    core();
    void turn_r();
    void turn_l();
	void w_now(node* u);
	int r_flg();
    node* r_now();
	node* r_start();
    void dfs(node* t,int x,int y,int z,int depth);
    node* find(int x,int y,int z);
    void cn_graph(node* v, node* u);
    void cn_tree(node* par,node* v);
    void ap_node(node* t,int dire);
    node* ac_next(node* t,int now_dir,int dire,int dist);
    node* ac_next(int dire,int dist);
	bl ck_conect(node* s,node* t);
    void go_st();
    void cl_dist(node* t,int d);
    void clear_dist();
    void bfs(node* s,node* t);
};
extern core ta;
#endif