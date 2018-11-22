/*
 * main.cpp
 *
 * Created: 2018/08/25 0:59:49
 *  Author: TOMOKI
 */ 

#include "source/petal.hpp"
#include "source/mv_control.hpp"
usart serial(&USARTC0,&PORTC);
#include "source/gyro_control.hpp"

int main(){	
	init_all();
	//init_mv();
	//finded_victim(1);
	_delay_ms(200);
	lcd_putstr(LCD1_TWI,"Hello");
	motor::wait();
	lcd_clear();
	lcd_putstr(LCD1_TWI,"Ready!");
	lcd_clear();
	serial.string("wake_up\n");
// 	while(1){
// 		lcd_putdec(LCD1_TWI,check_ping(v::back));
// 		_delay_ms(500);
// 		lcd_clear();
// 	}
	real_dfs(np,ta.r_start());
	lcd_putstr(LCD1_TWI,"THE_END");
	return 0;
}