/*
 * xmega_spi.cpp
 *
 * Created: 2018/08/23 1:26:17
 *  Author: shun2
 */ 
#include <avr/io.h>
#include "xmega_spi.hpp"

spi::spi(SPI_t *ports,PORT_t *portg,SPI_PRESCALER_enum scale){
	sport = ports;
	gport = portg;
	gport->DIRCLR = PIN6_bm;
	gport->DIRSET = PIN4_bm | PIN5_bm | PIN7_bm;//MOSI,MISO
	sport->CTRL = SPI_ENABLE_bm | SPI_MASTER_bm | SPI_MODE_0_gc | scale;
	return;
}
void spi::close(void){
	sport->CTRL = 0;
	gport->DIRCLR = PIN4_bm|PIN5_bm|PIN6_bm|PIN7_bm ;
	return;
}

uint8_t spi::send(uint8_t val){
	sport->DATA = val ;
	while(!(sport->STATUS & 0x80));
	sport->STATUS=0;
	return sport->DATA;
}