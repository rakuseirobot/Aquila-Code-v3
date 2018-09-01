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

core ta;

int for_write_walls1(int num){
	if(for_cp(num)==1){
		return v::wall;
	}else{
		return v::nowall;
	}
}

void for_write_walls2(int direc){
	if(check_ping2(direc)==2){
		ta.w_wall(direc,v::nowall);
		ta.append_node(direc,1);
		ta.w_wall(ta.ac_next(direc,1),3-direc,v::nowall);
		ta.w_wall(ta.ac_next(direc,1),direc,v::wall);
		ta.append_node(ta.ac_next(direc,1),direc,ta.r_dir(),1);
		ta.w_wall(ta.ac_next(direc,2),3-direc,v::wall);
	}else if(check_ping2(direc)==3){
		ta.w_wall(direc,v::nowall);
		ta.append_node(direc,1);
		ta.w_wall(ta.ac_next(direc,1),3-direc,v::nowall);
		ta.w_wall(ta.ac_next(direc,1),direc,v::nowall);
		ta.append_node(ta.ac_next(direc,1),direc,ta.r_dir(),1);
		ta.w_wall(ta.ac_next(direc,2),3-direc,v::nowall);
		ta.w_wall(ta.ac_next(direc,2),direc,v::wall);
		ta.append_node(ta.ac_next(direc,2),direc,ta.r_dir(),1);
		ta.w_wall(ta.ac_next(direc,3),3-direc,v::wall);
	}else if(check_ping2(direc)==4){
		 
	}else{
		 
	}
}

void write_walls(){
	ta.w_wall(v::left,for_write_walls1(v::left));
	ta.w_wall(ta.ac_next(v::left,1),v::right,for_write_walls1(v::left));
	ta.w_wall(v::front,for_write_walls1(v::front));
	ta.w_wall(ta.ac_next(v::front,1),v::back,for_write_walls1(v::front));
	ta.w_wall(v::right,for_write_walls1(v::right));
	ta.w_wall(ta.ac_next(v::right,1),v::left,for_write_walls1(v::right));
	ta.w_wall(v::back,for_write_walls1(v::back));
	ta.w_wall(ta.ac_next(v::back,1),v::front,for_write_walls1(v::back));
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
		motor::move(4);
	}else{/*no action*/}	
}

void saka(){
	 //実際に坂を下る関数。
	 //map系の処理はしなくてよい。
}

bool saka_check(){//未定。
	 bool t;
	 return t;
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
			motor::move(9);
			motor::move(0);
			ta.turn_r();
			ta.turn_r();
			ta.go_st();
			break;
		default:
			break;
	}
	if(ta.r_now()->type==v::unknown){ta.r_now()->type = v::normal;}
	write_walls();
	//motor::fix_position();
	blacktile();
	ondo();
}

void nachylenie(){//多分モーター回した後に入れるべきやつ。//色々改善の余地がある。
	if(motor::notify_long_ex()==1){ //F():坂判定機,今坂の上にいるかどうかがわかる。
		ta.r_now()->type=v::slope;
		node*t=ta.r_now()->back[0];
		//saka();
		ta.move_to(t->x,t->y,t->z+1);//???
		//ta.r_now()->type=v::slope;
	}else if(ta.r_now()->type==v::slope){
		saka();
		//node* t=ta.r_now()->back[0]->back[0];
		node* t=ta.r_now()->back[0];
		ta.w_now(t);
	}else{
		//no action
	}
}



