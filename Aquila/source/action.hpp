/*
 * action.h
 *
 * Created: 2018/08/23 13:57:09
 *  Author: shun2
 */ 


#ifndef ACTION_H_
#define ACTION_H_
#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include "xmega_twi.hpp"
#include "initializing.hpp"
#include "ui_control.hpp"

void throw_kit(void);
//void finded_victim(void);
void finded_victim(uint8_t co);
bool record_time(uint16_t time);
uint16_t read_time(void);
#endif /* ACTION_H_ */