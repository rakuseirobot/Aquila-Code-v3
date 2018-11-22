/*
 * mpu9250.h
 *
 * Created: 2018/04/21 14:40:12
 *  Author: Shun
 */ 


#ifndef MPU9250_H_
#define MPU9250_H_

#include "xmega_usart.hpp"
#include "xmega_twi.hpp"
#include <util/delay.h>


twi mpu(&TWIC,100000);
/////////////////////////MPU9250////////////////////////////

#define MPU9250_ADDRESS 0x68<<1//I2CでのMPU9250のスレーブアドレス
#define PWR_MGMT_1 0x6b//電源管理のアドレス，スリープモード解除用
#define INT_PIN_CFG 0x37//磁気センサのバイパスモード設定用のアドレス
#define ACCEL_CONFIG 0x1c//加速度センサ設定用のアドレス
#define ACCEL_FS_SEL_2G 0x00//加速度センサのレンジ(2G)
#define ACCEL_FS_SEL_4G 0x08//加速度センサのレンジ(4G)
#define ACCEL_FS_SEL_8G 0x10//加速度センサのレンジ(8G)
#define ACCEL_FS_SEL_16G 0x18//加速度センサのレンジ(16G)
#define GYRO_CONFIG 0x1b//ジャイロセンサ設定用のアドレス
#define GYRO_FS_SEL_250DPS 0x00//ジャイロセンサのレンジ(250DPS)
#define GYRO_FS_SEL_500DPS 0x08//ジャイロセンサのレンジ(500DPS)
#define GYRO_FS_SEL_1000DPS 0x10//ジャイロセンサのレンジ(1000DPS)
#define GYRO_FS_SEL_2000DPS 0x18//ジャイロセンサのレンジ(2000DPS)
#define AK8963_ADDRESS 0x0c//磁気センサのスレーブアドレス
#define CNTL1 0x0a//磁気センサ設定用のアドレス
#define CNTL1_MODE_SEL_8HZ 0x12//磁気センサの出力周期(8Hz)
#define CNTL1_MODE_SEL_100HZ 0x16//磁気センサの出力周期(100Hz)
#define ST1 0x02//データ読み込み用フラッグのアドレス
///////////////////////////////////////////////////////////

volatile float accRange;//計算で使用するので，選択したレンジを入力する定数
volatile float gyroRange;//計算で使用するので，選択したレンジを入力する定数

uint8_t accGyroTempData[14];//センサからのデータ格納用配列
volatile uint8_t magneticData[7];//センサからのデータ格納用配列
volatile uint8_t ST1Bit;//磁気センサのフラッグ

volatile int16_t ax = 0 ,ay = 0 ,az = 0 ,gx = 0 ,gy = 0 ,gz = 0 ,tempMPU9250Raw = 0 ,mx = 0 ,my = 0 ,mz = 0;//16bitの出力データ

volatile int16_t driftX = 0 ,driftY = 0 ,driftZ = 0;

volatile float accX = 0 ,accY = 0 ,accZ = 0;//加速度センサから求めた重力加速度
volatile float gyroX = 0;//ジャイロセンサから求めた角速度
volatile float gyroY = 0;//ジャイロセンサから求めた角速度
volatile float gyroZ = 0;//ジャイロセンサから求めた角速度

volatile float tempMPU9250 = 0;//MPU9250の温度

volatile float magX = 0 ,magY = 0 ,magZ = 0;//磁気センサから求めたuT


void init_mpu(void);
void mpu_debug(void);
void mpu_read(void);
void mpuWriteByte(uint8_t adr, uint8_t data1, uint8_t data2);
void mpu_i2c_Read(uint8_t Address, uint8_t Register, uint8_t NBytes, uint8_t *Data);