namespace for_func{
int check_neighbor(node* t,node* s){//tの周りにいるsについて、今のdirから見てどちら側にいるかを返す関数。(node*,node*)->direction
	int x=s->x - t->x;
	int y=s->y - t->y;
	int z=s->z - t->z;
	int ans=-1;//-1==error
	if(x!=0 && z==0 && y==0){//まずは基準の方向から考えた時のdirを返す。その後に今のdirと比較して相対化。
		if(x==1){
			ans = v::back;
		}else if(x==-1){
			ans =v::front;
		}else{/*error*/}
	}else if(x==0 && y!=0 && z==0){
		if(y==1){
			ans =v::right;
		}else if(y==-1){
			ans =v::left;
		}else{/*error*/}
	}else{/*error*/}
	return ans;//絶対的な方向を返す。相対的な方向に直してね。
}
				 
int change_relatively(int now_dir,int di){//diは絶対的な方向,now_dirは今の方向
	int num = now_dir - di;
	if(num == 0){
		return v::front;
	}else if(num == 1||num ==-3){
		return v::left;
	}else if(num == 2||num==-2){
		return v::back;
	}else if(num ==3||num == -1){
		return v::right;
	}else{/*error*/}
}
					 
int _change_relatively(int now_dir,int dir){/*now_dir::今のdir,dir::direction from now_dir,ans::絶対的なdir,,,,(now_dir,dir)->ans*/}
};//end of for_func

void float_killer(){
	node* a=ta.r_now();
	if(ta.count_walls(a)==3 || ta.count_walls(a)<=1 ||saka_check()==true){
						 //終了条件
	}else{
	if(ta.r_wall(a,v::left)==v::nowall){
		move(v::left);
		move(v::front);
	}else if(ta.r_wall(a,v::front)==v::nowall){
		move(v::front);
	}else if(ta.r_wall(a,v::right)==v::nowall){
		move(v::right);
		move(v::front);
	}else{ /*error*/ }
		float_killer();
	}
	move(for_func::change_relatively(ta.r_dir(),for_func::check_neighbor(a,a->back[0])));
	move(v::right);
}

bool for_fk2(int n){if(ta.ac_next(v::right,n) ==np || ta.ac_next(v::right,n)->type==v::unknown){return true;}else{return false;}}

void float_killer2(){
	if(for_fk2(1)){
		if(for_fk2(2) && ta.ac_next(v::right,3)->type!=v::unknown){
			 //右2ますいって戻る
			move(v::right);				
			move(v::front);
			move(v::front);
			move(v::back);
	    	move(v::back);
			move(v::left);
		}else{
			if(ta.ac_next(v::right,2)->type!=v::unknown){
				//右1マス進んで、戻る
				move(v::right);
				move(v::front);
				move(v::back);
				move(v::left);
			}else{
				//今の所no action
				//一応ここの条件の所にfor_fk2(3)とか色々書いておけば3マス右にとかできるよ。
			}
		}
	}else{
		 //no action
	}
}

void hidarite(){
	if(ta.r_wall(v::left)==v::wall || ta.ac_next(v::left,1)->type==v::black){
		if(ta.r_wall(v::front)==v::wall || ta.ac_next(v::left,1)->type==v::black){
			if(ta.r_wall(v::right)==v::wall || ta.ac_next(v::right,1)->type==v::black){
				move(0);
				move(0);
				move(1);
			}else{
				move(2);
				//move(0);
				move(1);
				//右へ
			}
		}else{
			move(1);
			//前へ
		}
	}else{
		move(0);
		move(1);
		//左へ
	}
 //壁書き込み。
}

void go_home(){
	node* t = ta.r_now();
	if(t==ta.r_start()){
		
	}else{
		int num = ta.ac_next(v::left,1)->depth - t->depth;
		int direc = v::left; 
		for(int i=1;i<4;i++){
			if( (ta.ac_next(i,1)->depth - t->depth) > num){ num = ta.ac_next(i,1)->depth - t->depth; direc = i; }
		}
		switch(direc){
			case 0:
				//go left block
				move(0);
				move(1);
				go_home();
				break;
			case 1:
				//go st
				move(1);
				go_home();
				break;
			case 2:
				//go right block
				move(2);
				move(0);
				go_home();
				break;
			case 3:
				move(3);
				go_home();
				break;
			default:
				
				break;
		}
	}
}



