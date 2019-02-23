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
#include <avr/io.h>
#include "lcd_control.hpp"
#include "xmega_spi.hpp"
#include "ping_control.hpp"
#include "ui_control.hpp"
#include "mv_control.hpp"
#include "data_structure.hpp"
//#include "initializing.hpp"
#include "gyro_control.hpp"
#include <stdlib.h>
#include <util/delay.h>
//#include "core.hpp"
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
float Saved_angle=0;

uint8_t data=000;

void init_motor(void){
	PORTB.DIRSET=PIN0_bm|PIN1_bm|PIN2_bm|PIN3_bm;
	PORTB.OUTSET=PIN0_bm|PIN1_bm|PIN2_bm|PIN3_bm;
	PORTB.OUTCLR=PIN0_bm|PIN1_bm;
}
void Save_angle(void){
	Saved_angle=gyro_angle();
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
	PORTB.OUTSET=PIN2_bm|PIN3_bm;
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
	_delay_us(10);
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
	float b_angle=0.0;
	
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
	uint8_t status(uint8_t m){//1:free 2:busy
		return mspi(0,m);
	}
	void wait(bool check){
		if((PORTJ.IN & PIN5_bm)==0 && check==true){
			mv_cap(1,false);
			mv_cap(2,false);
			mv_cap(3,false);
			check_mv(1);
			check=false;
		}
		if((PORTJ.IN & PIN6_bm)==0 && check==true){
			mv_cap(1,false);
			mv_cap(2,false);
			mv_cap(3,false);
			check_mv(2);
			check=false;
		}
		if((PORTJ.IN & PIN7_bm)==0 && check==true){
			mv_cap(1,false);
			mv_cap(2,false);
			mv_cap(3,false);
			check_mv(3);
			check=false;
		}
		while(mspi(0,1)!=1){
			if((PORTJ.IN & PIN5_bm)==0 && check==true){
				mv_cap(1,false);
				mv_cap(2,false);
				mv_cap(3,false);
				check_mv(1);
				check=false;
			}
			if((PORTJ.IN & PIN6_bm)==0 && check==true){
				mv_cap(1,false);
				mv_cap(2,false);
				mv_cap(3,false);
				check_mv(2);
				check=false;
			}
			if((PORTJ.IN & PIN7_bm)==0 && check==true){
				mv_cap(1,false);
				mv_cap(2,false);
				mv_cap(3,false);
				check_mv(3);
				check=false;
			}
		}
		while(mspi(0,2)!=1){
			if((PORTJ.IN & PIN5_bm)==0 && check==true){
				mv_cap(1,false);
				mv_cap(2,false);
				mv_cap(3,false);
				check_mv(1);
				check=false;
			}
			if((PORTJ.IN & PIN6_bm)==0 && check==true){
				mv_cap(1,false);
				mv_cap(2,false);
				mv_cap(3,false);
				check_mv(2);
				check=false;
			}
			if((PORTJ.IN & PIN7_bm)==0 && check==true){
				mv_cap(1,false);
				mv_cap(2,false);
				mv_cap(3,false);
				check_mv(3);
				check=false;
			}
		}
		if((PORTJ.IN & PIN5_bm)==0 && check==true){
			mv_cap(1,false);
			mv_cap(2,false);
			mv_cap(3,false);
			check_mv(1);
			check=false;
		}
		if((PORTJ.IN & PIN6_bm)==0 && check==true){
			mv_cap(1,false);
			mv_cap(2,false);
			mv_cap(3,false);
			check_mv(2);
			check=false;
		}
		if((PORTJ.IN & PIN7_bm)==0 && check==true){
			mv_cap(1,false);
			mv_cap(2,false);
			mv_cap(3,false);
			check_mv(3);
			check=false;
		}
		return;
	}

	/*
	void wait(bool check){
		bool chk[3]={true,true,true};
		if(check==false){
			chk[0]=false;
			chk[1]=false;
			chk[2]=false;
		}
		else{
			chk[0]=true;
			chk[1]=true;
			chk[2]=true;
		}
		if((PORTJ.IN & PIN5_bm)==0 && chk[0]==true){
			mv_cap(1,false);
			mv_cap(2,false);
			mv_cap(3,false);
			check_mv(1);
			chk[0]=false;
		}
		if((PORTJ.IN & PIN6_bm)==0 && chk[1]==true){
			mv_cap(1,false);
			mv_cap(2,false);
			mv_cap(3,false);
			check_mv(2);
			chk[1]=false;
		}
		if((PORTJ.IN & PIN7_bm)==0 && chk[2]==true){
			mv_cap(1,false);
			mv_cap(2,false);
			mv_cap(3,false);
			check_mv(3);
			chk[2]=false;
		}
		while(mspi(0,1)!=1){
			if((PORTJ.IN & PIN5_bm)==0 && chk[0]==true){
				mv_cap(1,false);
				mv_cap(2,false);
				mv_cap(3,false);
				check_mv(1);
				chk[0]=false;
			}
			if((PORTJ.IN & PIN6_bm)==0 && chk[1]==true){
				mv_cap(1,false);
				mv_cap(2,false);
				mv_cap(3,false);
				check_mv(2);
				chk[1]=false;
			}
			if((PORTJ.IN & PIN7_bm)==0 && chk[2]==true){
				mv_cap(1,false);
				mv_cap(2,false);
				mv_cap(3,false);
				check_mv(3);
				chk[2]=false;
			}
		}
		while(mspi(0,2)!=1){
			if((PORTJ.IN & PIN5_bm)==0 && chk[0]==true){
				mv_cap(1,false);
				mv_cap(2,false);
				mv_cap(3,false);
				check_mv(1);
				chk[0]=false;
			}
			if((PORTJ.IN & PIN6_bm)==0 && chk[1]==true){
				mv_cap(1,false);
				mv_cap(2,false);
				mv_cap(3,false);
				check_mv(2);
				chk[1]=false;
			}
			if((PORTJ.IN & PIN7_bm)==0 && chk[2]==true){
				mv_cap(1,false);
				mv_cap(2,false);
				mv_cap(3,false);
				check_mv(3);
				chk[2]=false;
			}
		}
		if((PORTJ.IN & PIN5_bm)==0 && chk[0]==true){
			mv_cap(1,false);
			mv_cap(2,false);
			mv_cap(3,false);
			check_mv(1);
			chk[0]=false;
		}
		if((PORTJ.IN & PIN6_bm)==0 && chk[1]==true){
			mv_cap(1,false);
			mv_cap(2,false);
			mv_cap(3,false);
			check_mv(2);
			chk[1]=false;
		}
		if((PORTJ.IN & PIN7_bm)==0 && chk[2]==true){
			mv_cap(1,false);
			mv_cap(2,false);
			mv_cap(3,false);
			check_mv(3);
			chk[2]=false;
		}
		return;
	}*/
	void move(uint8_t x=6){// x = 0:1 block Advance 1:2 blocks Advance 2:Right Turn with Gyro 3:Left Turn with Gyro 4:1 block Back 5:2 block Back 6:Half block Advance 7:Half block Back 8:right Turn without Compass 9:left Turn without Compass 
		_delay_ms(5);
		float first = 0;
		float now = 0;
		bool st_f = false;
		b_angle=gyro_angle();
		mv_cap(1,true);
		mv_cap(2,true);
		mv_cap(3,true);
		motor::wait();
		switch(x){
			case 0: //1block advance
				m_send(2,2,m_speed,1);
				m_send(1,2,m_speed,1);
				st_f=true;
				//_delay_ms(300);
			break;
			case 1: //2block advance
				m_send(1,2,m_speed,2);
				m_send(2,2,m_speed,2);
				st_f=true;
				//_delay_ms(300);
			break;
			case 2: //Right turn with gyro
				first = gyro_angle();
				//serial.putint(first);
				//serial.string("\n\r");
				turn_retry2:
				m_send(1,2,m_turnspeed,5);
				m_send(2,1,m_turnspeed,5);
				_delay_ms(2);
				now = gyro_angle();
				//serial.putint(now);
				//serial.string("\n\r");
				do{
					now=gyro_angle();
					//serial.putint(now);
					//serial.string("\n\r");
					if(motor::status(1)==1||motor::status(2)==1){	
						m_send(1,2,m_turnspeed,5);
						m_send(2,1,m_turnspeed,5);
					}
				}while((first<now?now-first:now-first+360)>270||(first<now?now-first:now-first+360)<90);
				motor::brake(1);
				motor::brake(2);
				_delay_ms(100);
				now=gyro_angle();
				if((first<now?now-first:now-first+360)>270||(first<now?now-first:now-first+360)<90){
					goto turn_retry2;
				}
				if(first<=90){
					fix_angle_v(first-90+360);
				}
				else{
					fix_angle_v(first-90);
				}
			break;
			case 3: //Left Turn with gyro
				first = gyro_angle();
				//serial.putint(first);
				//serial.string("\n\r");
				turn_retry3:
				m_send(1,1,m_turnspeed,5);
				m_send(2,2,m_turnspeed,5);
				_delay_ms(2);
				now=gyro_angle();
				//serial.putint(now);
				//serial.string("\n\r");
				do{
					now=gyro_angle();
					//serial.putint(now);
					//serial.string("\n\r");
					if(motor::status(1)==1||motor::status(2)==1){	
						m_send(1,1,m_turnspeed,5);
						m_send(2,2,m_turnspeed,5);
					}
				}while((first<now?now-first:now-first+360)<90||(first<now?now-first:now-first+360)>270);
				motor::brake(1);
				motor::brake(2);
				now=gyro_angle();
				_delay_ms(100);
				if((first<now?now-first:now-first+360)<90||(first<now?now-first:now-first+360)>270){
					goto turn_retry3;
				}
				if(first>=270){
					fix_angle_v(first+90-360);
				}
				else{
					fix_angle_v(first+90);
				}
				break;
			case 4: //1block back
				m_send(1,1,m_speed,1);
				m_send(2,1,m_speed,1);
				st_f=true;
			break;
			case 5: //2block back
				m_send(1,1,m_speed,2);
				m_send(2,1,m_speed,2);
				st_f=true;
			break;

			case 6: //half advance
				m_send(1,2,m_speed,4);
				m_send(2,2,m_speed,4);
				st_f=true;
			break;
			case 7: //half back
				m_send(1,1,m_speed,4);
				m_send(2,1,m_speed,4);
				st_f=true;
			break;
			case 8: //Left Turn without gyro (????)
				m_send(1,2,m_turnspeed,3);
				m_send(2,1,m_turnspeed,3);
				//_delay_ms(300);
			break;
			case 9: //Right Turn without gyro (???)
				m_send(1,1,m_turnspeed,3);
				m_send(2,2,m_turnspeed,3);
			break;
			default:
				motor::brake(1);
				motor::brake(2);
			break;
		}
		motor::wait();
		if(st_f==true){
			fix_angle_v(b_angle);
		}
		mv_cap(1,true);
		mv_cap(2,true);
		mv_cap(3,true);
		_delay_ms(200);
	}
	void forever(void){
		m_send(1,1,5,0);
		m_send(2,1,5,0);
		while(ping(3)>=3);
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
		dis[0]=ping(3);//Forward
		dis[1]=ping(6);//Back
		if(Sikiti>=dis[0]){
			_delay_ms(1);
			dis[0]=ping(3);
			if(!(Sikiti>=dis[0])){
				return;
			}
			lcd_clear();
			lcd_putstr(LCD1_TWI,"gb_fixF");
			if((gbbest-dis[0])<fixno*-1){
				m_send(1,2,1,2);
				m_send(2,2,1,2);
				while(dis[0]>gbbest){
					if(dis[0]>=longway){
						break;
					}
					if(motor::status(1)==1||motor::status(2)==1){
						m_send(1,2,1,2);
						m_send(2,2,1,2);
					}
					dis[0]=ping(3);
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
					if(motor::status(1)==1||motor::status(2)==1){
						m_send(1,1,1,2);
						m_send(2,1,1,2);
					}
					dis[0]=ping(3);
				}
				motor::brake(1);
				motor::brake(2);
			}
		}
		else if(Sikiti>=dis[1]){
			_delay_ms(1);
			dis[1]=ping(6);
			if(!(Sikiti>=dis[1])){
				return;
			}
			lcd_clear();
			lcd_putstr(LCD1_TWI,"gb_fixB");
			if((gbbest-dis[1])>fixno){
				m_send(1,2,1,2);
				m_send(2,2,1,2);
				while(dis[1]<gbbest){
					if(dis[1]>=longway){
						break;
					}
					if(motor::status(1)==1||motor::status(2)==1){
						m_send(1,2,1,2);
						m_send(2,2,1,2);
					}
					dis[1]=ping(6);
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
					if(motor::status(1)==1||motor::status(2)==1){
						m_send(1,1,1,2);
						m_send(2,1,1,2);
					}
					dis[1]=ping(6);
				}
				motor::brake(1);
				motor::brake(2);
			}
		}
		else{}
		lcd_clear();
		return;
	}
	const int32_t turnvalue = 1;
	void turn_fix(uint8_t force){
		int val=0;
		uint8_t chk[2]={0};
		if (!force&&smaller_s(ping(1),ping(2))>=Sikiti&&smaller_s(ping(4),ping(5))>=Sikiti){
			return;
		}
		else{
			if (smaller_s(ping(1),ping(2))<=Sikiti){
				if(!(ping(2)<=Sikiti&&ping(1)<=Sikiti)){
					return;
				}
			}
			if (smaller_s(ping(5),ping(4))<=Sikiti){
				if(!(ping(5)<=Sikiti&&ping(4)<=Sikiti)){
					return;
				}
			}
			int ldiff = (ping(2)+ping(1))/2;
			int rdiff = (ping(5)+ping(4))/2;
			
			if(rdiff>ldiff){
				chk[0]=1;
				chk[1]=2;
				//usart_string("use Left!\n\r");
			}
			else if(rdiff<ldiff){
				chk[0]=4;
				chk[1]=5;
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
				lcd_clear();
				lcd_putstr(LCD1_TWI,"FixingTurn");
				m_send(1,1,1,2);
				m_send(2,2,1,2);
				do{
					val=ping(chk[0])-ping(chk[1]);
					if(motor::status(1)==1||motor::status(2)==1){
						m_send(1,1,1,2);
						m_send(2,2,1,2);
					}
				}while(val<turnvalue);
			}
			else if(val > turnvalue){
				lcd_clear();
				lcd_putstr(LCD1_TWI,"FixingTurn");
				m_send(1,2,1,2);
				m_send(2,1,1,2);
				do{
					val=ping(chk[0])-ping(chk[1]);
					if(motor::status(1)==1||motor::status(2)==1){
						m_send(1,2,1,2);
						m_send(2,1,1,2);
					}
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
			//motor::wait();
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
		if(smaller_s(dis[2],dis[1])>=Sikiti&&rose>=smaller_s(dis[1],dis[2])){
			lcd_clear();
			lcd_putstr(LCD1_TWI,"NotifyP");
			motor::move(2);
			motor::move(0);
			motor::move(3);
			motor::move(4);
			lcd_clear();
			return 1;
		}
		else if(smaller_s(dis[5],dis[4])>=Sikiti&&rose>=smaller_s(dis[5],dis[4])){
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
	
	
	uint8_t notify_half(uint8_t x){
		uint8_t dis[10];
		uint8_t i = 0;
		dis[0] = c_p(1);
		dis[1] = c_p(2);
		dis[2] = c_p(5);
		dis[3] = c_p(4);
		if(dis[0]==1&&dis[1]!=1){
			i=2;
		}else if(dis[0]!=1&&dis[1]==1){
			i=2;
		}else if(dis[2]==1&&dis[3]!=1){
			i=2;
		}else if(dis[2]!=1&&dis[3]==1){
			i=2;
		}else{
			i=0;
		}
		if(i==2){
		dis[0] = c_p(1);
		dis[1] = c_p(2);
		dis[2] = c_p(5);
		dis[3] = c_p(4);
			if(dis[0]==1&&dis[1]!=1){
				i=1;
			}else if(dis[0]!=1&&dis[1]==1){
				i=1;
			}else if(dis[2]==1&&dis[3]!=1){
				i=1;
			}else if(dis[2]!=1&&dis[3]==1){
				i=1;
			}else{
				i=0;
			}
		}if(i==1){
			lcd_clear();
			lcd_putstr(LCD1_TWI,"NotifyHA");
			serial.string("NotifyHalf");
			serial.string("\n\r");
			if (x == v::front){
				motor::move(6);
			}
			else if(x == v::back){
				motor::move(7);
			}
			lcd_clear();
		}
		return i;
	}
	uint8_t notify_long_ex(void){
		int16_t dis[3];
		dis[0] = ping(3);
		if(dis[0]>=longway){
			led(Blueled,1);
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
			dis[0] = ping(3);
			if(gbbest<=dis[0]){
				led(Redled,1);
				do{
					m_send(1,2,7,1);
					m_send(2,2,7,1);
					dis[0] = ping(3);
					dis[1] = smaller_s(ping(2),ping(1));
					dis[2] = smaller_s(ping(5),ping(4));/*
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
		dis = ping(3);
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

			dis = ping(3);
			if(dis >= gbbest){
				led(Redled,1);
				do{
					m_send(1,2,m_speed,1);
					m_send(2,2,m_speed,1);
					dis = ping(3);
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
	
	uint8_t notify_long_acc(uint8_t x,bool buz){//0:なし,1:下り,2:上り
		float ac=acc_x_mes();
		uint8_t spos = 6;
		float now=0;
		if(x==v::front){//前進中
			if(ac>=Acc_thre_u){//上り
				if(longway<=ping(3)){
					ac=acc_x_mes();
					if(ac>=Acc_thre_u){
						if(buz==true){
						buzzer(400);
						buzzer(800);
						}
					}
					else{
						return 0;
					}
				}
				else{
					return 0;
				}
				lcd_clear();
				lcd_putstr(LCD1_TWI,"NotiL!U");
				m_send(1,2,spos,3);
				m_send(2,2,spos,3);
				ac=acc_x_mes();
				while(ac>=Acc_thre_u){
					ac=acc_x_mes();
					if(!(ac>=Acc_thre_u)){
						ac=acc_x_mes();
					}
					led(Redled,0);
					now=acc_y_mes();
					/*serial.putfloat(ac);
					serial.string(",");
					serial.putfloat(now);
					serial.string("\n\r");*/
					if(/*Acc_slope_thre*5>abs(now)&&*/abs(now)>Acc_slope_thre){//そこまで傾いていない
						if(now>Acc_slope_thre){//右向いてる
							error_led(2,1);
							error_led(1,0);
							do 
							{
								now=acc_y_mes();
								if(motor::status(1)==1||motor::status(2)==1){
									m_send(1,2,spos-2,3);
									m_send(2,2,spos,3);
								}
							} while (now>Acc_slope_thre);
						}
						else if(now<Acc_slope_thre*-1){//左を向いてる
							error_led(2,0);
							error_led(1,1);
							do
							{
								now=acc_y_mes();
								if(motor::status(1)==1||motor::status(2)==1){
									m_send(1,2,spos,3);
									m_send(2,2,spos-2,3);
								}
							} while (now<Acc_slope_thre*-1);
						}
					}
					/*else if(abs(now)>Acc_slope_thre*5){//大きく傾いている
						led(Redled,1);
						if(now>Acc_slope_thre){//右向いてる
							error_led(2,1);
							error_led(1,0);
						}
						else if(now<Acc_slope_thre*-1){//左を向いてる
							error_led(2,0);
							error_led(1,1);
						}
					}*/
					else{
						error_led(2,0);
						error_led(1,0);
					}
					if(motor::status(1)==1||motor::status(2)==1){
						m_send(1,2,spos,3);
						m_send(2,2,spos,3);
						error_led(2,0);
						error_led(1,0);
					}
				
					if(!(ac>=Acc_thre_u)){
						ac=acc_x_mes();
					}
				}
				led(Redled,0);
				error_led(2,0);
				error_led(1,0);
				ac=acc_x_mes();
				if(ac>=Acc_thre_u){
					notify_long_acc(x,false);
				}
				motor::move(10);
				ac=acc_x_mes();
				if(ac>=Acc_thre_u){
					notify_long_acc(x,false);
				}
				_delay_ms(1);
				lcd_clear();
				motor::move(7);
				return 2;
			}
			else if(ac<=Acc_thre_d){//下り
				if(longway/2<=ping(6)){
					ac=acc_x_mes();
					if(ac<=Acc_thre_d){
						if(buz==true){
						buzzer(800);
						buzzer(400);
						}
					}
					else{
						return 0;
					}
				}
				else{
					return 0;
				}
				lcd_clear();
				lcd_putstr(LCD1_TWI,"NotiL!D");
				m_send(1,2,spos,3);
				m_send(2,2,spos,3);
				ac=acc_x_mes();
				while(ac<=Acc_thre_d){
					ac=acc_x_mes();
					if(!(ac<=Acc_thre_d)){
						ac=acc_x_mes();
					}
					led(Redled,0);
					now=acc_y_mes();
					/*serial.putfloat(ac);
					serial.string(",");
					serial.putfloat(now);
					serial.string("\n\r");*/
					if(/*Acc_slope_thre*5>abs(now)&&*/abs(now)>Acc_slope_thre){//そこまで傾いていない
						if(now>Acc_slope_thre){//右向いてる
							error_led(2,1);
							error_led(1,0);
							do 
							{
								now=acc_y_mes();
								if(motor::status(1)==1||motor::status(2)==1){
									m_send(1,2,spos,3);
									m_send(2,2,spos-3,3);
								}
							} while (now>Acc_slope_thre);
						}
						else if(now<Acc_slope_thre*-1){//左を向いてる
							error_led(2,0);
							error_led(1,1);
							do
							{
								now=acc_y_mes();
								if(motor::status(1)==1||motor::status(2)==1){
									m_send(1,2,spos-3,3);
									m_send(2,2,spos,3);
								}
							} while (now<Acc_slope_thre*-1);
						}
					}
					/*else if(abs(now)>Acc_slope_thre*5){//大きく傾いている
						led(Redled,1);
						if(now>Acc_slope_thre){//右向いてる
							error_led(2,1);
							error_led(1,0);
						}
						else if(now<Acc_slope_thre*-1){//左を向いてる
							error_led(2,0);
							error_led(1,1);
						}
					}*/
					else{
						error_led(2,0);
						error_led(1,0);
					}
					if(motor::status(1)==1||motor::status(2)==1){
						m_send(1,2,spos,3);
						m_send(2,2,spos,3);
						error_led(2,0);
						error_led(1,0);
					}
				
					if(!(ac<=Acc_thre_d)){
						ac=acc_x_mes();
					}
				}
				led(Redled,0);
				error_led(2,0);
				error_led(1,0);
				ac=acc_x_mes();
				if(ac<=Acc_thre_d){
					notify_long_acc(x,false);
				}
				motor::move(10);
				ac=acc_x_mes();
				if(ac<=Acc_thre_d){
					notify_long_acc(x,false);
				}
				_delay_ms(1);
				lcd_clear();
				motor::move(7);
				return 1;
			}
		}else if(x==v::back){//後進中
			if(ac>=Acc_thre_u){//下り
				if(longway/2<=ping(3)){
					ac=acc_x_mes();
					if(ac>=Acc_thre_u){
						if(buz==true){
						buzzer(400);
						buzzer(800);
						}
					}
					else{
						return 0;
					}
				}
				else{
					return 0;
				}
				lcd_clear();
				lcd_putstr(LCD1_TWI,"NotiL!D");
				m_send(1,1,spos,3);
				m_send(2,1,spos,3);
				ac=acc_x_mes();
				while(ac>=Acc_thre_u){
					ac=acc_x_mes();
					if(!(ac>=Acc_thre_u)){
						ac=acc_x_mes();
					}
					led(Redled,0);
					now=acc_y_mes();
					if(/*Acc_slope_thre*5>abs(now)&&*/abs(now)>Acc_slope_thre){//そこまで傾いていない
						if(now>Acc_slope_thre){//右向いてる
							error_led(2,1);
							error_led(1,0);
							do 
							{
								now=acc_y_mes();
								if(motor::status(1)==1||motor::status(2)==1){
									m_send(2,1,spos-2,3);
									m_send(1,1,spos,3);
								}
							} while (now>Acc_slope_thre);
						}
						else if(now<Acc_slope_thre*-1){//左を向いてる
							error_led(2,0);
							error_led(1,1);
							do
							{
								now=acc_y_mes();
								if(motor::status(1)==1||motor::status(2)==1){
									m_send(2,1,spos,3);
									m_send(1,1,spos-2,3);
								}
							} while (now<Acc_slope_thre*-1);
						}
					}
					/*else if(abs(now)>Acc_slope_thre*5){//大きく傾いている
						led(Redled,1);
						if(now>Acc_slope_thre){//右向いてる
							error_led(2,1);
							error_led(1,0);
						}
						else if(now<Acc_slope_thre*-1){//左を向いてる
							error_led(2,0);
							error_led(1,1);
						}
					}*/
					else{
						error_led(2,0);
						error_led(1,0);
					}
					if(motor::status(1)==1||motor::status(2)==1){
						m_send(1,1,spos,3);
						m_send(2,1,spos,3);
						error_led(2,0);
						error_led(1,0);
					}
				
					if(!(ac>=Acc_thre_u)){
						ac=acc_x_mes();
					}
				}
				led(Redled,0);
				error_led(2,0);
				error_led(1,0);
				ac=acc_x_mes();
				if(ac>=Acc_thre_u){
					notify_long_acc(x,false);
				}
				motor::move(10);
				ac=acc_x_mes();
				if(ac>=Acc_thre_u){
					notify_long_acc(x,false);
				}
				_delay_ms(1);
				lcd_clear();
				motor::move(7);
				return 1;
			}
			else if(ac<=Acc_thre_d){//昇り
				if(longway<=ping(6)){
					ac=acc_x_mes();
					if(ac<=Acc_thre_d){
						if(buz==true){
						buzzer(800);
						buzzer(400);
						}
					}
					else{
						return 0;
					}
				}
				else{
					return 0;
				}
				lcd_clear();
				lcd_putstr(LCD1_TWI,"NotiL!U");
				m_send(1,1,spos,3);
				m_send(2,1,spos,3);
				ac=acc_x_mes();
				while(ac<=Acc_thre_d){
					ac=acc_x_mes();
					if(!(ac<=Acc_thre_d)){
						ac=acc_x_mes();
					}
					led(Redled,0);
					now=acc_y_mes();
					if(/*Acc_slope_thre*5>abs(now)&&*/abs(now)>Acc_slope_thre){//そこまで傾いていない
						if(now>Acc_slope_thre){//右向いてる
							error_led(2,1);
							error_led(1,0);
							do 
							{
								now=acc_y_mes();
								if(motor::status(1)==1||motor::status(2)==1){
									m_send(2,1,spos,3);
									m_send(1,1,spos-3,3);
								}
							} while (now>Acc_slope_thre);
						}
						else if(now<Acc_slope_thre*-1){//左を向いてる
							error_led(2,0);
							error_led(1,1);
							do
							{
								now=acc_y_mes();
								if(motor::status(1)==1||motor::status(2)==1){
									m_send(2,1,spos-3,3);
									m_send(1,1,spos,3);
								}
							} while (now<Acc_slope_thre*-1);
						}
					}
					/*else if(abs(now)>Acc_slope_thre*5){//大きく傾いている
						led(Redled,1);
						if(now>Acc_slope_thre){//右向いてる
							error_led(2,1);
							error_led(1,0);
						}
						else if(now<Acc_slope_thre*-1){//左を向いてる
							error_led(2,0);
							error_led(1,1);
						}
					}*/
					else{
						error_led(2,0);
						error_led(1,0);
					}
					if(motor::status(1)==1||motor::status(2)==1){
						m_send(1,1,spos,3);
						m_send(2,1,spos,3);
						error_led(2,0);
						error_led(1,0);
					}
				
					if(!(ac<=Acc_thre_d)){
						ac=acc_x_mes();
					}
				}
				led(Redled,0);
				error_led(2,0);
				error_led(1,0);
				ac=acc_x_mes();
				if(ac<=Acc_thre_d){
					notify_long_acc(x,false);
				}
				motor::move(10);
				ac=acc_x_mes();
				if(ac<=Acc_thre_d){
					notify_long_acc(x,false);
				}
				_delay_ms(1);
				lcd_clear();
				motor::move(7);
				return 2;
			}
		}
		else{
			lcd_clear();
			return 0;
		}
		return 0;
	}
	
	uint8_t notify_long_ang(uint8_t x,bool buz){//0:なし,1:下り,2:上り
		float ang=gyro_angle_y();
		float anx=gyro_angle_x();
		uint8_t spos = 6;
		if(x==v::front){//前進中
			if(ang<=Ang_slope_Norm-Ang_slope_thre){//上り
				lcd_clear();
				lcd_putstr(LCD1_TWI,"NotiL!U");
				buzzer(400);
				buzzer(800);
				m_send(1,2,spos,3);
				m_send(2,2,spos,3);
				while(ang<=Ang_slope_Norm-Ang_slope_thre){
					ang=gyro_angle_y();
					anx=gyro_angle_x();
					if(abs(anx-Ang_x_Norm)>Ang_x_thre){//そこまで傾いていない
						if(anx>Ang_x_Norm){//右向いてる
							error_led(2,1);
							error_led(1,0);
							do 
							{
								anx=gyro_angle_x();
								if(motor::status(1)==1||motor::status(2)==1){
									m_send(1,2,spos-2,3);
									m_send(2,2,spos,3);
								}
							} while (anx>Ang_x_Norm);
						}
						else if(anx<Ang_x_Norm){//左を向いてる
							error_led(2,0);
							error_led(1,1);
							do
							{
								anx=gyro_angle_x();
								if(motor::status(1)==1||motor::status(2)==1){
									m_send(1,2,spos,3);
									m_send(2,2,spos-2,3);
								}
							} while (anx<Ang_x_Norm);
						}
						m_send(1,2,spos,3);
						m_send(2,2,spos,3);
					}
					else{
						error_led(2,0);
						error_led(1,0);
					}
					if(motor::status(1)==1||motor::status(2)==1){
						m_send(1,2,spos,3);
						m_send(2,2,spos,3);
						error_led(2,0);
						error_led(1,0);
					}
				}
				return 2;
			}
			else if(ang>=Ang_slope_Norm+Ang_slope_thre){//下り
				lcd_clear();
				lcd_putstr(LCD1_TWI,"NotiL!D");
				buzzer(800);
				buzzer(400);
				m_send(1,2,spos,3);
				m_send(2,2,spos,3);
				while(ang>=Ang_slope_Norm+Ang_slope_thre){
					ang=gyro_angle_y();
					anx=gyro_angle_x();
					if(abs(anx-Ang_x_Norm)>Ang_x_thre){//そこまで傾いていない
						if(anx>Ang_x_Norm){//右向いてる
							error_led(2,1);
							error_led(1,0);
							do 
							{
								anx=gyro_angle_x();
								if(motor::status(1)==1||motor::status(2)==1){
									m_send(1,2,spos,3);
									m_send(2,2,spos-2,3);
								}
							} while (anx>Ang_x_Norm);
						}
						else if(anx<Ang_x_Norm){//左を向いてる
							error_led(2,0);
							error_led(1,1);
							do
							{
								anx=gyro_angle_x();
								if(motor::status(1)==1||motor::status(2)==1){
									m_send(1,2,spos-2,3);
									m_send(2,2,spos,3);
								}
							} while (anx<Ang_x_Norm);
						}
						m_send(1,2,spos,3);
						m_send(2,2,spos,3);
					}
					else{
						error_led(2,0);
						error_led(1,0);
					}
					if(motor::status(1)==1||motor::status(2)==1){
						m_send(1,2,spos,3);
						m_send(2,2,spos,3);
						error_led(2,0);
						error_led(1,0);
					}
				}
				return 1;
			}
		}else if(x==v::back){//後進中
			if(ang>=Ang_slope_Norm+Ang_slope_thre){//下り
				lcd_clear();
				lcd_putstr(LCD1_TWI,"NotiL!U");
				m_send(1,1,spos,3);
				m_send(2,1,spos,3);
				buzzer(400);
				buzzer(800);
				while(ang>=Ang_slope_Norm+Ang_slope_thre){
					ang=gyro_angle_y();
					anx=gyro_angle_x();
					if(abs(anx-Ang_x_Norm)>Ang_x_thre){//そこまで傾いていない
						if(anx>Ang_x_Norm){//右向いてる
							error_led(2,1);
							error_led(1,0);
							do 
							{
								anx=gyro_angle_x();
								if(motor::status(1)==1||motor::status(2)==1){
									m_send(2,1,spos,3);
									m_send(1,1,spos-2,3);
								}
							} while (anx>Ang_x_Norm);
						}
						else if(anx<Ang_x_Norm){//左を向いてる
							error_led(2,0);
							error_led(1,1);
							do
							{
								anx=gyro_angle_x();
								if(motor::status(1)==1||motor::status(2)==1){
									m_send(2,1,spos-2,3);
									m_send(1,1,spos,3);
								}
							} while (anx<Ang_x_Norm);
						}
						m_send(1,1,spos,3);
						m_send(2,1,spos,3);
					}
					else{
						error_led(2,0);
						error_led(1,0);
					}
					if(motor::status(1)==1||motor::status(2)==1){
						m_send(1,1,spos,3);
						m_send(2,1,spos,3);
						error_led(2,0);
						error_led(1,0);
					}
				}
				return 1;
			}
			else if(ang<=Ang_slope_Norm-Ang_slope_thre){//昇り
				lcd_clear();
				lcd_putstr(LCD1_TWI,"NotiL!U");
				m_send(1,1,spos,3);
				m_send(2,1,spos,3);
				buzzer(800);
				buzzer(400);
				while(ang<=Ang_slope_Norm-Ang_slope_thre){
					ang=gyro_angle_y();
					anx=gyro_angle_x();
					if(abs(anx-Ang_x_Norm)>Ang_x_thre){//そこまで傾いていない
						if(anx>Ang_x_Norm){//右向いてる
							error_led(2,1);
							error_led(1,0);
							do 
							{
								anx=gyro_angle_x();
								if(motor::status(1)==1||motor::status(2)==1){
									m_send(2,1,spos-2,3);
									m_send(1,1,spos,3);
								}
							} while (anx>Ang_x_Norm);
						}
						else if(anx<Ang_x_Norm){//左を向いてる
							error_led(2,0);
							error_led(1,1);
							do
							{
								anx=gyro_angle_x();
								if(motor::status(1)==1||motor::status(2)==1){
									m_send(2,1,spos,3);
									m_send(1,1,spos-2,3);
								}
							} while (anx<Ang_x_Norm);
						}
						m_send(1,1,spos,3);
						m_send(2,1,spos,3);
					}
					else{
						error_led(2,0);
						error_led(1,0);
					}
					if(motor::status(1)==1||motor::status(2)==1){
						m_send(1,1,spos,3);
						m_send(2,1,spos,3);
						error_led(2,0);
						error_led(1,0);
					}
				}
				return 2;
			}
		}
		else{
			lcd_clear();
			return 0;
		}
		return 0;
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
	void fix_position(uint8_t x){
		turn_fix();
		notify_half(x);
		gb_fix();
		turn_fix();
		//return;
	}
	
	void fix_angle(void){
		float now=0;
		uint8_t spos=1;
		float siki=1;//修正する閾値
		now=gyro_angle();
		if(abs(now-b_angle)>siki){
			lcd_clear();
			lcd_putstr(LCD1_TWI,"fix_angl");
			if(now-b_angle>0){
				if(abs(now-b_angle)<=180){
					m_send(1,2,spos,3);
					m_send(2,1,spos,3);
					do 
					{
						now=gyro_angle();
						if(motor::status(1)==1||motor::status(2)==1){
							m_send(1,2,spos,3);
							m_send(2,1,spos,3);
						}
					} while (abs(now-b_angle)>siki);
				}
				else if(abs(now-b_angle)>180){
					m_send(1,1,spos,3);
					m_send(2,2,spos,3);
					do
					{
						now=gyro_angle();
						if(motor::status(1)==1||motor::status(2)==1){
							m_send(1,1,spos,3);
							m_send(2,2,spos,3);
						}
					} while (abs(now-b_angle)>siki);
				}
			}
			else if(now-b_angle<0){
				if(abs(now-b_angle)<=180){
					m_send(1,1,spos,3);
					m_send(2,2,spos,3);
					do
					{
						now=gyro_angle();
						if(motor::status(1)==1||motor::status(2)==1){
							m_send(1,1,spos,3);
							m_send(2,2,spos,3);
						}
					} while (abs(now-b_angle)>siki);
				}
				else if(abs(now-b_angle)>180){
					m_send(1,2,spos,3);
					m_send(2,1,spos,3);
					do
					{
						now=gyro_angle();
						if(motor::status(1)==1||motor::status(2)==1){
							m_send(1,2,spos,3);
							m_send(2,1,spos,3);
						}
					} while (abs(now-b_angle)>siki);
				}
			}
		}
		lcd_clear();
		motor::brake(1);
		motor::brake(2);
		return;
	}void fix_angle_v(float angl){
		float now=0;
		uint8_t spos=1;
		float siki=1;//修正する閾値
		now=gyro_angle();
		if(abs(now-angl)>siki){
			lcd_clear();
			lcd_putstr(LCD1_TWI,"fix_ag_v");
			if(now-angl>0){
				if(abs(now-angl)<=180){
					m_send(1,2,spos,3);
					m_send(2,1,spos,3);
					do 
					{
						now=gyro_angle();
					} while (abs(now-angl)>siki);
				}
				else if(abs(now-angl)>180){
					m_send(1,1,spos,3);
					m_send(2,2,spos,3);
					do
					{
						now=gyro_angle();
					} while (abs(now-angl)>siki);
				}
			}
			else if(now-angl<0){
				if(abs(now-angl)<=180){
					m_send(1,1,spos,3);
					m_send(2,2,spos,3);
					do
					{
						now=gyro_angle();
					} while (abs(now-angl)>siki);
				}
				else if(abs(now-angl)>180){
					m_send(1,2,spos,3);
					m_send(2,1,spos,3);
					do
					{
						now=gyro_angle();
					} while (abs(now-angl)>siki);
				}
			}
		}
		lcd_clear();
		motor::brake(1);
		motor::brake(2);
		return;
	}
	void set_angle(float ang){
		float now=0;
		uint8_t spos=4;
		float siki=1;//修正する閾値
		now=gyro_angle();
		if(abs(now-ang)>siki){
			lcd_clear();
			lcd_putstr(LCD1_TWI,"set_ag_v");
			if(now-ang>0){
				if(abs(now-ang)<=180){
					m_send(1,2,spos,3);
					m_send(2,1,spos,3);
					do 
					{
						now=gyro_angle();
						if(motor::status(1)==1||motor::status(2)==1){
							m_send(1,2,spos,3);
							m_send(2,1,spos,3);
						}
					} while (abs(now-ang)>siki);
				}
				else if(abs(now-ang)>180){
					m_send(1,1,spos,3);
					m_send(2,2,spos,3);
					do
					{
						now=gyro_angle();
						if(motor::status(1)==1||motor::status(2)==1){
							m_send(1,1,spos,3);
							m_send(2,2,spos,3);
						}
					} while (abs(now-ang)>siki);
				}
			}
			else if(now-ang<0){
				if(abs(now-ang)<=180){
					m_send(1,1,spos,3);
					m_send(2,2,spos,3);
					do
					{
						now=gyro_angle();
						if(motor::status(1)==1||motor::status(2)==1){
							m_send(1,1,spos,3);
							m_send(2,2,spos,3);
						}
					} while (abs(now-ang)>siki);
				}
				else if(abs(now-ang)>180){
					m_send(1,2,spos,3);
					m_send(2,1,spos,3);
					do
					{
						now=gyro_angle();
						if(motor::status(1)==1||motor::status(2)==1){
							m_send(1,2,spos,3);
							m_send(2,1,spos,3);
						}
					} while (abs(now-ang)>siki);
				}
			}
		}
		lcd_clear();
		motor::brake(1);
		motor::brake(2);
		return;
	}
}

void enkaigei(void){
	float first=gyro_angle(), now=0;
	uint8_t spos=5;
	while(1){
		now=gyro_angle();
		if(abs(now-first)>1){
			if(now-first>0){
				if(abs(now-first)<=180){
					m_send(1,2,spos,3);
					m_send(2,1,spos,3);
				}
				else if(abs(now-first)>180){
					m_send(1,1,spos,3);
					m_send(2,2,spos,3);
				}
			}
			else if(now-first<0){
				if(abs(now-first)<=180){
					m_send(1,1,spos,3);
					m_send(2,2,spos,3);
				}
				else if(abs(now-first)>180){
					m_send(1,2,spos,3);
					m_send(2,1,spos,3);
				}
			}
		}
		else{
			motor::brake(1);
			motor::brake(2);
		}
	}
}