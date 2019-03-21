/*
 * ui_control.h
 *
 * Created: 2018/08/23 6:05:02
 *  Author: shun2
 */ 


#ifndef UI_CONTROL_H_
#define UI_CONTROL_H_

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define Greenled PIN1_bm
#define Blueled PIN2_bm
#define Redled PIN0_bm

#define SW1 (PORTH.IN & PIN0_bm)!=0
#define SW2 (PORTH.IN & PIN1_bm)!=0
#define SW3 (PORTH.IN & PIN2_bm)!=0

void init_buzzer(void);
void buzzer(uint16_t t=800);
void vbuzzer(void *param);
void delay_us(uint16_t t);
void led(uint8_t ledkind,uint8_t ledmode);
void error_led(uint8_t no,uint8_t i);
void start(void);

#endif /* UI_CONTROL_H_ */