/*
 * petal.cpp
 *
 * Created: 2018/08/25 0:59:49
 *  Author: TOMOKI
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
 #include "serial.hpp" //荳逡ｪ譛蠕後↓・・ｼ・#include <avr/eeprom.h>
 #include <util/delay.h>

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
			motor::move(9);
			ta.turn_l();
			break;
		case 1:
			motor::move(0);
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
		default:
			break;
	}
	if(ta.r_now()->type==v::unknown){ta.r_now()->type = v::normal;}
	//write_walls();
	motor::fix_position();
	//blacktile();//not needed;
	ondo();
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

bool movetoa(node* a){//move to A. If A is neighbor of now_node , move to A.
	lcd_putstr(LCD1_TWI,"MtoA");
	if(ta.ac_next(v::left,1)==a && ta.r_wall(v::left)!= v::wall){
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
		move(v::back);
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


void gobacktoa(node* ima){//imaから前の分岐点まで戻るx
	if(ta.count_next(ima)!=0){
		//no action
	}else{
		ta.clear_hosu();
		node* bak=ima;
		while(1){
			if(bak==ta.r_start() ){
				break;//end
			}else if(ta.count_next(bak)==0 ){
				bak = bak->back[0];	
			}else if(ta.count_next(bak)!=0 ){
				break;//finded
			}else{
				//error
			}
		}
		ta.bfs_type(bak,100);
		goback(bak);
		ta.clear_hosu();
	}
}

void real_dfs(node* t,node* s){
	lcd_clear();
	write_walls();
	lcd_putstr(LCD1_TWI,"dfs");
	if(false){//blacktile
		lcd_putstr(LCD1_TWI,"black");
		movetoa(t);
	}
	node* a = ta.ac_next(s,ta.r_dir(),v::left,1);
	node* b = ta.ac_next(s,ta.r_dir(),v::front,1);
	node* c = ta.ac_next(s,ta.r_dir(),v::right,1);
	node* d = ta.ac_next(s,ta.r_dir(),v::back,1);
	if(a!=np && a->type==v::unknown){ if(movetoa(a))real_dfs(s,ta.r_now()); }
	if(b!=np && b->type==v::unknown){ if(movetoa(b))real_dfs(s,ta.r_now()); }
	if(c!=np && c->type==v::unknown){ if(movetoa(c))real_dfs(s,ta.r_now()); }
	if(d!=np && d->type==v::unknown){ if(movetoa(d))real_dfs(s,ta.r_now()); }
	lcd_clear();
	write_walls();
	if(t!=np)movetoa(t);
	//if(t!=np)gobacktoa(t);
}


