/*
 * sermo_control.hpp
 *
 * Created: 2018/08/24 16:19:38
 *  Author: shun2
 */ 


#ifndef SERMO_CONTROL_H_
#define SERMO_CONTROL_H_
#include<avr/io.h>
#include<stdint.h>

#define sermo_fre 130000UL

#define BUS_TWI &TWID
void select_bus(uint8_t busnumber);
void ReadFromMLX90614(TWI_t *port ,char addr, char cmd);
double ReadTempFromMLX90614(TWI_t *port , char addr);
void sermo_test(void);
uint8_t check_sermo();

#endif /* SERMO_CONTROL_H_ */