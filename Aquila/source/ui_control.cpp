/*
 * ui_control.cpp
 *
 * Created: 2018/08/23 6:05:21
 *  Author: shun2
 */ 
#include "ui_control.hpp"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
void init_buzzer(void){
	TCD0.CTRLA=TC_CLKSEL_DIV1024_gc;
	TCD0.CTRLB=TC_WGMODE_NORMAL_gc;
	TCD0.INTCTRLA=TC_OVFINTLVL_MED_gc;
	TCD0.INTFLAGS=0b00000001;
}

ISR(TCD0_OVF_vect){
	
}
void buzzer(uint16_t t){
	while(1){
		for(int i=0;i<=1000-t;i++){
			PORTA.OUTSET=PIN0_bm;
			vTaskDelay(t);
			PORTA.OUTCLR=PIN0_bm;
			vTaskDelay(t);
		}
	}
}
void vbuzzer(void *param){
	uint16_t t = *(uint16_t *)param;;
	while(1){
		for(int i=0;i<=1000-t;i++){
			PORTA.OUTSET=PIN0_bm;
			vTaskDelay(t);
			PORTA.OUTCLR=PIN0_bm;
			vTaskDelay(t);
		}
	}
}

void delay_us(uint16_t t){
	for (uint16_t i=0;i<=t;i++){
		_delay_us(1);
	}
	return;
}

void led(uint8_t ledkind,uint8_t ledmode){//0:Á“”1:“_“”3:TGL4:‘S•”Á“”
	if(ledmode==1){
		PORTQ.OUTCLR=ledkind;
	}
	else if(ledmode==0){
		PORTQ.OUTSET=ledkind;
	}
	else if(ledmode==3){
		PORTQ.OUTTGL=ledkind;
	}
	else {
		PORTQ.OUTSET = PIN2_bm|PIN3_bm|PIN4_bm;
	}
}

void error_led(uint8_t no,uint8_t i){
	PORTK.DIRSET=PIN4_bm|PIN5_bm;
	if(no==2){
		if(i==1){
			PORTK.OUTSET=PIN4_bm;
		}
		else if(i==0){
			PORTK.OUTCLR=PIN4_bm;
		}
		else
		{
			PORTK.OUTTGL=PIN4_bm;
		}
	}
	else if(no==1){
		if(i==1){
			PORTK.OUTSET=PIN5_bm;
		}
		else if(i==0){
			PORTK.OUTCLR=PIN5_bm;
		}
		else{
			PORTK.OUTTGL=PIN5_bm;
		}
	}
	return;
}


void start(void){
	error_led(1,1);
	error_led(2,0);
	uint8_t timet = 150;
	while(!SW1&&!SW2&&!SW3){
		led(Redled,1);
		led(Blueled,0);
		led(Greenled,0);
		error_led(1,3);
		error_led(2,3);
		_delay_ms(timet);
		led(Redled,0);
		led(Blueled,1);
		led(Greenled,0);
		error_led(1,3);
		error_led(2,3);
		_delay_ms(timet);
		led(Redled,0);
		led(Blueled,0);
		led(Greenled,1);
		error_led(1,3);
		error_led(2,3);
		_delay_ms(timet);
	}
}