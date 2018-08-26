/*
 * main.cpp
 *
 * Created: 2018/08/25 1:27:46
 *  Author: TOMOKI
 */ 

#include "source/petal.hpp"
usart serial(&USARTC0,&PORTC);
void print_num(int num){serial.putdec(num);serial.string("\n");}
void print_num(int num,int num2,int num3){
	serial.putdec(num);
	serial.string(" , ");
	serial.putdec(num2);
	serial.string(" , ");
	serial.putdec(num3);
	serial.string("\n");
}


int main(){	
	init_all();
	_delay_ms(200);
	lcd_putstr(LCD1_TWI,"Hello");
	motor::wait();
	lcd_clear();
	lcd_putstr(LCD1_TWI,"Ready!");
	//lcd_clear();
	serial.string("wake_up\n");
	////////////////////////////////////////////////
	/*while(1){
		hidarite(); //左手法
		nachylenie();  //坂
		float_killer();//再帰
		float_killer2();//前と同じ原理の奴。
	}*/
	return 0;
}