/*
 * ping_control.cpp
 *
 * Created: 2018/08/23 3:43:42
 *  Author: shun2
 */ 
#include "ping_control.hpp"
/////////////////////////////・ｽs・ｽ・ｽ・ｽz・ｽu//////////////////////////
/*
ping1 PORTE0
ping2 PORTE1
ping3 PORTE2
ping4 PORTE3
ping5 PORTE4
ping6 PORTE5
////////////////   
  Front
	3
2		4
1		5
	6
   Back
///////////////
*/
////////////////////////////////////////////

#define TCC_CLK TC_CLKSEL_DIV64_gc
//#define TCC_CLK TC_TC0_CLKSEL_DIV64_gc    //if TCC_CLK is Error try this.

uint16_t ping(int no){
	uint16_t ping_data=0;
	switch(no){
		case 1:ping_data=ping_check(&PORTE,PIN0_bm);break;
		case 2:ping_data=ping_check(&PORTE,PIN1_bm);break;
		case 3:ping_data=ping_check(&PORTE,PIN2_bm);break;
		case 4:ping_data=ping_check(&PORTE,PIN3_bm);break;
		case 5:ping_data=ping_check(&PORTE,PIN4_bm);break;
		case 6:ping_data=ping_check(&PORTE,PIN5_bm);break;
		default:break;
	}
	return ping_data;
}
uint16_t ping_check (PORT_t *p_port,uint8_t p_pin){
	//PD2 ping4・ｽﾅ読みゑｿｽ・ｽ・ｽ・ｽﾆゑｿｽ
	//ping(&PORTD,PIN2_bm);
	p_port->DIRSET=p_pin;//(*p_port).DIRSET
	p_port->OUT=p_pin;
	_delay_us(4);
	p_port->OUTCLR=p_pin;
	p_port->DIRCLR=p_pin;
	while((p_port->IN&p_pin)==0);
	TCC0.CNT=0;
	cli();
	TCC0.CTRLA=TCC_CLK;
	while((p_port->IN&p_pin)!=0);
	TCC0.CTRLA=0x00;
	sei();
	_delay_ms(5);
	return (uint16_t)(TCC0.CNT);
}
const int Front=1,Back=4; 
const double LEFT=73.18, RIGHT=3.11; //L::2,3//R::5,6

/*
bool check_ping(int x){
 if(ping(x)<Sikiti){
	return true;
//  }else if(ping(x)<60){//50=>60・ｽﾉ修・ｽ・ｽ9.05
//   return 3;
//  }else if(ping(x)<80){
//   return 5;
 }else{//・ｽG・ｽ・ｽ・ｽ[
	return false;
 }
 return false;
}
*/

uint8_t check_ping2(int x){
 if(ping(x)<Sikiti){
	return 1;
 }else if(ping(x)<Sikiti*2){//50=>60・ｽﾉ修・ｽ・ｽ9.05
    return 2;
 }else if(ping(x)<Sikiti*3){
	return 3;
 }else if(ping(x)<Sikiti*4){
	return 4;
 }else{
	return 0;
 }
 return 255;
}

uint8_t smaller(uint8_t x,uint8_t y){//・ｽ蛯ｫ・ｽ・ｽ・ｽ・ｽ
 if(x<y){
  return y;
 }else{
  return x;
 }
}

uint8_t check_ping(int x){
	switch(x){
		case 0:
			return smaller(check_ping2(1),check_ping2(2));
			break;
		case 1:
			return check_ping2(3);
			break;
		case 2:
			return smaller(check_ping2(4),check_ping2(5));
			break;
		case 3:
			return check_ping2(6);
			break;
		default:
			break;
	}
}

// uint8_t check_ping(double x){
//  if(x==73.18)return smaller(check_ping(2),check_ping(3));
//  if(x==3.11)return smaller(check_ping(5),check_ping(6));
//  else return 0;
// }


void ping_debug(void){
	for(uint8_t i=1;i<=6;i++){
		serial.putdec(ping(i));
		serial.string(",");
	}
	serial.string("\n\r");
	return;
}
