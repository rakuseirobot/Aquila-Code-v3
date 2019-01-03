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

MV
   2
1  ��  3 

*/

#include "mv_control.hpp"
#include <avr/io.h>
#include <util/delay.h>
#include "xmega_spi.hpp"
#include "action.hpp"
#include "lcd_control.hpp"
#include "ui_control.hpp"
#include "core.hpp"

spi mv(&SPID,&PORTD,SPI_PRESCALER_DIV4_gc);
//hako hhh;

void init_mv(void){
	PORTD.DIRSET=PIN2_bm|PIN3_bm|PIN4_bm;
	PORTJ.DIRCLR=PIN5_bm|PIN6_bm|PIN7_bm;
	PORTJ.OUTSET=PIN5_bm|PIN6_bm|PIN7_bm;
	PORTD.OUTCLR=PIN2_bm|PIN3_bm|PIN4_bm;
	return;
}


void mv_cap(uint8_t di,bool st){
	switch(di){
		case 1:
			if(st==true){
				PORTD.OUTCLR=PIN2_bm;
			}else{
				PORTD.OUTSET=PIN2_bm;
			}
			break;
		case 2:
			if(st==true){
				PORTD.OUTCLR=PIN3_bm;
			}else{
				PORTD.OUTSET=PIN3_bm;
			}
			break;
		case 3:
			if(st==true){
				PORTD.OUTCLR=PIN4_bm;
			}else{
				PORTD.OUTSET=PIN4_bm;
			}
			break;
		default:
			break;
	}
	return;
}
void mv_sig(uint8_t i,bool ud){
	if (ud){
		if (i==1){
			PORTD.OUTSET=PIN2_bm;
		}
		else if (i==2){	
			PORTD.OUTSET=PIN3_bm;
		}
		else if(i==3){
			PORTD.OUTSET=PIN4_bm;
		}
		else{
			return;
		}
	}
	else{
		if (i==1){
			PORTD.OUTCLR=PIN2_bm;
		}
		else if (i==2){
			PORTD.OUTCLR=PIN3_bm;
		}
		else if(i==3){
			PORTD.OUTCLR=PIN4_bm;
		}
		else{
			return;
		}
	}
	return;
}
uint8_t mv_spi_send(uint8_t val, uint8_t i){
	if (i==1){
		PORTD.OUTSET=PIN2_bm;
	}
	else if (i==2){
		PORTD.OUTSET=PIN3_bm;
	}
	else if(i==3){
		PORTD.OUTSET=PIN4_bm;
	}
	else{
		return 0;
	}
	_delay_ms(10);
	if (i==1){
		PORTD.OUTCLR=PIN2_bm;
	}
	else if (i==2){
		PORTD.OUTCLR=PIN3_bm;
	}
	else if(i==3){
		PORTD.OUTCLR=PIN4_bm;
	}
	else{
		return 0;
	}
	uint8_t dat = 0;
	_delay_ms(10);
	dat = mv.send(val);
	if (i==1){
		PORTD.OUTSET=PIN2_bm;
	}
	else if (i==2){
		PORTD.OUTSET=PIN3_bm;	
	}
	else if(i==3){
		PORTD.OUTSET=PIN4_bm;
	}
	else{
		return 0;
	}
	_delay_ms(10);
	PORTD.OUTCLR=PIN2_bm|PIN3_bm|PIN4_bm;
	//while((PORTJ.IN & PIN5_bm)==0||(PORTJ.IN & PIN6_bm)==0||(PORTJ.IN & PIN7_bm)==0);
	return dat;
}

bool kit_chk(void){
	if(ta.ac_next(v::front,1)==np)return false;
	int key = ta.r_now()->type;
	return (key==v::normal||key==v::unknown);
	//return (hhh.key==0 || hhh.key==1 );//normal or unknown
}

void check_mv(uint8_t dir){
	PORTB.OUTSET=PIN0_bm|PIN1_bm;
	//mv_sig(dir,false);
	_delay_ms(2);
	led(Blueled,1);
	uint8_t res = mv_spi_send(dir,1);
	if(kit_chk()==false){
		PORTB.OUTCLR=PIN0_bm|PIN1_bm;
		return;
	}
	ta.r_now()->type=v::r_kit;
	//hhh.key=1;
	mv_cap(dir,false);
	serial.string("ch");
	serial.putdec(res);
	serial.string("\n\r");
	mv_cap(1,false);
	mv_cap(2,false);
	mv_cap(3,false);
	led(Blueled,0);
	switch(res){
		case 3://H  2kits
			lcd_clear();
			lcd_putstr(LCD1_TWI,"Find H!");
			finded_victim(2);
			break;
		case 4://S  1kits
			lcd_clear();
			lcd_putstr(LCD1_TWI,"Find S!");
			finded_victim(1);
			break;
		case 5://U 0kits
			lcd_clear();
			lcd_putstr(LCD1_TWI,"Find U!");
			finded_victim(0);
			break;
		case 6:
			lcd_clear();
			lcd_putstr(LCD1_TWI,"Find Sermo");
			finded_victim(1);
			break;
		case 7:
		case 8:
			lcd_clear();
			lcd_putstr(LCD1_TWI,"Find Error");
			finded_victim(1);
			break;
		default:
			break;
	};
	lcd_clear();
	
	mv_cap(1,true);
	mv_cap(2,true);
	mv_cap(3,true);
	mv_cap(dir,false);
	_delay_ms(2);
	PORTB.OUTCLR=PIN0_bm|PIN1_bm;
	return;
}