void init_mpu(void){
	mpuWriteByte(MPU9250_ADDRESS, PWR_MGMT_1, 0x00);//スリープモードを解除

	mpuWriteByte(MPU9250_ADDRESS, ACCEL_CONFIG, ACCEL_FS_SEL_16G);//加速度センサの測定レンジの設定

	accRange = 16.0;//計算で使用するので，選択したレンジを入力する

	mpuWriteByte(MPU9250_ADDRESS, GYRO_CONFIG, GYRO_FS_SEL_2000DPS);//ジャイロセンサの測定レンジの設定

	gyroRange = 2000;//計算で使用するので，選択したレンジを入力する
}

void mpu_debug(void){

usart serial(&USARTC0,&PORTC);
	serial.string("ax: ");

	serial.puthex2(accX*1000000);

	serial.string(",");

	serial.string("ay: ");

	serial.puthex2(accY*1000000);

	serial.string(",");

	serial.string("az: ");

	serial.puthex2(accZ*1000000);

	serial.string(",");

	serial.string("gx: ");

	serial.puthex2(gyroX*1000000);

	serial.string(",");

	serial.string("gy: ");

	serial.puthex2(gyroY*1000000);

	serial.string(",");

	serial.string("gz: ");

	serial.puthex2(gyroZ*1000000);

	serial.string("\n\r");
	_delay_ms(10);
}
void mpu_read(void){
	//Acc

	mpu_i2c_Read(MPU9250_ADDRESS, 0x3b, 14, accGyroTempData); //0x3bから，14バイト分をaccGyroDataにいれる	

	ax = (accGyroTempData[0] << 8) | accGyroTempData[1];//accGyroTempData[0]を左に8シフトし(<<)，accGyroTempData[1]を足し合わせる(|)

	ay = (accGyroTempData[2] << 8) | accGyroTempData[3];//accGyroTempData[2]を左に8シフトし(<<)，accGyroTempData[3]を足し合わせる(|)

	az = (accGyroTempData[4] << 8) | accGyroTempData[5];//accGyroTempData[4]を左に8シフトし(<<)，accGyroTempData[5]を足し合わせる(|)

  //Gyro

	gx = (accGyroTempData[8] << 8) | accGyroTempData[9];//accGyroTempData[8]を左に8シフトし(<<)，accGyroTempData[9]を足し合わせる(|)

	gy = (accGyroTempData[10] << 8) | accGyroTempData[11];//accGyroTempData[10]を左に8シフトし(<<)，accGyroTempData[11]を足し合わせる(|)

	gz = (accGyroTempData[12] << 8) | accGyroTempData[13];//accGyroTempData[12]を左に8シフトし(<<)，accGyroTempData[13]を足し合わせる(|)

	accX = ax * accRange / 32768.0;//[G]に変換

 	accY = ay * accRange / 32768.0;//[G]に変換

	accZ = az * accRange / 32768.0;//[G]に変換

	gyroX = (gx - driftX) * gyroRange / 32768.0;//[deg/s]に変換

	gyroY = (gy - driftY) * gyroRange / 32768.0;//[deg/s]に変換

	gyroZ = (gz - driftZ) * gyroRange / 32768.0;//[deg/s]に変換

	return;
}
void mpuWriteByte(uint8_t adr, uint8_t data1, uint8_t data2){
	mpu.Address(adr,0);
	mpu.WriteSingle(data1);
	mpu.WriteSingle(data2);
	mpu.Stop();
	return;
}



void mpu_i2c_Read(uint8_t Address, uint8_t Register, uint8_t NBytes, uint8_t *Data) {//指定したアドレスのデータを読む関数

	mpu.Address(Address,0);//指定したアドレスと通信を始める

	mpu.WriteSingle(Register);//レジスタを書き込む

	mpu.Stop();//通信を終了する

	mpu.Address(Address,1);//指定したアドレスと通信を始める

	mpu.ReadMultiple(Data,NBytes);

	mpu.Stop();
	
	return;

}
#endif /* MPU9250_H_ */