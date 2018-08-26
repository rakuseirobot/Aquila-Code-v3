/*
 * motor_control.cpp
 *
 * Created: 2018/08/23 3:04:47
 *  Author: shun2
 */ 

/*
PB2,PB3 --SS
*/
#include "motor_control.hpp"
#define RightM PIN2_bm //1
#define LeftM PIN3_bm  //2


spi motor_spi(&SPIC,&PORTC,SPI_PRESCALER_DIV16_gc);
const int rose=700;
const int16_t longway = 4500;

#if rose>Sikiti*2
#error "Rose's value is wrong!"
//rose=(Sikiti/2)*3
#else
#endif

const int gbno=120;



uint8_t data=000;

void init_motor(void){
	PORTB.DIRSET=PIN2_bm|PIN3_bm;
	PORTB.OUTSET=PIN2_bm|PIN3_bm;
}
uint8_t mspi(uint8_t val,uint8_t i){
	if(i==2){
		PORTB.OUTCLR=LeftM;
	}
	else if(i==1){
		PORTB.OUTCLR=RightM;
	}
	else{
		return 0;
	}
	uint8_t dat = 0;
	_delay_ms(5);
	dat = motor_spi.send(val);
	return dat;
}
void m_send(uint8_t rl,uint8_t x,uint8_t y,uint8_t z){//rl:Right or Left(1:L2:R)x:Go or Back(1:Back,2:Go)y:speed(max7(1 is for fixing movement))z:distance(1:1block,2:2blocks,3:turn,4:half))
	//data = 100*x+10*y+z;
	if(x>3){
		x=3;
	}
	if(y>7){
		y=7;
	}
	if(z>7){
		z=7;
	}
	data = (x<<6) | (y<<3) | z;
	if(rl == 1){
		PORTB.OUTCLR = PIN2_bm;
	}
	else if(rl == 2){
		PORTB.OUTCLR = PIN3_bm;
	}
	else{
		return;
	}
	_delay_ms(5);
	motor_spi.send(data);
	PORTB.OUTSET=PIN2_bm|PIN3_bm;
}
int16_t smaller_s(int16_t x,int16_t y){
	if(x<y){
		return x;
	}
	else {
		return y;
	}
}

namespace motor{
	//usart serial(&USARTC0,&PORTC);
	
	void brake(uint8_t x){
		m_send(x,3,0,0);
		return;
	}
	//uint32_t mcount = 0;
	//const uint32_t mvalue = 600;
	/*void wait(void){
		PORTF.OUTCLR = PIN3_bm;
		while(spi(0,1)!=1&&mcount<=mvalue){
			mcount++;
		}
		while(spi(0,2)!=1&&mcount<=mvalue){
			mcount++;
		}
		if(mcount >= mvalue){
			PORTF.OUTSET = PIN3_bm;
		}
		mcount = 0;
	}*/
	void wait(void){
		while(mspi(0,1)!=1);
		while(mspi(0,2)!=1);
		return;
	}

