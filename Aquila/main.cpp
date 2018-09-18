/*
 * main.cpp
 *
 * Created: 2018/08/25 1:27:46
 *  Author: TOMOKI
 */ 

#include "source/petal.hpp"
#include "source/gyro_control.hpp"
#include "source/mv_control.hpp"
usart serial(&USARTC0,&PORTC);
void print_num(int num){serial.putdec(num);serial.string("\n");}
void print_num(int num,int num2,int num3){
	serial.putdec(num);
	serial.string(" , ");
	serial.putdec(num2);
	serial.string(" , ");
	serial.putdec(num3);
	serial.string("\n");
}
void lcd_num(int x,int y,int z){
	lcd_putdec(LCD1_TWI,x);
	_delay_ms(1000);
	lcd_clear();
	lcd_putdec(LCD1_TWI,y);
	_delay_ms(1000);
	lcd_clear();
	lcd_putdec(LCD1_TWI,z);
	_delay_ms(1000);
	lcd_clear();
}
void debugping(int direction){
	int x,x2;
	switch(direction){
		case v::left:
			x=1;
			x2=2;
			break;
		case v::front:
			x=3;
			x2=3;
			break;
		case v::right:
			x=4;
			x2=5;
			break;
		case v::back:
			x=6;
			x2=6;
			break;
		default:
			break;
	}
	lcd_num(direction,x,x2);
	lcd_num(ping(x),ping(x2),for_cp(direction));
	write_walls();
	lcd_num(for_cp(direction),for_write_walls1(direction),ta.r_wall(direction));
	serial.string("\n");
	lcd_putstr(LCD1_TWI,"end");
	_delay_ms(200);
	lcd_clear();
	//ping->for_cp(ping_control)->for_write_walls1->write_walls(petal)
}

int main(){	
	init_all();
	init_mv();
	_delay_ms(200);
	uint8_t s = 30;
	while(1){
		s=mv_spi_send(30,1);
		serial.putdec(s);
		serial.string("\n\r");
	}
	lcd_putstr(LCD1_TWI,"Hello");
	//motor::wait();
	lcd_clear();
	lcd_putstr(LCD1_TWI,"Ready!");
	//lcd_clear();
	serial.string("wake_up\n");
// 	while(1){
// 		dserial.putdec(1);
// 	}serial.string("wake_up\n");
	serial.putdec(3.14*100);
	init_bmx055();
	//while(1){
		//motor::move(0);
	//}
	bmx_test();
	//while(1){
		//BMX055_Accl();
		//serial.string("Accl= ");
		//serial.putdec(xAccl);
		//serial.string(",");
		//serial.putdec(yAccl);
		//serial.string(",");
		//serial.putdec(zAccl);
		//serial.string(",");
		//BMX055_Gyro();
		//serial.string("Gyro= ");
		//serial.putdec(xGyro);
		//serial.string(",");
		//serial.putdec(yGyro);
		//serial.string(",");
		//serial.putdec(zGyro);
		//serial.string(",");
		//BMX055_Mag();
		//serial.string("Mag= ");
		//serial.putdec(xMag);
		//serial.string(",");
		//serial.putdec(yMag);
		//serial.string(",");
		//serial.putdec(zMag);
		//serial.string("\n\r");
		//_delay_ms(500);
	//}
	//
	////////////////////////////////////////////////
	while(1){
		if(SW1){
			motor::gb_fix();
		}
		if(SW2){
			motor::turn_fix();
		}
		if(SW3){
			motor::fix_position();
		}
		motor::move(0);
	}
	while(1){
		for(int i=1;i<=6;i++){
			serial.puthex(ping(i));
			serial.string(",");
		}
		serial.string("\n\r");
	}
	/*
    while(1){
	  motor::move(0);
	  motor::move(1);
    }
	while(1){
		if(SW1){
			motor::notify_long_ex();
		}
	}
	////////////////////////////////////////////////
	while(1){
		write_walls();
		hidarite(); //左手法
		//buzzer();
		//serial.string("saka\n");
		//nachylenie();  //坂
		//serial.string("k_start\n");
		float_killer();//再帰
	//	serial.string("k_end\n");
		//float_killer2();//前と同じ原理の奴。
		//serial.string("k2_end\n");
	}
	//motor::notify_long_ex();
	*/
	while(1){
		debugping(v::left);
		debugping(v::front);
		debugping(v::right);
		debugping(v::back);
		_delay_ms(500);
	}
	return 0;
}