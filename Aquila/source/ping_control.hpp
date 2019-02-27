/*
 * ping_control.h
 *
 * Created: 2018/08/23 3:43:27
 *  Author: shun2
 */ 


#ifndef PING_CONTROL_H_
#define PING_CONTROL_H_
#define Sikiti 600
#include <avr/io.h>
#include <stdint.h>

uint16_t ping(int no);
uint16_t ping_check (PORT_t *p_port,uint8_t p_pin);
//int for_cp(int num);
//bool check_ping(int x);
uint8_t c_p(int x);
uint16_t smaller(uint16_t x,uint16_t y);
uint8_t check_ping(int x);
void ping_debug(void);

typedef enum{
    LEFT_BACK,
    LEFT_FRONT,
    FRONT,
    RIGHT_FRONT,
    RIGHT_BACK,
    BACK
}ping_ch_t;



#endif /* PING_CONTROL_H_ */