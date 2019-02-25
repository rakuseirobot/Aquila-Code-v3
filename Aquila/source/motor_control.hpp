/*
 * motor_control.h
 *
 * Created: 2018/08/23 3:04:31
 *  Author: shun2
 */ 


#ifndef MOTOR_CONTROL_H_
#define MOTOR_CONTROL_H_

#include "data_structure.hpp"
#include <stdint.h>

#define Acc_thre_u 2.9
#define Acc_thre_d -4
#define Acc_slope_thre 0.1
#define Ang_slope_Norm 178
#define Ang_slope_thre 15
#define Ang_x_Norm 180
#define Ang_x_thre 3
void Save_angle(void);
void init_motor(void);
uint8_t mspi(uint8_t val,uint8_t i);
void m_send(uint8_t rl,uint8_t x,uint8_t y,uint8_t z);
int16_t smaller_s(int16_t x,int16_t y);
namespace motor{
	void brake(uint8_t x);
	void wait(bool check=true);
	void move(uint8_t x);/*
	void forever(void);
	void back(int x);
	void advance(int x);
	void turn(int x);*/
	void fix(uint8_t x,uint8_t ping1,uint8_t ping2,int no);
	//uint8_t fix_position(void);
	uint8_t notify_half(uint8_t x);
	void fix_position(uint8_t x =v::front);
	void gb_fix(void);
	void turn_fix(uint8_t x=0);
	uint8_t notify_long_acc(uint8_t x,bool buz=true);
	uint8_t notify_long_ang(uint8_t x,bool buz=true);
	uint8_t notify_long_ex(void);
	void fix_angle(void);
	void fix_angle_v(float angl);
	void set_angle(float ang);
	#define m_speed 7
	#define m_turnspeed 6
}
void enkaigei(void);
#endif /* MOTOR_CONTROL_H_ */