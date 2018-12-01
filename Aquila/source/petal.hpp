/*
 * petal.cpp
 *
 * Created: 2018/08/25 0:59:49
 *  Author: TOMOKI
 */ 
 #ifndef PETAL_HPP_
 #define PETAL_HPP_

 #include "avr/io.h"
 //#include "core.hpp"
 #include "test_core.hpp"
 #include "initializing.hpp"
 #include "ui_control.hpp"
 #include "lcd_control.hpp"
 #include "motor_control.hpp"
 #include "action.hpp"
 #include "color_control.hpp"
 #include "sermo_control.hpp"
 #include "serial.hpp" //荳逡ｪ譛蠕後↓・・ｼ・#include <avr/eeprom.h>
 #include "mv_control.hpp"
 #include "util/delay.h"
 //usart serial(&USARTC0,&PORTC);

core ta;

void for_w_w(int direc){
	switch(check_ping(direc)){
		case 1: //横に壁
			ta.w_wall(direc,v::wall);
			break;
		case 2: //隣のマスに壁がある
			ta.append_node(direc,1);
			ta.w_wall(direc,v::nowall);
			ta.w_wall(ta.ac_next(direc,1),direc,v::wall);//隣のマスに壁をwrite
			break;
		case 3: //2つ隣のマスに壁がある
			ta.append_node(direc,1);
			ta.append_node(ta.ac_next(direc,1),direc,ta.r_dir(),1);//隣の隣のマス作成
			ta.w_wall(direc,v::nowall);
			ta.w_wall(ta.ac_next(direc,1),direc,v::nowall);
			break;
		case -4: //3つ隣のマスに壁がある
			ta.append_node(direc,1);
			ta.append_node(ta.ac_next(direc,1),direc,ta.r_dir(),1);//隣の隣のマス作成
			ta.append_node(ta.ac_next(direc,2),direc,ta.r_dir(),1);//隣の隣の隣のマス作成
			ta.w_wall(direc,v::nowall);
			ta.w_wall(ta.ac_next(direc,1),direc,v::nowall);
			ta.w_wall(ta.ac_next(direc,2),direc,v::nowall);
			ta.w_wall(ta.ac_next(direc,3),direc,v::wall);
			break;
		case -5: //4つ隣のマスに壁がある,実はcase 2:と全く同じ。
			ta.append_node(direc,1);
			ta.w_wall(direc,v::nowall);
			ta.w_wall(ta.ac_next(direc,1),direc,v::wall);//隣のマスに壁をwrite
			break;
		case 0:
		//case 2:
		//case 3:
		case 4:
		case 5:
			ta.append_node(direc,1);
			ta.w_wall(direc,v::nowall);
			break;
		
	}
}

void write_walls(){
	for_w_w(v::left);
	for_w_w(v::front);
	for_w_w(v::right);
	for_w_w(v::back);
}

void ondo(){
	if(ta.r_now()->type==v::hisai||ta.r_now()->type==v::r_kit){
		
	}else if(check_sermo()==0 || check_sermo()==1 || check_sermo()==2){
		ta.r_now()->type=v::hisai;
		throw_kit();
		ta.r_now()->type=v::r_kit;
		finded_victim(1);//?????????error
	}else{
		
	}
}

void blacktile(){
	if(color_check()==1){
		write_walls();
		ta.r_now()->type=v::black;
		motor::move(v::back);
	}else{/*no action*/}	
}

void move(int num){//num::0:turn_l(90deg),1:go_st,2:turn_r(90deg),3:back(turn),4:back(usiro)
	switch(num){
		case 0:
			mv_cap(1,true);
			mv_cap(2,true);
			mv_cap(3,true);
			motor::move(9);
			ta.turn_l();
			break;
		case 1:
			mv_cap(1,true);
			mv_cap(2,true);
			mv_cap(3,true);
			motor::move(0);
			ta.go_st();
			break;
		case 4:
			mv_cap(1,true);
			mv_cap(2,true);
			mv_cap(3,true);
			motor::move(4);
			ta.turn_l();
			ta.turn_l();
			ta.go_st();
			ta.turn_l();
			ta.turn_l();
			break;
		case 2:
			mv_cap(1,true);
			mv_cap(2,true);
			mv_cap(3,true);
			motor::move(8);
			ta.turn_r();
			break;
		case 3:
			mv_cap(1,true);
			mv_cap(2,true);
			mv_cap(3,true);
			motor::move(9);
			motor::fix_position();
			motor::move(9);
			motor::fix_position();
			motor::move(0);
			ta.turn_r();
			ta.turn_r();
			ta.go_st();
			break;
		default:
			break;
	}
	if(ta.r_now()->type==v::unknown){ta.r_now()->type = v::normal;}
	if(ta.r_now()->type==v::hisai){mv_cap(1,false);mv_cap(2,false);mv_cap(3,false);}
	if(k_r_read()==6 && ta.r_now()->type==v::normal){ ta.r_now()->type=v::hisai; }//ondo
	k_r_write(0);
	serial.putdec(ta.r_now()->x);
	serial.string("\n\r");
	serial.putdec(ta.r_now()->y);
	serial.string("\n\r");
	serial.putdec(ta.r_now()->depth);
	serial.string("\n\r");
	serial.string("\n\r");
	if(color_check()==1){//kuro
		ta.r_now()->type=v::black;
		motor::move(4);
		ta.turn_l();
		ta.turn_l();
		ta.go_st();
		ta.turn_l();
		ta.turn_l();	
	}
	led(Greenled,1);
	motor::fix_position();
	led(Blueled,1);
	lcd_clear();
}

