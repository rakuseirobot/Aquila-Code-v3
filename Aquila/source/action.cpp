/*
 * action.cpp
 *
 * Created: 2018/08/23 13:56:34
 *  Author: shun2
 */ 
#include "action.hpp"


void throw_kit(void){
	for(int i=0;i<=40;i++){
		PORTK.OUTSET = PIN0_bm;
		PORTK.OUTCLR = PIN1_bm|PIN2_bm|PIN3_bm;
		_delay_ms(2);
		PORTK.OUTSET = PIN1_bm;
		PORTK.OUTCLR = PIN0_bm|PIN2_bm|PIN3_bm;
		_delay_ms(2);
		PORTK.OUTSET = PIN2_bm;
		PORTK.OUTCLR = PIN0_bm|PIN1_bm|PIN3_bm;
		_delay_ms(2);
		PORTK.OUTSET = PIN3_bm;
		PORTK.OUTCLR = PIN0_bm|PIN1_bm|PIN2_bm;
		_delay_ms(2);
	}
}

void finded_victim(uint8_t co){//How many kits does victim need?
	led(Redled,1);
	led(Blueled,1);
	led(Greenled,1);	
	for(int i=0;i<=40*co;i++){
		PORTK.OUTSET = PIN0_bm;
		PORTK.OUTCLR = PIN1_bm|PIN2_bm|PIN3_bm;
		_delay_ms(2);
		PORTK.OUTSET = PIN1_bm;
		PORTK.OUTCLR = PIN0_bm|PIN2_bm|PIN3_bm;
		_delay_ms(2);
		PORTK.OUTSET = PIN2_bm;
		PORTK.OUTCLR = PIN0_bm|PIN1_bm|PIN3_bm;
		_delay_ms(2);
		PORTK.OUTSET = PIN3_bm;
		PORTK.OUTCLR = PIN0_bm|PIN1_bm|PIN2_bm;
		_delay_ms(2);
	}
	led(Redled,0);
	led(Blueled,0);
	led(Greenled,0);
	PORTK.OUTCLR = PIN0_bm|PIN1_bm|PIN2_bm|PIN3_bm;
}





static uint16_t EEMEM rectime = 0x1000;
bool record_time(unsigned int time){
	eeprom_busy_wait();
	eeprom_write_word(&rectime,time);
	return true;
}
uint16_t read_time(void){
	eeprom_busy_wait();
	return eeprom_read_word(&rectime);
}
