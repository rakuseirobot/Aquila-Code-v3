/*
 * gyro_control.cpp
 *
 * Created: 2018/08/28 11:18:39
 *  Author: shun2
 */ 

#include "gyro_control.hpp"
twi gyro(&TWIC,2000000);
void gyro_mes(void){
	int yawl =0, yawh=0;
	float angle=0.0;
	gyro.Address(0x50<<1,0); //J901との通信開始
	gyro.WriteSingle(0x3f); //z軸の角度を読み取るアドレスを送信
	gyro.Address(0x50<<1,1); //J901との通信開始
	yawl = gyro.ReadSingle(1); //1byte目受信
	yawh = gyro.ReadSingle(0); //2byte目受信
	gyro.Stop();
	angle = (float)((yawh << 8) | yawl)  / 32768 * 180;
	serial.puthex2(angle*100);
	serial.string("\n\r");
}
void gyro_set200hz(void){
	gyro.Address(0x50<<1,0); //J901との通信開始
	gyro.WriteSingle(0x03); //RATE
	gyro.WriteSingle(0x0a); //Set 200Hz
	gyro.WriteSingle(0); //上位ビット
	gyro.Stop(); //通信終了	
	_delay_ms(500);
}
void gyro_Save(void){
	gyro.Address(0x50<<1,0); //J901との通信開始
	gyro.WriteSingle(0); //RATE
	gyro.WriteSingle(0); //Set 200Hz
	gyro.WriteSingle(0); //上位ビット
	gyro.Stop(); //通信終了
}


void gyro_cali(void){
	//Z軸調整モード
	gyro.Address(0x50<<1,0); //J901との通信開始
	gyro.WriteSingle(0x01); //キャリブレーションモード選択
	gyro.WriteSingle(0x03); //下位ビット
	gyro.WriteSingle(0); //上位ビット
	gyro.Stop(); //通信終了
	_delay_ms(1000); //待機時間）（任意）
	//磁気センサキャリブレーションモード
	gyro.Address(0x50<<1,0); //J901との通信開始
	gyro.WriteSingle(0x01); //キャリブレーションモード選択
	gyro.WriteSingle(0x02); //下位ビット
	gyro.WriteSingle(0); //上位ビット
	gyro.Stop(); //通信終了
	_delay_ms(5000); //キャリブレーション時間（任意）
	//キャリブレーションモード終了
	gyro.Address(0x50<<1,0); //J901との通信開始
	gyro.WriteSingle(0x01); //キャリブレーション
	gyro.WriteSingle(0); //下位ビット
	gyro.WriteSingle(0); //上位ビット
	gyro.Stop(); //通信終了
}