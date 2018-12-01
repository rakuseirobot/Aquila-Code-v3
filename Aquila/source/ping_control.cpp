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
// const int Front=1,Back=4; 
// const double LEFT=73.18, RIGHT=3.11; //L::2,3//R::5,6

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

// int for_cp(int num){
// 	if(num==0){
// 		if(smaller(ping(1),ping(2))<Sikiti){
// 			return 1;
// 		}else{
// 			return	0;
// 		}
// 	}else if(num==1){
// 		if(ping(3)<Sikiti){
// 			return 1;
// 		}else{
// 			return 0;
// 		}
// 	}else if(num==2){
// 		if(smaller(ping(4),ping(5))<Sikiti){
// 			return 1;
// 		}else{
// 			return 0;
// 		}
// 	}else if(num==3){
// 		if(ping(6)<Sikiti){
// 			return 1;
// 		}else{
// 			return 0;
// 		}
// 	}
// }

uint8_t check_ping2(int x){
 if(ping(x)<Sikiti){
	return 1;
 }else if(ping(x)<1400){//50=>60・ｽﾉ修・ｽ・ｽ9.05
    return 2;
 }else if(ping(x)<2360){
	return 3;
 }else if(ping(x)<3200){
	return 4;
 }else{
	return 5;
 }
 return 255;
}

uint16_t smaller(uint16_t x,uint16_t y){//・ｽ蛯ｫ・ｽ・ｽ・ｽ・ｽ
 if(x<y){
  return x;
 }else{
  return y;
 }
}

uint8_t check_ping(int x){//x::direction,return 1,2,3,4,0(0=error)
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

void ping_debug(void){
	for(uint8_t i=1;i<=6;i++){
		serial.putdec(ping(i));
		serial.string(",");
	}
	serial.string("\n\r");
	return;
}
