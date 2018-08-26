/*
 * initializing.cpp
 *
 * Created: 2018/08/23 6:18:26
 *  Author: shun2
 */ 
#include "initializing.hpp"
#include "ui_control.hpp"
#include "lcd_control.hpp"
#include "motor_control.hpp"
#include "action.hpp"
#include "color_control.hpp"

void init_all(void){
	init_clock();
	init_avr();
	init_lcd();
	init_motor();
	rtc();
	init_color();
}


void init_clock(void){
	OSC.CTRL |= 0x02;					//32MHz�������U�퓮�싖��
	while((OSC.STATUS & 0x02) == 0);	//32MHz�������U�킪���肷��܂ő҂�
	CPU_CCP = 0xD8;						//�V�X�e���N���b�N���䃌�W�X�^�A�N�Z�X�����s��
	CLK.CTRL = 0x01;					//�V�X�e���N���b�N��32MHz�ɕύX
	CLK.PSCTRL=CLK_PSADIV0_bm;		//select Prescaler A as 2, Prescaler B and Prescaler C as 1, Clksys=16MHz, Clkper4=Clkper2=Clkper=Clkcpu=8MHz
	OSC.CTRL &= 0xFE;					//2MHz�������U�퓮���~
}


void rtc(void){
	OSC.CTRL = OSC_RC32KEN_bm;
	while((OSC_STATUS&&OSC_RC32KRDY_bm)==0);
	CLK.RTCCTRL = 0b00000001;
	RTC.CTRL = RTC_PRESCALER_DIV1024_gc;
}

void init_avr(void){
	PORTA.DIRSET=PIN0_bm;//�u�U�[
	PORTQ.DIRSET = PIN0_bm|PIN1_bm|PIN2_bm;//�t���J���[LED
	PORTQ.OUTSET = PIN0_bm|PIN1_bm|PIN2_bm;
	PORTK.DIRSET = PIN0_bm|PIN1_bm|PIN2_bm|PIN3_bm;//�X�e�b�s���Oinit
}
