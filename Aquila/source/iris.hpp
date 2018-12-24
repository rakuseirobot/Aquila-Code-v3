#ifndef IRIS_HPP_
#define IRIS_HPP_
/*
 * iris.hpp
 *
 * Created: 2018/12/20 12:00:00
 *  Author: Emile
 */ 
#include <avr/io.h>
#include "core.hpp"
#include "initializing.hpp"
#include "ui_control.hpp"
#include "lcd_control.hpp"
#include "motor_control.hpp"
#include "action.hpp"
#include "color_control.hpp"
#include "sermo_control.hpp"
#include "serial.hpp" 
#include "ping_control.hpp"
#include <util/delay.h>
//#include "core.hpp" // it's my code

void make_nodes(){
    if(!ta.r_now()->ac){
		rep(i,4){
			if(check_ping(i)>1){
				if(ta.ac_next(i,1)==np){
					ta.ap_node(ta.r_now(),i);
				}else{
					ta.cn_graph(ta.r_now(),ta.ac_next(i,1));
				}
			}
		}
		ta.r_now()->ac=true;
    }
}

void black_tile(){
	if(color_check()==1 || ta.r_now()->type==v::black){
		ta.r_now()->type=v::black;
		ta.r_now()->color=v::black;
		motor::move(4);
		motor::fix_position();
		ta.turn_l();
		ta.turn_l();
		ta.go_st();
		ta.turn_l();
		ta.turn_l();
	}
}
// 
// void nachylenie(){
// 	if(ta.r_now()->type==v::kaidan){
// 		rep(i,4){
// 			if(ta.r_now()->next[i]->type==v::kaidan){
// 				ta.w_now(ta.r_now()->next[i]);
// 				//ŠK’i‚Ì“®ì
// 				break;
// 			}
// 		}
// 	}else if(/*”»’èŠÖ”*/){
// 		node* u = ta.r_now();
// 		if(/*ãŒü‚«*/){
// 			u->type=v::kaidan;
// 			node* t = mall.make(100,100,u->z+1,(ta.r_flg()+1)%2);
// 			ta.cn_graph(u,t);
// 			//ŠK’i‚ðã‚é“®ì‚ð“ü‚ê‚é
// 			ta.w_now(t);
// 		}else if(/*‰ºŒü‚«*/){
// 			u->type=v::kaidan;
// 			node* t = mall.make(100,100,u->z-1,(ta.r_flg()+1)%2);
// 			ta.cn_graph(u,t);
// 			//ŠK’i‚ð‰º‚é“®ì‚ð“ü‚ê‚é
// 			ta.w_now(t);
// 		}else{ /*error*/ }
// 	}
// }

void move(int num){//num::0:turn_l(90deg)+go_st,1:go_st,2:turn_r(90deg)+go_st,4:back(turn),3:back(usiro)
	switch(num){
		case 0:
			motor::move(9);
			motor::fix_position();
			ta.turn_l();
			motor::move(0);
			motor::fix_position();
			ta.go_st();
			break;
		case 1:
			motor::move(0);
			motor::fix_position();
			ta.go_st();
			break;
		case 2:
			motor::move(8);
			motor::fix_position();
			ta.turn_r();
			motor::move(0);
			motor::fix_position();
			ta.go_st();
			break;
		case 4:
			motor::move(9);
			motor::fix_position();
			motor::move(9);
			motor::fix_position();
			motor::move(0);
			ta.turn_r();
			ta.turn_r();
			ta.go_st();
			break;
        case 3:
			motor::move(4);
			motor::fix_position();
			ta.turn_l();
			ta.turn_l();
			ta.go_st();
			ta.turn_l();
			ta.turn_l();
			break;
		default:
			break;
	}
	if(ta.r_now()->type==v::unknown){ta.r_now()->type = v::normal;}
	black_tile();
	make_nodes();
	if(ta.r_now()!=ta.r_start())ta.r_now()->color=color::black;
}
void move_n(node* n){//move to neighborhood((node*)n)
    //lcd_clear();
	//lcd_putstr(LCD1_TWI,"m");
	if(n!=np){
		//lcd_putstr(LCD1_TWI,"n");
        rep(i,4)if(ta.ac_next(i,1)==n && ta.ck_conect(ta.r_now(),ta.ac_next(i,1)) && ta.ac_next(i,1)->type!=v::black){
	        move(i);
			break;
		}
    }
}


void move_toa(node* a){//move to (node*)a
	lcd_clear();
	ta.clear_dist();
	ta.bfs(a,ta.r_now());
	bl fg;
	while(ta.r_now()!=a){
		fg = false;
		rep(i,4){
			if(!fg && ta.ac_next(i,1)!=np && ta.ck_conect(ta.r_now(),ta.ac_next(i,1)) && ta.ac_next(i,1)->dist<ta.r_now()->dist && ta.ac_next(i,1)->type!=v::black){ move_n(ta.ac_next(i,1)); fg=true; }
		}
	}
	ta.clear_dist();
	lcd_clear();
}

void stack_dfs(){
	ta.stk.push(ta.r_start());
	ta.r_start()->color=color::gray;
	make_nodes();
	bl fg;
	while(!ta.stk.empty()){
		if(ta.r_now()!=ta.r_start())ta.r_now()->color=color::black;
		for(int j=1;j>=0;j--){
			rep(i,4){
				if(check_ping(i)>j+1)if(ta.ac_next(i,1)!=np&&ta.ac_next(i,1)->color==color::white&&ta.ck_conect(ta.r_now(),ta.ac_next(i,1))){
					ta.stk.push(ta.ac_next(i,1));
					ta.ac_next(i,1)->color=color::gray;
				}
			}
		}
		fg=false;
		while(!fg){
			if(ta.stk.top()->color!=color::black && !ta.stk.empty() && ta.stk.top()!=np){
				move_toa(ta.stk.top());
				fg=true;
			}else{ ta.stk.pop(); }	
		}
	}
}

#endif
