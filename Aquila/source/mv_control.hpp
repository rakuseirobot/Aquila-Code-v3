/*
 * mv_control.h
 *
 * Created: 2018/09/18 16:51:37
 *  Author: shun2
 */ 


#ifndef MV_CONTROL_HPP_
#define MV_CONTROL_HPP_
#include <avr/io.h>
#include <util/delay.h>
#include "xmega_spi.hpp"
#include "action.hpp"
#include "lcd_control.hpp"

class kit_result{
	uint8_t num;
	public:
	void write(uint8_t x){num=x;}
	uint8_t read(){return num;}
	kit_result(){num=0;}
};
int k_r_read();
void k_r_write(int x);
//kit_result bblk;
void mv_sig(uint8_t i,bool ud);
void init_mv(void);
void mv_cap(uint8_t di,bool st);
uint8_t mv_spi_send(uint8_t val, uint8_t i);
void check_mv(uint8_t dir);

#endif /* MV_CONTROL_H_ */