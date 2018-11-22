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

#define MPU9250_ADDRESS 0x68<<1//I2C�ł�MPU9250�̃X���[�u�A�h���X
#define PWR_MGMT_1 0x6b//�d���Ǘ��̃A�h���X�C�X���[�v���[�h�����p
#define INT_PIN_CFG 0x37//���C�Z���T�̃o�C�p�X���[�h�ݒ�p�̃A�h���X
#define ACCEL_CONFIG 0x1c//�����x�Z���T�ݒ�p�̃A�h���X
#define ACCEL_FS_SEL_2G 0x00//�����x�Z���T�̃����W(2G)
#define ACCEL_FS_SEL_4G 0x08//�����x�Z���T�̃����W(4G)
#define ACCEL_FS_SEL_8G 0x10//�����x�Z���T�̃����W(8G)
#define ACCEL_FS_SEL_16G 0x18//�����x�Z���T�̃����W(16G)
#define GYRO_CONFIG 0x1b//�W���C���Z���T�ݒ�p�̃A�h���X
#define GYRO_FS_SEL_250DPS 0x00//�W���C���Z���T�̃����W(250DPS)
#define GYRO_FS_SEL_500DPS 0x08//�W���C���Z���T�̃����W(500DPS)
#define GYRO_FS_SEL_1000DPS 0x10//�W���C���Z���T�̃����W(1000DPS)
#define GYRO_FS_SEL_2000DPS 0x18//�W���C���Z���T�̃����W(2000DPS)
#define AK8963_ADDRESS 0x0c//���C�Z���T�̃X���[�u�A�h���X
#define CNTL1 0x0a//���C�Z���T�ݒ�p�̃A�h���X
#define CNTL1_MODE_SEL_8HZ 0x12//���C�Z���T�̏o�͎���(8Hz)
#define CNTL1_MODE_SEL_100HZ 0x16//���C�Z���T�̏o�͎���(100Hz)
#define ST1 0x02//�f�[�^�ǂݍ��ݗp�t���b�O�̃A�h���X
///////////////////////////////////////////////////////////

volatile float accRange;//�v�Z�Ŏg�p����̂ŁC�I�����������W����͂���萔
volatile float gyroRange;//�v�Z�Ŏg�p����̂ŁC�I�����������W����͂���萔

uint8_t accGyroTempData[14];//�Z���T����̃f�[�^�i�[�p�z��
volatile uint8_t magneticData[7];//�Z���T����̃f�[�^�i�[�p�z��
volatile uint8_t ST1Bit;//���C�Z���T�̃t���b�O

volatile int16_t ax = 0 ,ay = 0 ,az = 0 ,gx = 0 ,gy = 0 ,gz = 0 ,tempMPU9250Raw = 0 ,mx = 0 ,my = 0 ,mz = 0;//16bit�̏o�̓f�[�^

volatile int16_t driftX = 0 ,driftY = 0 ,driftZ = 0;

volatile float accX = 0 ,accY = 0 ,accZ = 0;//�����x�Z���T���狁�߂��d�͉����x
volatile float gyroX = 0;//�W���C���Z���T���狁�߂��p���x
volatile float gyroY = 0;//�W���C���Z���T���狁�߂��p���x
volatile float gyroZ = 0;//�W���C���Z���T���狁�߂��p���x

volatile float tempMPU9250 = 0;//MPU9250�̉��x

volatile float magX = 0 ,magY = 0 ,magZ = 0;//���C�Z���T���狁�߂�uT


void init_mpu(void);
void mpu_debug(void);
void mpu_read(void);
void mpuWriteByte(uint8_t adr, uint8_t data1, uint8_t data2);
void mpu_i2c_Read(uint8_t Address, uint8_t Register, uint8_t NBytes, uint8_t *Data);

void init_mpu(void){
	mpuWriteByte(MPU9250_ADDRESS, PWR_MGMT_1, 0x00);//�X���[�v���[�h������

	mpuWriteByte(MPU9250_ADDRESS, ACCEL_CONFIG, ACCEL_FS_SEL_16G);//�����x�Z���T�̑��背���W�̐ݒ�

	accRange = 16.0;//�v�Z�Ŏg�p����̂ŁC�I�����������W����͂���

	mpuWriteByte(MPU9250_ADDRESS, GYRO_CONFIG, GYRO_FS_SEL_2000DPS);//�W���C���Z���T�̑��背���W�̐ݒ�

	gyroRange = 2000;//�v�Z�Ŏg�p����̂ŁC�I�����������W����͂���
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

	mpu_i2c_Read(MPU9250_ADDRESS, 0x3b, 14, accGyroTempData); //0x3b����C14�o�C�g����accGyroData�ɂ����	

	ax = (accGyroTempData[0] << 8) | accGyroTempData[1];//accGyroTempData[0]������8�V�t�g��(<<)�CaccGyroTempData[1]�𑫂����킹��(|)

	ay = (accGyroTempData[2] << 8) | accGyroTempData[3];//accGyroTempData[2]������8�V�t�g��(<<)�CaccGyroTempData[3]�𑫂����킹��(|)

	az = (accGyroTempData[4] << 8) | accGyroTempData[5];//accGyroTempData[4]������8�V�t�g��(<<)�CaccGyroTempData[5]�𑫂����킹��(|)

  //Gyro

	gx = (accGyroTempData[8] << 8) | accGyroTempData[9];//accGyroTempData[8]������8�V�t�g��(<<)�CaccGyroTempData[9]�𑫂����킹��(|)

	gy = (accGyroTempData[10] << 8) | accGyroTempData[11];//accGyroTempData[10]������8�V�t�g��(<<)�CaccGyroTempData[11]�𑫂����킹��(|)

	gz = (accGyroTempData[12] << 8) | accGyroTempData[13];//accGyroTempData[12]������8�V�t�g��(<<)�CaccGyroTempData[13]�𑫂����킹��(|)

	accX = ax * accRange / 32768.0;//[G]�ɕϊ�

 	accY = ay * accRange / 32768.0;//[G]�ɕϊ�

	accZ = az * accRange / 32768.0;//[G]�ɕϊ�

	gyroX = (gx - driftX) * gyroRange / 32768.0;//[deg/s]�ɕϊ�

	gyroY = (gy - driftY) * gyroRange / 32768.0;//[deg/s]�ɕϊ�

	gyroZ = (gz - driftZ) * gyroRange / 32768.0;//[deg/s]�ɕϊ�

	return;
}
void mpuWriteByte(uint8_t adr, uint8_t data1, uint8_t data2){
	mpu.Address(adr,0);
	mpu.WriteSingle(data1);
	mpu.WriteSingle(data2);
	mpu.Stop();
	return;
}



void mpu_i2c_Read(uint8_t Address, uint8_t Register, uint8_t NBytes, uint8_t *Data) {//�w�肵���A�h���X�̃f�[�^��ǂފ֐�

	mpu.Address(Address,0);//�w�肵���A�h���X�ƒʐM���n�߂�

	mpu.WriteSingle(Register);//���W�X�^����������

	mpu.Stop();//�ʐM���I������

	mpu.Address(Address,1);//�w�肵���A�h���X�ƒʐM���n�߂�

	mpu.ReadMultiple(Data,NBytes);

	mpu.Stop();
	
	return;

}
#endif /* MPU9250_H_ */