/*
 * main.cpp
 *
 * Created: 2018/08/25 0:59:49
 *  Author: TOMOKI
 */ 

#include "source/petal.hpp"
#include "source/mv_control.hpp"
usart serial(&USARTC0,&PORTC);
#include "source/gyro_control.hpp"

int main(){	
	init_all();
	init_mv();
	//finded_victim(1);
	_delay_ms(200);
	/*lcd_putstr(LCD1_TWI,"Hello");
	motor::wait();
	lcd_clear();
	lcd_putstr(LCD1_TWI,"Ready!");
	lcd_clear();*/
	serial.string("wake_up\n");
	uint8_t s = 0;
	//_delay_ms(500);
	////init_mpu();
	write_walls();
	real_dfs(np,ta.r_now());
	while(1);
	gyro_cali();
	while(1){
		gyro_mes();
	}
	select_bus(7);
	while(1){
		sermo_test();
		serial.string("\n\r");
	}
	while(1){
		mv_cap(1,true);
		motor::move(1);
		led(Redled,1);
		_delay_ms(1000);
		led(Redled,0);
	}
	while(1){
		if((PORTJ.IN & PIN5_bm)==0){
			_delay_ms(1);
			check_mv(1);
			_delay_ms(100);
			mv_cap(1,false);
		}
		if(SW1){
			while(SW1);
		}
		if (SW2){
			mv_cap(1,false);
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
		hidarite(); //����@
		//buzzer();
		//serial.string("saka\n");
		//nachylenie();  //��
		//serial.string("k_start\n");
		float_killer();//�ċA
	//	serial.string("k_end\n");
		//float_killer2();//�O�Ɠ��������̓z�B
		//serial.string("k2_end\n");
	}
	//motor::notify_long_ex();
	*/
	//while(1){
		//debugping(v::left);
		//debugping(v::front);
		//debugping(v::right);
		//debugping(v::back);
	return 0;
}