void nachylenie(){//多分モーター回した後に入れるべきやつ。//色々改善の余地がある。
	if(motor::notify_long_ex()==1){ //F():坂判定機,今坂の上にいるかどうかがわかる。
		ta.r_now()->type=v::slope;
		node*t=ta.r_now()->back[0];
		ta.move_to(t->x,t->y,t->z+1);
	}else if(ta.r_now()->type==v::slope){
		node* t=ta.r_now()->back[0];
		ta.w_now(t);
	}else{
		//no action
	}
}

node* check_node(node* x){
	node* ans = np;
	node* tmp= x;
	while(){

	}
	return ans;
}

bool movetoa(node* a){//move to A. If A is neighbor of now_node , move to A.
	//lcd_putstr(LCD1_TWI,"MtoA");
	if(a==np)return false;
	if(ta.ac_next(v::left,1)==a && ta.r_wall(v::left)!= v::wall){
		lcd_clear();
		move(v::left);
		move(v::front);
		lcd_clear();
		return true;
	}else if(ta.ac_next(v::front,1)==a && ta.r_wall(v::front) != v::wall ){
		move(v::front);
		lcd_clear();
		return true;
	}else if(ta.ac_next(v::right,1)==a && ta.r_wall(v::right) != v::wall ){
		move(v::right);
		move(v::front);
		lcd_clear();
		return true;
	}else if(ta.ac_next(v::back,1)==a && ta.r_wall(v::back) != v::wall ){
		move(4);
		lcd_clear();
		return true;
	}else{
		//error
		return false;
	}
}

void goback(node *saki){
	if(ta.r_now()==saki){
		//end
	}else{
		for(int i=0;i<4;i++){
			if(ta.ac_next(i,1)->hosu +1 == ta.r_now()->hosu){	movetoa(ta.ac_next(i,1)); goback(saki);  break;  }
		}
	}
}

void go_bfs(node* x){
	lcd_clear();
	ta.bfs_type(x,v::damy);
	lcd_putstr(LCD1_TWI,"bfs");
	lcd_putdec(LCD1_TWI,ta.r_now()->hosu);
	node* tmp=np;
	while(ta.r_now()->hosu!=0){
		tmp = ta.r_now();
		for(int i=0;i<4;i++){
			if(tmp->next[i]!=np && tmp->next[i]->hosu < tmp->hosu){
				movetoa(tmp->next[i]);
				break;
			}
		}
	}
}

void real_dfs(node* t,node* s){
	lcd_clear();
	write_walls();
	lcd_putstr(LCD1_TWI,"dfs");
	node* a = ta.ac_next(s,ta.r_dir(),v::left,1);
	node* b = ta.ac_next(s,ta.r_dir(),v::front,1);
	node* c = ta.ac_next(s,ta.r_dir(),v::right,1);
	node* d = ta.ac_next(s,ta.r_dir(),v::back,1);
	lcd_clear();
	lcd_putdec(LCD1_TWI,ta.r_vnum());
	if(a!=np && a->type==v::unknown){ if(movetoa(a))real_dfs(s,ta.r_now()); }
	if(b!=np && b->type==v::unknown){ if(movetoa(b))real_dfs(s,ta.r_now()); }
	if(c!=np && c->type==v::unknown){ if(movetoa(c))real_dfs(s,ta.r_now()); }
	if(d!=np && d->type==v::unknown){ if(movetoa(d))real_dfs(s,ta.r_now()); }
	lcd_clear();
	write_walls();
	//if(s==ta.r_now() && t!=np)go_bfs(check_node(t));
	go_bfs(check_node(ta.r_now()));
	lcd_clear();
	//if(t!=np)movetoa(t);
	//if(t!=np)gobacktoa(t);
}

void stack_dfs(node* t){
	lcd_clear();
	lcd_putstr(LCD1_TWI,"s_dfs");
	write_walls();
	st.push(t);
	for(int i=0;i<4;i++)if(ta.ac_next(i,1)!=np && ta.ac_next(i,1)->type==v::unknown && ta.ac_next(i,1)->color==0){ st.push(ta.ac_next(i,1)); ta.ac_next(i,1)->color=1;}
	while(!st.empty()){
		go_bfs(check_node(st.top()));
		st.top()->color=2;
		st.pop();
		for(int i=0;i<4;i++)if(ta.ac_next(i,1)!=np && ta.ac_next(i,1)->type==v::unknown && ta.ac_next(i,1)->color==0){ st.push(ta.ac_next(i,1)); ta.ac_next(i,1)->color=1;}
		write_walls();
	}
}

#endif