/*
 * initializing.cpp
 *
 * Created: 2018/08/23 6:18:26
 *  Author: shun2
 */ 
#include "initializing.hpp"
#include "xmega_usart.hpp"
#include "ping_control.hpp"
#include "ui_control.hpp"
#include "lcd_control.hpp"
#include "gyro_control.hpp"
#include "motor_control.hpp"
#include "action.hpp"
#include "color_control.hpp"
#include "mv_control.hpp"
#include <avr/io.h>
#include <avr/interrupt.h>
void init_all(void){
	init_clock();
	init_avr();
	init_lcd();
	init_motor();
	//rtc();
	init_color();
}


void init_clock(void){
	OSC.CTRL |= 0x02;					//32MHz内蔵発振器動作許可
	while((OSC.STATUS & 0x02) == 0);	//32MHz内蔵発振器が安定するまで待つ
	CPU_CCP = 0xD8;						//システムクロック制御レジスタアクセス許可を行う
	CLK.CTRL = 0x01;					//システムクロックを32MHzに変更
	CLK.PSCTRL=CLK_PSADIV0_bm;		//select Prescaler A as 2, Prescaler B and Prescaler C as 1, Clksys=16MHz, Clkper4=Clkper2=Clkper=Clkcpu=8MHz
	OSC.CTRL &= 0xFE;					//2MHz内蔵発振器動作停止
}


void rtc(void){
	OSC.CTRL = OSC_RC32KEN_bm;
	while((OSC_STATUS&&OSC_RC32KRDY_bm)==0);
	CLK.RTCCTRL = 0b00000001;
	RTC.CTRL = RTC_PRESCALER_DIV1024_gc;
}

void init_avr(void){
	PORTA.DIRSET=PIN0_bm;//ブザー
	PORTQ.DIRSET = PIN0_bm|PIN1_bm|PIN2_bm;//フルカラーLED
	PORTQ.OUTSET = PIN0_bm|PIN1_bm|PIN2_bm;
	PORTK.DIRSET = PIN0_bm|PIN1_bm|PIN2_bm|PIN3_bm;//ステッピングinit
	PORTB.DIRSET = PIN0_bm|PIN1_bm;//モーター用SIG
}

void init_int(void){
	PORTH.DIRCLR=PIN0_bm|PIN1_bm|PIN2_bm;
	PORTH.INTCTRL=PORT_INT0LVL_LO_gc|PORT_INT1LVL_HI_gc;
	PORTH.PIN0CTRL=PORT_ISC_RISING_gc;
	PORTH.PIN1CTRL=PORT_ISC_RISING_gc;
	PORTH.INT0MASK=PIN0_bm;
	PORTH.INT1MASK=PIN1_bm;
	PMIC.CTRL = PMIC_LOLVLEN_bm|PMIC_HILVLEN_bm;
	sei();
}
bool pause=false;
void pause_fun(void){
	lcd_clear();
	lcd_putstr(LCD1_TWI,"PAUSE!");
	while(pause==true){
		led(Redled,0);
		led(Blueled,0);
		led(Greenled,0);
		error_led(1,0);
		error_led(2,0);
		buzzer(200);
		led(Redled,1);
		led(Blueled,1);
		led(Greenled,1);
		error_led(1,1);
		error_led(2,1);
		buzzer(700);
		if(pause==false){
			lcd_clear();
			break;
		}
	}
	return;
}
ISR(PORTH_INT0_vect){
	PORTB.OUTSET=PIN0_bm|PIN1_bm;
	led(Redled,1);
	led(Blueled,1);
	led(Greenled,1);
	error_led(1,1);
	error_led(2,1);
	pause=true;
	pause_fun();
}
ISR(PORTH_INT1_vect){
	pause=false;
	led(Redled,0);
	led(Blueled,0);
	led(Greenled,0);
	error_led(1,0);
	error_led(2,0);
	PORTB.OUTCLR=PIN0_bm|PIN1_bm;
}

namespace debug{
	void ping_d(uint8_t num,bool s){
		if(s==true){
			serial.string("\r");
		}
		else{
			serial.string(" ");
		}
		lcd_clear();
		lcd_putstr(LCD1_TWI,"D_ping");
		serial.string("\x1b[42m[ping:");
		if(num==7){
			for(uint8_t i=1;i<=6;i++){
				serial.putint(ping(i));
				if (i!=6){
					serial.string(",");
				}else{
					serial.string("]");
				}
			}
		}
		else{
			serial.putint(ping(num));
			serial.string("]");
		}
		lcd_clear();
		return;
	}
	void mv(uint8_t num){
		lcd_clear();
		lcd_putstr(LCD1_TWI,"D_mv");
		serial.string("mv:");
		if(num==4){
			serial.putint((PORTJ.IN & PIN5_bm));
			serial.string(",");
			serial.putint((PORTJ.IN & PIN6_bm));
			serial.string(",");
			serial.putint((PORTJ.IN & PIN7_bm));
			serial.string("\n\r");
		}
		else{
			switch (num)
			{
			case 1:
				serial.putint((PORTJ.IN & PIN5_bm));
				break;
			case 2:
				serial.putint((PORTJ.IN & PIN6_bm));
				break;
			case 3:
				serial.putint((PORTJ.IN & PIN7_bm));
				break;
			default:
				break;
			}
			serial.string("\n\r");
		}
		lcd_clear();
		return;
	}
	void jy(bool s){
		lcd_clear();
		lcd_putstr(LCD1_TWI,"D_jy");
		if(s==true){
			serial.string("\r");
		}
		else{
			serial.string(" ");
		}
		serial.string("\x1b[41m[Acceleration: ");
		acc_mes();
		serial.string("][Angle: ");
		gyro_mes();
		serial.string("][JY901--time: ");
		gyro_time_mes();
		serial.string("]\x1b[40m        ");
		lcd_clear();
	}
	void color(bool s){
		lcd_clear();
		lcd_putstr(LCD1_TWI,"D_jy");
		if(s==true){
			serial.string("\r");
		}
		else{
			serial.string(" ");
		}
		serial.string("\x1b[45m\x1b[30m[Color:\x1b[40m\x1b[37m");
		uint8_t res = color_check();
		if(res==1){
			serial.string("Black");
		}
		else{
			serial.string("\x1b[7mWhite\x1b[0m");
		}
		serial.string(" \x1b[31mRed:");//Red
		serial.putint(red);
		serial.string("\x1b[32m Green:");//Green
		serial.putint(green);
		serial.string("\x1b[34m Blue:");//Blue
		serial.putint(blue);
		serial.string("]\x1b[40m\x1b[37m");
		lcd_clear();
	}
}