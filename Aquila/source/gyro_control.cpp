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
	gyro.Address(0x50<<1,0); //J901�Ƃ̒ʐM�J�n
	gyro.WriteSingle(0x3f); //z���̊p�x��ǂݎ��A�h���X�𑗐M
	gyro.Address(0x50<<1,1); //J901�Ƃ̒ʐM�J�n
	yawl = gyro.ReadSingle(1); //1byte�ڎ�M
	yawh = gyro.ReadSingle(0); //2byte�ڎ�M
	gyro.Stop();
	angle = (float)((yawh << 8) | yawl)  / 32768 * 180;
	serial.puthex2(angle*100);
	serial.string("\n\r");
}
void gyro_set200hz(void){
	gyro.Address(0x50<<1,0); //J901�Ƃ̒ʐM�J�n
	gyro.WriteSingle(0x03); //RATE
	gyro.WriteSingle(0x0a); //Set 200Hz
	gyro.WriteSingle(0); //��ʃr�b�g
	gyro.Stop(); //�ʐM�I��	
	_delay_ms(500);
}
void gyro_Save(void){
	gyro.Address(0x50<<1,0); //J901�Ƃ̒ʐM�J�n
	gyro.WriteSingle(0); //RATE
	gyro.WriteSingle(0); //Set 200Hz
	gyro.WriteSingle(0); //��ʃr�b�g
	gyro.Stop(); //�ʐM�I��
}


void gyro_cali(void){
	//Z���������[�h
	gyro.Address(0x50<<1,0); //J901�Ƃ̒ʐM�J�n
	gyro.WriteSingle(0x01); //�L�����u���[�V�������[�h�I��
	gyro.WriteSingle(0x03); //���ʃr�b�g
	gyro.WriteSingle(0); //��ʃr�b�g
	gyro.Stop(); //�ʐM�I��
	_delay_ms(1000); //�ҋ@���ԁj�i�C�Ӂj
	//���C�Z���T�L�����u���[�V�������[�h
	gyro.Address(0x50<<1,0); //J901�Ƃ̒ʐM�J�n
	gyro.WriteSingle(0x01); //�L�����u���[�V�������[�h�I��
	gyro.WriteSingle(0x02); //���ʃr�b�g
	gyro.WriteSingle(0); //��ʃr�b�g
	gyro.Stop(); //�ʐM�I��
	_delay_ms(5000); //�L�����u���[�V�������ԁi�C�Ӂj
	//�L�����u���[�V�������[�h�I��
	gyro.Address(0x50<<1,0); //J901�Ƃ̒ʐM�J�n
	gyro.WriteSingle(0x01); //�L�����u���[�V����
	gyro.WriteSingle(0); //���ʃr�b�g
	gyro.WriteSingle(0); //��ʃr�b�g
	gyro.Stop(); //�ʐM�I��
}