	void move(uint8_t x=6){// x = 0:1 block Advance 1:2 blocks Advance 2:Left Turn with Copass 3:Right Turn with Compass 4:1 block Back 5:2 block Back 6:Half block Advance 7:Half block Back 8:Left Turn without Compass 9:Right Turn without Compass 
		_delay_ms(5);
		int16_t first = 0;
		int16_t now = 0;
		switch(x){
			case 0: //???u???b?N????
			m_send(2,2,m_speed,1);
			m_send(1,2,m_speed,1);
			motor::wait();
			//_delay_ms(300);
			break;
			case 1: //2?u???b?N????
			m_send(1,2,m_speed,2);
			m_send(2,2,m_speed,2);
			motor::wait();
			//_delay_ms(300);
			break;
			/*case 2:
				first = compass_read();
				//usart_putdec(first);
				m_send(1,2,m_turnspeed,5);
				m_send(2,1,m_turnspeed,5);
				_delay_ms(10);
				now=compass_read();
				//usart_putdec(now);
				//usaort_string("\n\r");
				do{
					now=compass_read();
					//usart_putdec(now);
					//usart_string("\n\r");
				}while((first<now?now-first:now-first+3600)>2700||(first<now?now-first:now-first+3600)<50);
				motor::brake(1);
				motor::brake(2);
				motor::wait();
			break;
			case 3:
				first = compass_read();
				//usart_putdec(first);
				m_send(1,1,m_turnspeed,5);
				m_send(2,2,m_turnspeed,5);
				_delay_ms(10);
				now=compass_read();
				//usart_putdec(now);
				//usart_string("\n\r");
				do{
					now=compass_read();
					//usart_putdec(now);
					//usart_string("\n\r");
				}while((first<now?now-first:now-first+3600)<900||(first<now?now-first:now-first+3600)>3550);
				motor::brake(1);
				motor::brake(2);
				motor::wait();
				break;*/
			case 4: //???u???b?N?O?i
			m_send(1,1,m_speed,1);
			m_send(2,1,m_speed,1);
			motor::wait();
			//_delay_ms(300);
			break;
			case 5: //???u???b?N?O?
			m_send(1,1,m_speed,2);
			m_send(2,1,m_speed,2);
			//_delay_ms(300);
			motor::wait();
			break;

			case 6:
				m_send(1,2,m_speed,4);
				m_send(2,2,m_speed,4);
				motor::wait();
				//_delay_ms(300);
			break;
			case 7:
				m_send(1,1,m_speed,4);
				m_send(2,1,m_speed,4);
				motor::wait();
				//_delay_ms(300);
			break;
			case 8: //?E????V?n????
				m_send(1,2,m_turnspeed,3);
				m_send(2,1,m_turnspeed,3);
				motor::wait();
				//_delay_ms(300);
			break;
			case 9: //??????V?n????
				m_send(1,1,m_turnspeed,3);
				m_send(2,2,m_turnspeed,3);
				motor::wait();
				//_delay_ms(300);
			break;
			default:
				motor::brake(1);
				motor::brake(2);
				motor::wait();
				//_delay_ms(300);
			//break;
		}
		_delay_ms(20);
	}
	void forever(void){
		m_send(1,1,5,0);
		m_send(2,1,5,0);
		while(ping(1)>=3);
		motor::move();
		return;
	}/*
	void back(int x){
		if(x==1){
			lcd_putstr(LCD1_TWI,"back1");
		}
		else if(x==2){
			lcd_putstr(LCD1_TWI,"back2");
		}
		motor::move(x-1);
		lcd_clear();
		return;
	}

	void advance(int x){
		if(x==1){
			lcd_putstr(LCD1_TWI,"advance1");
		}
		else if(x==2){
			lcd_putstr(LCD1_TWI,"advance2");
		}
		motor::move(x+3);
		lcd_clear();
		return;
	}

	void turn(int x){//right left ???
		if(x==1){
			lcd_putstr(LCD1_TWI,"turn_l");
		}
		else if(x==2){
			lcd_putstr(LCD1_TWI,"turn_r");
		}
		motor::move(x+1);
		lcd_clear();
		return;
	}*/

