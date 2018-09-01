/*
 * main.cpp
 *
 * Created: 2018/08/25 0:59:49
 *  Author: TOMOKI
 */ 

#include "source/petal.hpp"
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
	while(1){
		write_walls();
		hidarite();
	}
    return 0;
}