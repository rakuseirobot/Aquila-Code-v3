/*
 * initializing.h
 *
 * Created: 2018/08/23 6:18:45
 *  Author: shun2
 */ 


#ifndef INITIALIZING_H_
#define INITIALIZING_H_
#include <stdint.h>
#include "xmega_usart.hpp"

extern usart serial;
extern uint8_t rtcov;
void init_all(void);
void init_clock(void);
void rtc(void);//About 16 hours past, it will be overflow.	
void init_avr(void);
void init_int(void);
void pause_fun(void);
namespace debug{
	void ping_d(uint8_t num=7,bool s=true);
	void mv(uint8_t num=4);
	void jy(bool s=true);
	void color(bool s=true);
}


#endif /* INITIALIZING_H_ */