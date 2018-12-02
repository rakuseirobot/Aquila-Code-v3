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
	lcd_putstr(LCD1_TWI,"Hello");
	motor::wait();
	lcd_clear();
	lcd_putstr(LCD1_TWI,"Ready!");
	lcd_clear();
	serial.string("wake_up\n");
	uint8_t s = 0;
	//motor::move(1);
	write_walls();
	move(v::front);
	move(v::left);
	move(v::right);
	move(v::back);
	real_dfs(np,ta.r_now());
	while(1){
		buzzer();
	}
	while(1){
		ping_debug();
	}
	//_delay_ms(500);
	////init_mpu();
	while(1){
		mv_cap(1,true);
		motor::move(1);
		_delay_ms(500);
	}
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
		//debugping(v::back)



		/*
	lcd_clear();
	lcd_putstr(LCD1_TWI,"Ready!");
	lcd_clear();
<<<<<<< HEAD
	serial.string("wake_up\n");
	ta.append_node(v::front,1);
	ta.go_st();
	ta.append_node(v::front,1);
	ta.go_st();
	ta.append_node(v::left,1);
	ta.append_node(v::back,1);
	ta.find(1,1,1);
	lcd_putdec(LCD1_TWI,ta.r_vnum());


	*/
/*
=======
>>>>>>> 82752fb0e9336857bc727f9da255bfcbffa7c10c
	_delay_ms(1000);
	motor::fix_position();
	buzzer();
	write_walls();
	//real_dfs(np,ta.r_start());
	//st.push(ta.r_now());
	stack_dfs(ta.r_now());
	lcd_clear();
	lcd_putstr(LCD1_TWI,"HOME");
	stack_dfs(ta.r_start());
	lcd_clear();
	lcd_putstr(LCD1_TWI,"THE_END");
<<<<<<< HEAD
	return 0;
=======
*/	return 0;
<<<<<<< HEAD
=======
>>>>>>> a432630dccc103d6a2c52788f0dd3cea98f6b38f
>>>>>>> d1973c8107ec7ffc7ec39661514ab98df6158c03
>>>>>>> 82752fb0e9336857bc727f9da255bfcbffa7c10c
}