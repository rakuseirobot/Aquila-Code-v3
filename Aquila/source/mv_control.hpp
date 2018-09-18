/*
 * mv_control.h
 *
 * Created: 2018/09/18 16:51:37
 *  Author: shun2
 */ 


#ifndef MV_CONTROL_H_
#define MV_CONTROL_H_
#include <avr/io.h>
#include <util/delay.h>
#include "xmega_spi.hpp"

void init_mv(void);
uint8_t mv_spi_send(uint8_t val, uint8_t i);


#endif /* MV_CONTROL_H_ */