/*
 * gyro_control.h
 *
 * Created: 2018/08/28 11:18:56
 *  Author: shun2
 */ 


#ifndef GYRO_CONTROL_H_
#define GYRO_CONTROL_H_

#include <avr/io.h>
#include <util/delay.h>
#include "xmega_usart.hpp"
#include "xmega_twi.hpp"

#define bmx_fre 400000

#define Addr_Accl 0x19  // (JP1,JP2,JP3 = Openの時)
// BMX055　ジャイロセンサのI2Cアドレス
#define Addr_Gyro 0x69 // (JP1,JP2,JP3 = Openの時)
// BMX055　磁気センサのI2Cアドレス
#define Addr_Mag 0x13   // (JP1,JP2,JP3 = Openの時)

//extern float xAccl,yAccl,zAccl,xGyro,yGyro,zGyro;
//extern int yMag,xMag,zMag;

void init_bmx055(void);
void BMX055_Accl(void);
void BMX055_Gyro(void);
void BMX055_Mag(void);
void bmx_test(void);
#endif /* GYRO_CONTROL_H_ */