/*
 * main.cpp
 *
 * Created: 2018/08/25 0:59:49
 *  Author: TOMOKI
 */ 

#include "source/petal.hpp"
#include "source/gyro_control.hpp"
#include "source/mv_control.hpp"
usart serial(&USARTC0,&PORTC);

int main(){
    init_all();
    _delay_ms(200);
    lcd_putstr(LCD1_TWI,"Hello");
    motor::wait();
    lcd_clear();
    lcd_putstr(LCD1_TWI,"Ready!");
    //lcd_clear();
    serial.string("wake_up\n");
 	write_walls();
	lcd_putdec(LCD1_TWI,check_ping(v::left));
	_delay_ms(1000);
	lcd_clear();
 	real_dfs(np,ta.r_now());
	return 0;	
}