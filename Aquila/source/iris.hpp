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
#include <util/delay.h>
//#include "core.hpp" // it's my code

void make_nodes(){
    if(!ta.r_now()->ac){
		rep(i,4){
			if(check_ping(i)>1 && ta.ac_next(i,1)==np)ta.ap_node(ta.r_now(),i);
		}
		ta.r_now()->ac=true;
    }
}

void nachylenie(){

}

void move(int num){//num::0:turn_l(90deg),1:go_st,2:turn_r(90deg),3:back(turn),4:back(usiro)
	switch(num){
		case 0:
			motor::move(9);
			ta.turn_l();
			break;
		case 1:
			motor::move(0);
			ta.go_st();
			break;
		case 2:
			motor::move(8);
			ta.turn_r();
			break;
		case 3:
			motor::move(9);
			motor::fix_position();
			motor::move(9);
			motor::fix_position();
			motor::move(0);
			ta.turn_r();
			ta.turn_r();
			ta.go_st();
			break;
        case 4:
			motor::move(4);
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
	make_nodes();
}
void move_n(node* n){//move to neighborhood((node*)n)
    if(n!=np){
        rep(i,4)if(ta.ac_next(i,1)==n){
	        move(i);
			break;
		}
    }
}


void move_toa(node* a){//move to (node*)a
	ta.clear_dist();
	ta.bfs(a,ta.r_now());
	bl fg;
	while(ta.r_now()->dist!=0){
		fg = false;
		rep(i,4){
			if(!fg&&ta.ac_next(i,1)->dist<ta.r_now()->dist){ move_n(ta.r_now()); fg=true; }
		}
	}
	ta.clear_dist();
}

void stack_dfs(){
	ta.stk.push(ta.r_start());
	ta.r_start()->color=color::gray;
	make_nodes();
	bl fg;
	while(!ta.stk.empty()){
		fg=false;
		while(!fg){
			if(ta.stk.top()->color!=color::black){
				move_toa(ta.stk.top());
				fg=true;
			}else{ ta.stk.pop(); }	
		}
		rep(i,4){
			if(ta.ac_next(i,1)!=np&&ta.ac_next(i,1)->color==color::white){
				ta.stk.push(ta.ac_next(i,1));
				ta.ac_next(i,1)->color=color::gray;
			}
		}
	}
}

#endif
