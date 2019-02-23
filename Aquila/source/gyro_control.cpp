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
	gyro.Address(0x50<<1,0); //J901‚Æ‚Ì’ÊMŠJŽn
	gyro.WriteSingle(reg);
	gyro.WriteSingle(dh);
	gyro.WriteSingle(dl);
	gyro.Stop(); //’ÊMI—¹
}

float gyro_angle(void){
	int16_t d[2];
	jy_recv_wd(0x3f,d);
	return ((float)((d[1] << 8) | d[0])  / 32768 * 180) + 180;
}
float gyro_angle_x(void){
	int16_t d[2];
	jy_recv_wd(0x3d,d);
	return ((float)((d[1] << 8) | d[0])  / 32768 * 180) + 180;
}
float gyro_angle_y(void){
	int16_t d[2];
	jy_recv_wd(0x3e,d);
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
	serial.string("”N ");
	serial.putint(data[1]);
	serial.string("ŒŽ ");
	jy_recv_wd(0x31,data);
	serial.putint(data[0]);
	serial.string("“ú ");
	serial.putint(data[1]);
	serial.string("Žž ");
	jy_recv_wd(0x32,data);
	serial.putint(data[0]);
	serial.string("•ª ");
	serial.putint(data[1]);
	serial.string("•b ");
	jy_recv_wd(0x33,data);
	serial.putint((data[1]<<8)|data[0]);
	serial.string("m•b");
	//serial.string("\n\r");
}

void gyro_mes(void){
	int16_t data[2];
	serial.string("x:");
	jy_recv_wd(0x3d,data);
	serial.putfloat(((float)((data[1] << 8) | data[0])  / 32768 * 180) + 180);
	serial.string(",y:");
	jy_recv_wd(0x3e,data);
	serial.putfloat(((float)((data[1] << 8) | data[0])  / 32768 * 180) + 180);
	serial.string(",z:");
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
	//ZŽ²’²®ƒ‚[ƒh
	buzzer(600);
	gyro_send(0x01,0x01,0);//ƒLƒƒƒŠƒuƒŒ[ƒVƒ‡ƒ“ƒ‚[ƒh‘I‘ð
	_delay_ms(1000); //‘Ò‹@ŽžŠÔji”CˆÓj
	//Ž¥‹CƒZƒ“ƒTƒLƒƒƒŠƒuƒŒ[ƒVƒ‡ƒ“ƒ‚[ƒh
	gyro_send(0x01,0x02,0);//ƒLƒƒƒŠƒuƒŒ[ƒVƒ‡ƒ“ƒ‚[ƒh‘I‘ð
	buzzer(600);
	_delay_ms(5000); //ƒLƒƒƒŠƒuƒŒ[ƒVƒ‡ƒ“ŽžŠÔi”CˆÓj
	gyro_send(0x01,0,0);//ƒLƒƒƒŠƒuƒŒ[ƒVƒ‡ƒ“ƒ‚[ƒhI—¹
	_delay_ms(1000); //‘Ò‹@ŽžŠÔji”CˆÓj
	gyro_send(0,0,0);//ƒLƒƒƒŠƒuƒŒ[ƒVƒ‡ƒ“ƒ‚[ƒhI—¹
	_delay_ms(1000); //‘Ò‹@ŽžŠÔji”CˆÓj
	buzzer(400);
}
void jy_recv_wd(uint8_t sd,int16_t *d){
	gyro.Address(0x50<<1,0); //J901‚Æ‚Ì’ÊMŠJŽn
	gyro.WriteSingle(sd); //zŽ²‚ÌŠp“x‚ð“Ç‚ÝŽæ‚éƒAƒhƒŒƒX‚ð‘—M
	gyro.Address(0x50<<1,1); //J901‚Æ‚Ì’ÊMŠJŽn
	*d = gyro.ReadSingle(1); //1byte–ÚŽóM
	d++;
	*d = gyro.ReadSingle(0); //2byte–ÚŽóM
	gyro.Stop();
	return;
}