	void fix(uint8_t x,uint8_t ping1,uint8_t ping2,int no){//x=right,left ping1,ping2:Compare ping number no:Sikiiti 
		int val=0;
		if(x==1){//left
			m_send(1,1,2,1);
			m_send(2,2,2,1);
		}
		else if(x==2){//right
			m_send(1,2,2,1);
			m_send(2,1,2,1);
		}
		while(1){
			val=ping(ping1)-ping(ping2);
			if(abs(val)>=no){
				break;
			}
		}
		motor::move();
		return;
	}
	#define fixno 5 //gb_fix Sikiiti
	const int gbbest=185;
	uint32_t tbest = 600;
	void gb_fix(void){
		int16_t dis[3];
		dis[0]=ping(1);//Forward
		dis[1]=ping(4);//Back
		if(Sikiti>=dis[0]){
			if((gbbest-dis[0])<fixno*-1){
				m_send(1,2,1,2);
				m_send(2,2,1,2);
				while(dis[0]>gbbest){
					if(dis[0]>=longway){
						break;
					}
					dis[0]=ping(1);
				}
				motor::brake(1);
				motor::brake(2);
			}
			else if((gbbest-dis[0])>fixno){
				m_send(1,1,1,2);
				m_send(2,1,1,2);
				while(dis[0]<gbbest){
					if(dis[0]>=longway){
						break;
					}
					dis[0]=ping(1);
				}
				motor::brake(1);
				motor::brake(2);
			}
		}
		else if(Sikiti>=dis[1]){
			if((gbbest-dis[1])>fixno){
				m_send(1,2,1,2);
				m_send(2,2,1,2);
				while(dis[1]<gbbest){
					if(dis[1]>=longway){
						break;
					}
					dis[1]=ping(4);
				}
				motor::brake(1);
				motor::brake(2);
			}
			else if((gbbest-dis[1])<fixno*-1){
				m_send(1,1,1,2);
				m_send(2,1,1,2);
				while(dis[1]>gbbest){
					if(dis[1]>=longway){
						break;
					}
					dis[1]=ping(4);
				}
				motor::brake(1);
				motor::brake(2);
			}
		}
		else{}
		return;
	}
	const int32_t turnvalue = 2;
	void turn_fix(uint8_t force = 0){
		int val=0;
		uint8_t chk[2]={0};
		if (!force&&smaller_s(ping(2),ping(3))>=Sikiti&&smaller_s(ping(5),ping(6))>=Sikiti){
			return;
		}
		else{
			if (smaller_s(ping(2),ping(3))<=Sikiti){
				if(!(ping(2)<=Sikiti&&ping(3)<=Sikiti)){
					return;
				}
			}
			if (smaller_s(ping(5),ping(6))<=Sikiti){
				if(!(ping(5)<=Sikiti&&ping(6)<=Sikiti)){
					return;
				}
			}
			int ldiff = (ping(2)+ping(3))/2;
			int rdiff = (ping(5)+ping(6))/2;
			lcd_clear();
			lcd_putstr(LCD1_TWI,"FixingTurn");
			
			if(rdiff>ldiff){
				chk[0]=2;
				chk[1]=3;
				//usart_string("use Left!\n\r");
			}
			else if(rdiff<ldiff){
				chk[0]=5;
				chk[1]=6;
				//usart_string("use Right!\n\r");
			}
			//usart_putdec(ping(chk[0]));
			//usart_string(",");
			//usart_putdec(ping(chk[1]));
			val=ping(chk[0])-ping(chk[1]);
			//usart_string(",");
			//usart_puthex2(abs(val));
			//usart_string("\n\r");
			if(val < turnvalue*-1){
				m_send(1,1,1,2);
				m_send(2,2,1,2);
				do{
					val=ping(chk[0])-ping(chk[1]);
					//usart_string("-");
					//usart_putdec(abs(val));
					//usart_string("\n\r");
				}while(val<turnvalue);
			}
			else if(val > turnvalue){
				m_send(1,2,1,2);
				m_send(2,1,1,2);
				do{
					val=ping(chk[0])-ping(chk[1]);
					//usart_string("+");
					//usart_putdec(abs(val));
					//usart_string("\n\r");
				}while(val>turnvalue);
			}
			else{
				lcd_clear();
				//lcd_putstr(LCD1_TWI,"Finished!");
			//	usart_string("Needless\n\r");
				return;
			}
			motor::brake(1);
			motor::brake(2);
			motor::wait();
			lcd_clear();
			//lcd_putstr(LCD1_TWI,"Succees");
			//usart_string("Succees!\n\r");
			//_delay_ms(1000);
			//lcd_clear();
			return;
			//lcd_putdec(LCD1_TWI,val);
			//_delay_ms(2);
		}
	}
	
