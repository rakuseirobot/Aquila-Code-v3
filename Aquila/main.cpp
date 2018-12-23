/*
 * main.cpp
 *
 * Created: 2018/08/25 0:59:49
 *  Author: TOMOKI
 */ 

#include "source/iris.hpp"
#include "source/mv_control.hpp"
usart serial(&USARTC0,&PORTC);
#include "source/gyro_control.hpp"
#include "source/vl53lox.hpp"
#include <string.h>

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
	uint8_t s = 0;
	//motor::move(1);
	buzzer();
	//gyro_cali();
	stack_dfs();	
	lcd_clear();
	lcd_putstr(LCD1_TWI,"END");
	return 0;
}