/*
 * gyro_control.h
 *
 * Created: 2018/08/28 11:18:56
 *  Author: shun2
 */ 


#ifndef GYRO_CONTROL_H_
#define GYRO_CONTROL_H_

#include <avr/io.h>
#include <util/delay.h>
#include "xmega_usart.hpp"
#include "xmega_twi.hpp"
#include "initializing.hpp"

void gyro_send(uint8_t reg,uint8_t dh,uint8_t dl);
void gyro_mes(void);
void gyro_cali(void);
void gyro_set200hz(void);
void gyro_Save(void);


#endif /* GYRO_CONTROL_H_ */