/*
 * main.cpp
 *
 * Created: 2018/08/25 0:59:49
 *  Author: TOMOKI
 */ 

#include "source/petal.hpp"
<<<<<<< HEAD
=======
#include "source/gyro_control.hpp"
#include "source/mv_control.hpp"
>>>>>>> b17a220c847f593cf9d83065e55e0a131cc126f2
usart serial(&USARTC0,&PORTC);

<<<<<<< HEAD
int main(){
    init_all();
    _delay_ms(200);
    lcd_putstr(LCD1_TWI,"Hello");
    motor::wait();
    lcd_clear();
    lcd_putstr(LCD1_TWI,"Ready!");
    //lcd_clear();
    serial.string("wake_up\n");
 	write_walls();
	lcd_putdec(LCD1_TWI,check_ping(v::left));
	_delay_ms(1000);
	lcd_clear();
// 	while(1){
// 		write_walls();
// 		_delay_ms(1000);
// 	}
 	real_dfs(np,ta.r_now());
	//real_dfs(np,ta.r_now());
    //real_dfs(np,ta.r_now());
    //real_dfs(np,ta.r_now());
	//real_dfs(np,ta.r_now());
	/*while(1){
		lcd_putdec(LCD1_TWI,check_ping(v::front));
		//lcd_putstr(LCD1_TWI,"\n");
		//lcd_putdec(LCD1_TWI,ping(5));
		_delay_ms(300);
		lcd_clear();
	}*/
// 	while(1){
// 		lcd_putdec(LCD1_TWI,check_ping(v::left));
// 		_delay_ms(200);
// 		lcd_clear();
// 	}
    return 0;
=======
int main(){	
	init_all();
	init_mv();
	finded_victim(1);
	_delay_ms(200);
	lcd_putstr(LCD1_TWI,"Hello");
	motor::wait();
	lcd_clear();
	lcd_putstr(LCD1_TWI,"Ready!");
	lcd_clear();
	serial.string("wake_up\n");
	uint8_t s = 0;
	while(1){
		if(SW1){
			PORTB.OUTCLR=PIN0_bm|PIN1_bm;
			m_send(2,2,m_speed,1);
			m_send(1,2,m_speed,1);
			serial.string("sw1\n");
			while(SW1);
		}
		if (SW2){
			//check_mv(1);
			uint8_t res = mv_spi_send(1,1);
			serial.putdec(res);
			serial.string("sw2\n");
			while(SW2);
		}
		if (SW3){
			mv_cap(1,true);
			serial.string("sw3\n");
			while(SW3);
		}
	}
	while(1){
		if(SW1){
			mv_cap(1,false);
			serial.string("push1\n\r");
			while(SW1);
		}
		if(SW2){
			mv_cap(1,true);
			serial.string("push2\n\r");
			while(SW2);
		}
		if(SW3){
			check_mv(1);
			serial.putdec(s);
			while(SW3);
		}
	}
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
>>>>>>> b17a220c847f593cf9d83065e55e0a131cc126f2
}