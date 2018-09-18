/*
 * mv_control.cpp
 *
 * Created: 2018/09/18 16:51:55
 *  Author: shun2
 */ 

/*
PD2:MV1
PD3:MV2
PD4:MV3  --SIG

PJ5:MVS1
PJ6:MVS2
PJ7:MVS3  --SS
*/

#include "mv_control.hpp"
spi mv(&SPID,&PORTD,SPI_PRESCALER_DIV16_gc);
void init_mv(void){
	PORTJ.DIRSET=PIN5_bm|PIN6_bm|PIN7_bm;
	PORTJ.OUTSET=PIN5_bm|PIN6_bm|PIN7_bm;
	return;
}
uint8_t mv_spi_send(uint8_t val, uint8_t i){
	if (i==1){
		PORTJ.OUTCLR=PIN5_bm;
	}
	else if (i==2){
		
		PORTJ.OUTCLR=PIN6_bm;
	}
	else if(i==3){
		
		PORTJ.OUTCLR=PIN7_bm;
	}
	else{
		return 0;
	}
	uint8_t dat = 0;
	_delay_ms(5);
	dat = mv.send(val);
	PORTJ.OUTSET=PIN5_bm|PIN6_bm|PIN7_bm;
	return dat;
}

