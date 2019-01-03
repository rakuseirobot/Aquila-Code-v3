/*
 * iris.cpp
 *
 * Created: 2018/12/31 12:39:15
 *  Author: shun2
 */ 


#include "iris.hpp"
#include "initializing.hpp"
#include "color_control.hpp"
#include "motor_control.hpp"
#include "lcd_control.hpp"
#include "ping_control.hpp"
#include "mv_control.hpp"


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

void nachylenie(){
	if(ta.r_now()->type==v::kaidan){
		rep(i,4){
			if(ta.r_now()->next[i]->type==v::kaidan){
				ta.w_now(ta.r_now()->next[i]->next[0]);
				motor::notify_long_acc();
				lcd_clear();
				lcd_putstr(LCD1_TWI,"saka!");
				break;
			}
		}
	}else if(motor::notify_long_acc()==2){
		node* u = ta.r_now();
			u->type=v::kaidan;
			node* t = ta.mall.make(100,100,u->z+1,(ta.r_flg()+1)%2);
			ta.cn_graph(u,t);
			ta.w_now(t);
			ta.ap_node(ta.r_now(),v::front);
			ta.w_now(ta.ac_next(v::front,1));
			ta.r_now()->color=color::gray;
			ta.stk.push(ta.r_now());
			for(int j=1;j>=0;j--){
				rep(i,4){
					if(check_ping(i)>j+1)if(ta.ac_next(i,1)!=np&&ta.ac_next(i,1)->color==color::white&&ta.ck_conect(ta.r_now(),ta.ac_next(i,1))){
						ta.stk.push(ta.ac_next(i,1));
						ta.ac_next(i,1)->color=color::gray;
					}
				}
			}
			lcd_clear();
			lcd_putstr(LCD1_TWI,"nac");
			//???????????
	}else if(motor::notify_long_acc()==1){
		node* u = ta.r_now();
			u->type=v::kaidan;
			node* t = ta.mall.make(100,100,u->z-1,(ta.r_flg()+1)%2);
			ta.cn_graph(u,t);
			//???????????
			ta.w_now(t);
			ta.ap_node(ta.r_now(),v::front);
	}else{ /*error*/ }
	make_nodes();
}

void move(int num){//num::0:turn_l(90deg)+go_st,1:go_st,2:turn_r(90deg)+go_st,4:back(turn),3:back(usiro)
	switch(num){
		case 0:

			motor::fix_position();
			ta.turn_l();
			ta.go_st();
			//hhh.type=ta.r_now()->type;
			motor::move(3);
			motor::move(0);
			motor::fix_position();
			break;
		case 1:
			ta.go_st();
			//hhh.type=ta.r_now()->type;
			motor::move(0);
			motor::fix_position();
			break;
		case 2:
			motor::fix_position();
			ta.turn_r();
			//hhh.type=ta.r_now()->type;
			motor::move(2);
			ta.go_st();
			motor::move(0);
			motor::fix_position();
			break;
		case 4:
			ta.turn_r();
			motor::move(9);
			motor::fix_position();
			ta.turn_r();
			motor::move(9);
			motor::fix_position();
			//hhh.type=ta.r_now()->type;
			ta.go_st();
			motor::move(0);
			motor::fix_position();
			break;
        case 3:
			ta.turn_l();
			ta.turn_l();
			ta.go_st();
			ta.turn_l();
			ta.turn_l();
			//hhh.type=ta.r_now()->type;
			motor::move(4);
			motor::fix_position();
			break;
		default:
			break;
	}
	if(ta.r_now()->type==v::unknown){ta.r_now()->type = v::normal;}
	if(ta.r_now()!=ta.r_start())ta.r_now()->color=color::black;
	//if(hhh.key==1){ta.r_now()->type=v::r_kit;}
	//hhh.key=0;
	black_tile();
	nachylenie();
	make_nodes();
}
void move_n(node* n){//move to neighborhood((node*)n)
    //lcd_clear();
	//lcd_putstr(LCD1_TWI,"m");
	if(n!=np){
		//lcd_putstr(LCD1_TWI,"n");
		serial.string("m_n-a");
        rep(i,4)if(ta.ac_next(i,1)==n && ta.ck_conect(ta.r_now(),ta.ac_next(i,1)) && ta.ac_next(i,1)->type!=v::black){
	        move(i);
			lcd_clear();
			lcd_putstr(LCD1_TWI,"m_n");
			serial.string("m_n-b");
			break;
		}
    }
}


void move_toa(node* a){//move to (node*)a
	lcd_clear();
	ta.clear_dist();
	ta.bfs(a,ta.r_now());
	bl fg;
	serial.string("m_a-a");
	while(ta.r_now()!=a||a->type==v::black||a->type==v::slope){
		fg = false;
		rep(i,4){
			serial.string("m_a-b");
			if(!fg && ta.ac_next(i,1)!=np && ta.ck_conect(ta.r_now(),ta.ac_next(i,1)) && ta.ac_next(i,1)->dist<ta.r_now()->dist && ta.ac_next(i,1)->type!=v::black){ move_n(ta.ac_next(i,1)); fg=true; }
		}
	}
	serial.string("m_a-c");
	ta.clear_dist();
	lcd_clear();
	lcd_putstr(LCD1_TWI,"m_a");
}

void stack_dfs(){
	ta.stk.push(ta.r_start());
	ta.r_start()->color=color::gray;
	make_nodes();
	bl fg;
	while(!ta.stk.empty()){
		if(ta.r_now()!=ta.r_start())ta.r_now()->color=color::black;
		for(int j=1;j>=0;j--){
			for(int i=3;i>=0;i--){
				serial.string("k");
				if(check_ping(i)>j+1)if(ta.ac_next(i,1)!=np&&ta.ac_next(i,1)->color==color::white&&ta.ck_conect(ta.r_now(),ta.ac_next(i,1))){
					serial.string("a");
					ta.stk.push(ta.ac_next(i,1));
					ta.ac_next(i,1)->color=color::gray;
				}
			}
		}
		serial.string("b");
		lcd_clear();
		lcd_putstr(LCD1_TWI,"dfs");
		fg=false;
		while(!fg){
			serial.string("c");
			if(ta.stk.top()->color!=color::black && !ta.stk.empty() && ta.stk.top()!=np){
				serial.string("d");
				move_toa(ta.stk.top());
				lcd_clear();
				lcd_putstr(LCD1_TWI,"dfs2");
				fg=true;
				serial.string("e");
			}else{ ta.stk.pop(); }	
		}
		serial.string("end");
	}
}