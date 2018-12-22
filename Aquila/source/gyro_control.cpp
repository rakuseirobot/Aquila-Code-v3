/*
 * gyro_control.cpp
 *
 * Created: 2018/08/28 11:18:39
 *  Author: shun2
 */ 

#include "gyro_control.hpp"
twi gyro(&TWIC,250000);

void gyro_send(uint8_t reg,uint8_t dh,uint8_t dl){
	gyro.Address(0x50<<1,0); //J901との通信開始
	gyro.WriteSingle(reg);
	gyro.WriteSingle(dh);
	gyro.WriteSingle(dl);
	gyro.Stop(); //通信終了
}

float gyro_angle(void){
	int yawl =0, yawh=0;
	float angle=0.0;
	gyro.Address(0x50<<1,0); //J901との通信開始
	gyro.WriteSingle(0x3f); //z軸の角度を読み取るアドレスを送信
	gyro.Address(0x50<<1,1); //J901との通信開始
	yawl = gyro.ReadSingle(1); //1byte目受信
	yawh = gyro.ReadSingle(0); //2byte目受信
	gyro.Stop();
	angle = ((float)((yawh << 8) | yawl)  / 32768 * 180) + 180;
	return angle;
}

float acc_x_mes(void){
	uint8_t yawl=0, yawh=0;
	gyro.Address(0x50<<1,0); //J901との通信開始
	gyro.WriteSingle(0x34); //X軸の加速度を読み取るアドレスを送信
	gyro.Address(0x50<<1,1); //J901との通信開始
	yawl = gyro.ReadSingle(1); //1byte目受信
	yawh = gyro.ReadSingle(0); //2byte目受信
	gyro.Stop();
	return ((float)((yawh << 8) | yawl)  / 32768 * 16 * 9.8);
}

float acc_y_mes(void){
	uint8_t yawl=0, yawh=0;
	gyro.Address(0x50<<1,0); //J901との通信開始
	gyro.WriteSingle(0x35); //Y軸の加速度を読み取るアドレスを送信
	gyro.Address(0x50<<1,1); //J901との通信開始
	yawl = gyro.ReadSingle(1); //1byte目受信
	yawh = gyro.ReadSingle(0); //2byte目受信
	gyro.Stop();
	return ((float)((yawh << 8) | yawl)  / 32768 * 16 * 9.8);
}

void acc_mes(void){
	float data[3]={0,0,0};
	uint8_t yawl=0, yawh=0;
	gyro.Address(0x50<<1,0); //J901との通信開始
	gyro.WriteSingle(0x34); //X軸の加速度を読み取るアドレスを送信
	gyro.Address(0x50<<1,1); //J901との通信開始
	yawl = gyro.ReadSingle(1); //1byte目受信
	yawh = gyro.ReadSingle(0); //2byte目受信
	gyro.Stop();
	data[0] = ((float)((yawh << 8) | yawl)  / 32768 * 16 * 9.8);
	gyro.Address(0x50<<1,0); //J901との通信開始
	gyro.WriteSingle(0x35); //Y軸の加速度を読み取るアドレスを送信
	gyro.Address(0x50<<1,1); //J901との通信開始
	yawl = gyro.ReadSingle(1); //1byte目受信
	yawh = gyro.ReadSingle(0); //2byte目受信
	gyro.Stop();
	data[1] = ((float)((yawh << 8) | yawl)  / 32768 * 16 * 9.8);
	gyro.Address(0x50<<1,0); //J901との通信開始
	gyro.WriteSingle(0x36); //Z軸の加速度を読み取るアドレスを送信
	gyro.Address(0x50<<1,1); //J901との通信開始
	yawl = gyro.ReadSingle(1); //1byte目受信
	yawh = gyro.ReadSingle(0); //2byte目受信
	gyro.Stop();
	data[2] = ((float)((yawh << 8) | yawl)  / 32768 * 16 * 9.8);
	//angle=(angle-359)*(-1);
	serial.string("x:");
	serial.putfloat(data[0]);
	serial.string("y:");
	serial.putfloat(data[1]);
	serial.string("z:");
	serial.putfloat(data[2]);
	serial.string("\n\r");
}
void gyro_time_mes(void){
	int yawl=0, yawh=0;
	gyro.Address(0x50<<1,0); //J901との通信開始
	gyro.WriteSingle(0x30); //z軸の角度を読み取るアドレスを送信
	gyro.Address(0x50<<1,1); //J901との通信開始
	yawl = gyro.ReadSingle(1); //1byte目受信
	yawh = gyro.ReadSingle(0); //2byte目受信
	gyro.Stop();
	serial.putint(yawl);
	serial.string("年");
	serial.putint(yawh);
	serial.string("月");
	gyro.Address(0x50<<1,0); //J901との通信開始
	gyro.WriteSingle(0x31); //z軸の角度を読み取るアドレスを送信
	gyro.Address(0x50<<1,1); //J901との通信開始
	yawl = gyro.ReadSingle(1); //1byte目受信
	yawh = gyro.ReadSingle(0); //2byte目受信
	gyro.Stop();
	serial.putint(yawl);
	serial.string("日");
	serial.putint(yawh);
	serial.string("時");
	gyro.Address(0x50<<1,0); //J901との通信開始
	gyro.WriteSingle(0x32); //z軸の角度を読み取るアドレスを送信
	gyro.Address(0x50<<1,1); //J901との通信開始
	yawl = gyro.ReadSingle(1); //1byte目受信
	yawh = gyro.ReadSingle(0); //2byte目受信
	gyro.Stop();
	serial.putint(yawl);
	serial.string("分");
	serial.putint(yawh);
	serial.string("秒");
	gyro.Address(0x50<<1,0); //J901との通信開始
	gyro.WriteSingle(0x33); //z軸の角度を読み取るアドレスを送信
	gyro.Address(0x50<<1,1); //J901との通信開始
	yawl = gyro.ReadSingle(1); //1byte目受信
	yawh = gyro.ReadSingle(0); //2byte目受信
	gyro.Stop();
	serial.putint((yawh<<8)|yawl);
	serial.string("m秒");
	serial.string("\n\r");
}

void gyro_mes(void){
	int yawl=0, yawh=0;
	uint16_t angle=0.0;
	gyro.Address(0x50<<1,0); //J901との通信開始
	gyro.WriteSingle(0x3f); //z軸の角度を読み取るアドレスを送信
	gyro.Address(0x50<<1,1); //J901との通信開始
	yawl = gyro.ReadSingle(1); //1byte目受信
	yawh = gyro.ReadSingle(0); //2byte目受信
	gyro.Stop();
	angle = ((float)((yawh << 8) | yawl)  / 32768 * 180) + 180;
	//angle=(angle-359)*(-1);
	serial.putint(angle);
	serial.string("\n\r");
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