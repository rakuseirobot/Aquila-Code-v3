/*
 * ping_control.h
 *
 * Created: 2018/08/23 3:43:27
 *  Author: shun2
 */ 


#ifndef PING_CONTROL_H_
#define PING_CONTROL_H_
#include "initializing.hpp"
#include <avr/interrupt.h>
#include <util/delay.h>


#define Sikiti 550

uint16_t ping(int no);
uint16_t ping_check (PORT_t *p_port,uint8_t p_pin);
//bool check_ping(int x);
uint8_t check_ping2(int x);
uint8_t smaller(uint8_t x,uint8_t y);
uint8_t check_ping(int x);
void ping_debug(void);



#endif /* PING_CONTROL_H_ */