/*
 * main.cpp
 *
 * Created: 2018/08/25 0:59:49
 *  Author: TOMOKI
 */ 

#include "source/petal.hpp"
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
=======
int main(){	
	init_all();
	_delay_ms(200);
	lcd_putstr(LCD1_TWI,"Hello");
	//motor::wait();
	lcd_clear();
	lcd_putstr(LCD1_TWI,"Ready!");
	//lcd_clear();
	serial.string("wake_up\n");
// 	while(1){
// 		serial.putdec(1);
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
>>>>>>> 2ab421225dbd27dcc700b47476d8c852bc03cc8d
	while(1){
		write_walls();
		hidarite();
	}
    return 0;
}