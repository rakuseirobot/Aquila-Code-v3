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
#include "source/initializing.hpp"
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
	buzzer();
	/*while(1){
		motor::move(0);
		motor::fix_angle();
		motor::move(0);
		motor::turn_fix();
		motor::gb_fix();
		motor::turn_fix();
	}
	while(1){
		debug::color();
		debug::ping_d(7,false);
		debug::jy(false);
		_delay_ms(10);
	}
	while(1){
		mv_cap(1,true);
		motor::move(1);
		led(Redled,1);
		_delay_ms(1000);
		led(Redled,0);
	}
	*/
	//gyro_cali();
	stack_dfs();
	lcd_clear();
	lcd_putstr(LCD1_TWI,"END");
	return 0;
}