	uint8_t notify_position(void){
		int16_t dis[10];
		for (uint8_t i=0;i<=7;i++){
			dis[i]=ping(i);
		}
		if(smaller_s(dis[2],dis[3])>=Sikiti&&rose>=smaller_s(dis[2],dis[3])){
			lcd_clear();
			lcd_putstr(LCD1_TWI,"NotifyP");
			motor::move(2);
			motor::move(0);
			motor::move(3);
			motor::move(4);
			lcd_clear();
			return 1;
		}
		else if(smaller_s(dis[5],dis[6])>=Sikiti&&rose>=smaller_s(dis[5],dis[6])){
			lcd_clear();
			lcd_putstr(LCD1_TWI,"NotifyP");
			motor::move(3);
			motor::move(0);
			motor::move(2);
			motor::move(4);
			lcd_clear();
			return 1;
		}
		else{return 0;}
			
	return 0;
	}
	
	
	uint8_t notify_half(void){
		uint8_t dis[10];
		uint8_t i = 0;
		dis[0] = check_ping(2);
		dis[1] = check_ping(3);
		dis[2] = check_ping(6);
		dis[3] = check_ping(5);
		if(dis[0]==1&&dis[1]==0){
			i=1;
		}else if(dis[0]==0&&dis[1]==1){
			i=1;	
		}else if(dis[2]==1&&dis[3]==0){
			i=1;
		}else if(dis[2]==0&&dis[3]==1){
			i=1;
		}else{
			i=0;
		}if(i==1){
			lcd_clear();
			lcd_putstr(LCD1_TWI,"NotifyHA");
			serial.string("NotifyHalf");
			serial.string("\n\r");
			motor::move(6);
			lcd_clear();
		}
		return i;
	}
	uint8_t notify_long_ex(void){
		int16_t dis[3];
		uint8_t dev;
		dis[0] = ping(1);
		if(dis[0]>=longway){
			led(Blueled,1);
			lcd_clear();
			lcd_putstr(LCD1_TWI,"NotifyL");
			led(Blueled,1);
			motor::turn_fix();
			lcd_clear();
			lcd_putstr(LCD1_TWI,"NotifyL");
			if (PORTJ.IN && PIN7_bm == 0){
				dev = 0x80;
			}
			else if (PORTJ.IN && PIN6_bm == 0){
				dev = 0x40;
			}
			else{
				dev = 0x00;
			}
			motor::move(0);
			motor::turn_fix();
			lcd_clear();
			lcd_putstr(LCD1_TWI,"NotifyL");
			motor::move(0);
			motor::turn_fix();
			lcd_clear();
			lcd_putstr(LCD1_TWI,"NotifyL");
			motor::move(0);
			motor::turn_fix();
			lcd_clear();
			lcd_putstr(LCD1_TWI,"NotifyL");
			dis[0] = ping(1);
			if(gbbest<=dis[0]){
				led(Redled,1);
				do{
					m_send(1,2,7,1);
					m_send(2,2,7,1);
					dis[0] = ping(1);
					dis[1] = smaller_s(ping(2),ping(3));
					dis[2] = smaller_s(ping(5),ping(6));/*
					usart_putdec(dis[1]);
					usart_putdec(dis[2]);
					usart_string("\n\r");*/

				}while(/*PORTJ.IN&dev == 0 && */(dis[2]<=Sikiti)&&(dis[1]<=Sikiti)&&(dis[0] >= gbbest));
				motor::brake(1);
				motor::brake(2);
			}
			motor::move(6);
			led(Blueled,0);
			led(Redled,0);
			serial.string("saka!");
			serial.string("\n\r");
			motor::fix_position();
			//w_w();
			return 1;
		}
		else{
			return 0;
		}
	}
	uint8_t notify_long(void){
		int16_t dis;
		dis = ping(1);
		if(dis >= longway){
			lcd_clear();
			lcd_putstr(LCD1_TWI,"NotifyL");
			led(Blueled,1);
			motor::turn_fix();
			lcd_clear();
			lcd_putstr(LCD1_TWI,"NotifyL");
			motor::move(0);
			motor::turn_fix();
			lcd_clear();
			lcd_putstr(LCD1_TWI,"NotifyL");
			motor::move(0);
			motor::turn_fix();
			lcd_clear();
			lcd_putstr(LCD1_TWI,"NotifyL");
			motor::move(0);
			motor::turn_fix();
			lcd_clear();
			lcd_putstr(LCD1_TWI,"NotifyL");

			dis = ping(1);
			if(dis >= gbbest){
				led(Redled,1);
				do{
					m_send(1,2,m_speed,1);
					m_send(2,2,m_speed,1);
					dis = ping(1);
				}while(dis >= gbbest);
				motor::brake(1);
				motor::brake(2);
				//motor::wait();
				led(Redled,0);
			}
			serial.string("saka!");
			serial.string("\n\r");
			led(Blueled,0);
			motor::gb_fix();
			lcd_clear();
			return 1;
		}
		else{
			return 0;
		}
	}
	// uint8_t fix_position(void){
	// 	uint8_t i=0;
	// 	uint8_t e=0;
	// 	e = motor::notify_long();
	// 	if(!e){motor::notify_half();}
	// 	motor::turn_fix();
	// 	//i=motor::notify_position();
	// 	motor::gb_fix();
	// 	motor::turn_fix();
	// 	return e;
	// }
	void fix_position(void){
		//notify_half();
		turn_fix();
		gb_fix();
		turn_fix();
		return;
	}
}

