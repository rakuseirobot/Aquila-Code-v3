/*
 * gyro_control.cpp
 *
 * Created: 2018/08/28 11:18:39
 *  Author: shun2
 */ 

#include "gyro_control.hpp"

#include <avr/io.h>
#include <util/delay.h>
#include "xmega_usart.hpp"
#include "xmega_twi.hpp"
#include "initializing.hpp"
#include "ui_control.hpp"
twi gyro(&TWIC,250000);

void gyro_send(uint8_t reg,uint8_t dh,uint8_t dl){
	gyro.Address(0x50<<1,0); //J901との通信開始
	gyro.WriteSingle(reg);
	gyro.WriteSingle(dh);
	gyro.WriteSingle(dl);
	gyro.Stop(); //通信終了
}

float gyro_angle(void){
	int16_t d[2];
	jy_recv_wd(0x3f,d);
	return ((float)((d[1] << 8) | d[0])  / 32768 * 180) + 180;
}

float acc_x_mes(void){
	int16_t d[2];
	jy_recv_wd(0x34,d);
	return ((float)((d[1] << 8) | d[0])  / 32768 * 16 * 9.8);
}

float acc_y_mes(void){
	int16_t d[2];
	jy_recv_wd(0x35,d);
	return ((float)((d[1] << 8) | d[0])  / 32768 * 16 * 9.8);
}

void acc_mes(void){
	float data[3]={0,0,0};
	int16_t d[2];
	jy_recv_wd(0x34,d);
	data[0] = ((float)((d[1] << 8) | d[0])  / 32768 * 16 * 9.8);
	jy_recv_wd(0x35,d);
	data[1] = ((float)((d[1] << 8) | d[0])  / 32768 * 16 * 9.8);
	jy_recv_wd(0x36,d);
	data[2] = ((float)((d[1] << 8) | d[0])  / 32768 * 16 * 9.8);
	//angle=(angle-359)*(-1);
	serial.string("x:");
	serial.putfloat(data[0]);
	serial.string("y:");
	serial.putfloat(data[1]);
	serial.string("z:");
	serial.putfloat(data[2]);
	//serial.string("\n\r");
}
void gyro_time_mes(void){
	int16_t data[2];
	jy_recv_wd(0x30,data);
	serial.putint(data[0]);
	serial.string("年 ");
	serial.putint(data[1]);
	serial.string("月 ");
	jy_recv_wd(0x31,data);
	serial.putint(data[0]);
	serial.string("日 ");
	serial.putint(data[1]);
	serial.string("時 ");
	jy_recv_wd(0x32,data);
	serial.putint(data[0]);
	serial.string("分 ");
	serial.putint(data[1]);
	serial.string("秒 ");
	jy_recv_wd(0x33,data);
	serial.putint((data[1]<<8)|data[0]);
	serial.string("m秒");
	//serial.string("\n\r");
}

void gyro_mes(void){
	int16_t data[2];
	jy_recv_wd(0x3f,data);
	serial.putfloat(((float)((data[1] << 8) | data[0])  / 32768 * 180) + 180);
	//serial.string("\n\r");
	return;
}

void gyro_set200hz(void){
	gyro_send(0x03,0x0a,0);
	_delay_ms(500);
}
void gyro_Save(void){
	gyro_send(0,0,0);
}


void gyro_cali(void){
	//Z軸調整モード
	buzzer(600);
	gyro_send(0x01,0x01,0);//キャリブレーションモード選択
	_delay_ms(1000); //待機時間）（任意）
	//磁気センサキャリブレーションモード
	gyro_send(0x01,0x02,0);//キャリブレーションモード選択
	buzzer(600);
	_delay_ms(5000); //キャリブレーション時間（任意）
	gyro_send(0x01,0,0);//キャリブレーションモード終了
	_delay_ms(1000); //待機時間）（任意）
	gyro_send(0,0,0);//キャリブレーションモード終了
	_delay_ms(1000); //待機時間）（任意）
	buzzer(400);
}
void jy_recv_wd(uint8_t sd,int16_t *d){
	gyro.Address(0x50<<1,0); //J901との通信開始
	gyro.WriteSingle(sd); //z軸の角度を読み取るアドレスを送信
	gyro.Address(0x50<<1,1); //J901との通信開始
	*d = gyro.ReadSingle(1); //1byte目受信
	d++;
	*d = gyro.ReadSingle(0); //2byte目受信
	gyro.Stop();
	return;
}