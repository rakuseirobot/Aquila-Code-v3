/*
 * sermo_control.cpp
 *
 * Created: 2018/08/24 16:20:22
 *  Author: shun2
 */ 
#include "sermo_control.hpp"


/*
PORTCとPORTD-プルアップ忘れてる
*/
//#define SERMO_addr 0xB4
#define SERMO_addr 0xB4
//#define temp_thre 15680
#define temp_thre 15200
uint32_t dat[6];
//uint8_t data[3]={0};
//twiReadMultiple(LCD1_TWI,&data,3);
/*
使い方は単純
アドレス0xE0(shifted)でbusnumber | (1<<3)を送ればいい。一応7を超えてないかだけ確認する必要がある。
*/

void select_bus(uint8_t busnumber){
	twi bus(BUS_TWI,sermo_fre);
	uint8_t bus_addr = 0x70<<1;
	bus.Address(bus_addr,0);
	if(busnumber<=7 && bus.WriteSingle(bus_addr)==0){
		_delay_us(10);
		bus.WriteSingle(busnumber | (1 << 3));
		_delay_us(10);
		bus.Stop();
	}
	else{
		bus.Stop();
		serial.string("error1\n\r");
	}
	_delay_us(100);
}
void ReadFromMLX90614(TWI_t *port ,char addr, char cmd){
	twi serm(port,sermo_fre);
	dat[4]=serm.Address(addr,0);	 // start recieve process
	serm.WriteSingle(cmd);	// Write command
	//twiStop(port);	 // publish repeat start condition
	dat[5]=serm.Address(addr,1);
	dat[0]=serm.ReadSingle(1);	 // request data+pec byte
	dat[1]=serm.ReadSingle(1);
	dat[2]=serm.ReadSingle(0);
	
	//serial.puthex2(dat[1]);
	serm.Stop();	 // end recieve process
	return;
}

/* Function that reads temperature from MLX90614 *
*	arguments									 *
*	addr : slave address						 *
*	return										 *
*	Degrees Celsius of temperature				 *
*************************************************/
double ReadTempFromMLX90614(TWI_t *port , char addr){
	uint32_t tmp;
	double temp;
	ReadFromMLX90614(port, addr, 0x07);	 // recieve from MLX90614
	tmp = ( dat[1] << 8 )	+ dat[0];	 // to temperature in marge two bytes
	temp = ( ( (double)tmp * 2.0 ) - 27315.0 ) / 100.0;	 // convert Absolute temperature to Degrees Celsius of temperature
	/*if(tmp>=20000){
		tmp=0;
	}*/
	return tmp;
}

void sermo_test(void){
	uint32_t dic;
	while(1){
		dic=ReadTempFromMLX90614(&TWID,SERMO_addr);
		serial.puthex2(dic);
		serial.string("\n\r");
		_delay_ms(1000);
	}
}

uint8_t check_sermo(){//0:left,1:front,2:right,3:no,4:error
	uint32_t f,r,l;
	select_bus(0);
	f=ReadTempFromMLX90614(&TWID,SERMO_addr);
	select_bus(2);
	l=ReadTempFromMLX90614(&TWID,SERMO_addr);
	select_bus(7);
	r=ReadTempFromMLX90614(&TWID,SERMO_addr);
	/*
	serial.puthex(f);
	serial.puthex(l);
	serial.puthex(r);
	serial.string("\n\r");*/
	bool fr=false,rr=false,lr=false;
	if (f>temp_thre){
		fr = true;
	}
	if (r>temp_thre){
		rr=true;
	}
	if (l>temp_thre){
		lr=true;
	}
	if(rr==true&&lr==false&&fr==false){
		return 2;
	}
	else if(lr==true&&rr==false&&fr==false){
		return 0;
	}
	else if(fr==true&&lr==false&&rr==false){
		return 1;
	}
	else if(rr==false&&lr==false&&fr==false){
		return 3;
	}
	else{
		return 4;
	}
}
