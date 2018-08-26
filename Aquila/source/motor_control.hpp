/*
 * motor_control.h
 *
 * Created: 2018/08/23 3:04:31
 *  Author: shun2
 */ 


#ifndef MOTOR_CONTROL_H_
#define MOTOR_CONTROL_H_


#include <avr/io.h>
#include "lcd_control.hpp"
#include "xmega_spi.hpp"
#include "ping_control.hpp"
#include "ui_control.hpp"
#include "initializing.hpp"
#include <stdlib.h>
#include <util/delay.h>

void init_motor(void);
uint8_t mspi(uint8_t val,uint8_t i);
void m_send(uint8_t rl,uint8_t x,uint8_t y,uint8_t z);
int16_t smaller_s(int16_t x,int16_t y);
namespace motor{
	void brake(uint8_t x);
	void wait(void);
	void move(uint8_t x);/*
	void forever(void);
	void back(int x);
	void advance(int x);
	void turn(int x);*/
	void fix(uint8_t x,uint8_t ping1,uint8_t ping2,int no);
	//uint8_t fix_position(void);
	void fix_position();
	void turn_fix(uint8_t x);
	uint8_t notify_long_ex();
	#define m_speed 7
	#define m_turnspeed 5
}
#endif /* MOTOR_CONTROL_H_ */