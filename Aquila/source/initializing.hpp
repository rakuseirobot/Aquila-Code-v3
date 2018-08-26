/*
 * initializing.h
 *
 * Created: 2018/08/23 6:18:45
 *  Author: shun2
 */ 


#ifndef INITIALIZING_H_
#define INITIALIZING_H_

#include <avr/io.h>
#include "xmega_usart.hpp"
#include <avr/interrupt.h>
extern usart serial;
extern uint8_t rtcov;
void init_all(void);
void init_clock(void);
void rtc(void);//About 16 hours past, it will be overflow.	
void init_avr(void);


#endif /* INITIALIZING_H_ */