/*
 * action.cpp
 *
 * Created: 2018/08/23 13:56:34
 *  Author: shun2
 */ 
#include "action.hpp"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include "xmega_twi.hpp"
#include "initializing.hpp"
#include "ui_control.hpp"

uint16_t Servo_int_num=1;
uint16_t Servo_count=110;
void init_Servo(){
	TCF1.CTRLA=TC_CLKSEL_DIV256_gc; //1count:0.000008s  //250count:2ms
	TCF1.INTCTRLA=TC_OVFINTLVL_HI_gc;
	PMIC.CTRL=PMIC_HILVLEN_bm;
	TCF1.PER=1;
}


ISR(TCF1_OVF_vect){
	if(Servo_int_num>=1000){
		Servo_int_num=0;
	}
	if(Servo_int_num<=Servo_count){
		PORTK.OUTCLR = PIN2_bm;
	}
	if(Servo_int_num>=Servo_count){
		PORTK.OUTSET = PIN2_bm;
	}
	Servo_int_num+=1;
	TCF1.CNT=0;
}

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
	PORTK.OUTCLR = PIN0_bm|PIN1_bm|PIN2_bm|PIN3_bm;
	led(Redled,0);
	led(Blueled,0);
	led(Greenled,0);
	error_led(1,1);
	for (int i = 0; i<=8; i++){
	error_led(1,3);
	error_led(2,4);
	buzzer();
	_delay_ms(300);
	}
	error_led(1,0);
	error_led(2,0);
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
