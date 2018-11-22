/*
 * gyro_control.cpp
 *
 * Created: 2018/08/28 11:18:39
 *  Author: shun2
 */ 

#include "gyro_control.hpp"
twi gyro(&TWIC,2000000);

void gyro_send(uint8_t reg,uint8_t dh,uint8_t dl){
	gyro.Address(0x50<<1,0); //J901との通信開始
	gyro.WriteSingle(reg);
	gyro.WriteSingle(dh);
	gyro.WriteSingle(dl);
	gyro.Stop(); //通信終了
}


void gyro_mes(void){
	int yawl =0, yawh=0;
	uint16_t angle=0.0;
	gyro.Address(0x50<<1,0); //J901との通信開始
	gyro.WriteSingle(0x3f); //z軸の角度を読み取るアドレスを送信
	gyro.Address(0x50<<1,1); //J901との通信開始
	yawl = gyro.ReadSingle(1); //1byte目受信
	yawh = gyro.ReadSingle(0); //2byte目受信
	gyro.Stop();
	angle = (float)((yawh << 8) | yawl)  / 32768 * 180;
	angle=(angle-359)*(-1);
	if(angle<0){
		angle=angle+360;
	}
	serial.putdec(angle);
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
	gyro_send(0x01,0x01,0);//キャリブレーションモード選択
	_delay_ms(1000); //待機時間）（任意）
	//磁気センサキャリブレーションモード
	gyro_send(0x01,0x02,0);//キャリブレーションモード選択
	_delay_ms(5000); //キャリブレーション時間（任意）
	gyro_send(0x01,0,0);//キャリブレーションモード終了
	_delay_ms(1000); //待機時間）（任意）
	gyro_send(0,0,0);//キャリブレーションモード終了
	_delay_ms(1000); //待機時間）（任意）
}