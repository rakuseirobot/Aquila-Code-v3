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
	init_mv();
	//finded_victim(1);
	_delay_ms(200);
	lcd_putstr(LCD1_TWI,"Hello");
	motor::wait();
	lcd_clear();
	lcd_putstr(LCD1_TWI,"Ready!");
	lcd_clear();
	serial.string("wake_up\n");
	ta.append_node(v::front,1);
	ta.go_st();
	ta.append_node(v::front,1);
	ta.go_st();
	ta.append_node(v::left,1);
	ta.append_node(v::back,1);
	ta.find(1,1,1);
	lcd_putdec(LCD1_TWI,ta.r_vnum());
/*
	_delay_ms(1000);
	buzzer();
	real_dfs(np,ta.r_start());
	//st.push(ta.r_now());
	//stack_dfs(ta.r_now());
	lcd_clear();
	lcd_putstr(LCD1_TWI,"THE_END");
*/	return 0;
}