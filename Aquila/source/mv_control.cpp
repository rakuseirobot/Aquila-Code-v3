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
1  Å™  3 


*/

#include "mv_control.hpp"
spi mv(&SPID,&PORTD,SPI_PRESCALER_DIV4_gc);
void init_mv(void){
	PORTD.DIRCLR=PIN2_bm|PIN3_bm|PIN4_bm;
	PORTJ.DIRSET=PIN5_bm|PIN6_bm|PIN7_bm;
	PORTJ.OUTSET=PIN5_bm|PIN6_bm|PIN7_bm;
	//PORTD.OUTSET=PIN2_bm|PIN3_bm|PIN4_bm;
	return;
}
void mv_cap(uint8_t di,bool st){
	uint8_t resp = 0;
	if (st){
		resp=mv_spi_send(di,1);
	}
	else {
		resp=mv_spi_send(di,2);
	}
	//if(resp!=90){
		//mv_cap(di,st);
	//}
	return;
}
void mv_sig(uint8_t i,bool ud){
	if (ud){
		if (i==1){
			PORTJ.OUTSET=PIN5_bm;
		}
		else if (i==2){	
			PORTJ.OUTSET=PIN6_bm;
		}
		else if(i==3){
			PORTJ.OUTSET=PIN7_bm;
		}
		else{
			return;
		}
	}
	else{
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
			return;
		}
	}
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

void check_mv(uint8_t dir){
	PORTB.OUTSET=PIN0_bm|PIN1_bm;
	mv_sig(dir,false);
	_delay_ms(5);
	led(Blueled,1);
	uint8_t res = mv_spi_send(dir,1);
	serial.putdec(res);
	led(Blueled,0);
	switch(res){
		case 3://H  2kits
			finded_victim(2);
			break;
		case 4://S  1kits
			finded_victim(1);
			break;
		case 5:
			finded_victim(0);
			break;
		case 6:
			finded_victim(1);
			break;
		case 7:
			finded_victim(1);
			break;
		default:
			break;
	}
	//mv_sig(dir,true);
	_delay_ms(10);
	PORTB.OUTCLR=PIN0_bm|PIN1_bm;
	return;
}

