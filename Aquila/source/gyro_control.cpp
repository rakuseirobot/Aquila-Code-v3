/*
 * gyro_control.cpp
 *
 * Created: 2018/08/28 11:18:39
 *  Author: shun2
 */ 

#include "gyro_control.hpp"
void gyro_mes(void){
	float yawl =0.0, yawh=0.0;
	float angle=0.0;
	twi gyro(&TWIC,2000000);
	gyro.Address(0x50,0); //J901�Ƃ̒ʐM�J�n
	gyro.WriteSingle(0x3f); //z���̊p�x��ǂݎ��A�h���X�𑗐M
	gyro.Stop(false); //���M�I��
	gyro.Address(0x50,1); //J901�Ƃ̒ʐM�J�n
	//gyro.requestFrom(0x50, 2); //�f�[�^�v��
	/*while (Wire.available() < 2) //�f�[�^�ҋ@
	{
		Wire.beginTransmission(0x50); //J901�Ƃ̒ʐM�J�n
		gyro.WriteSingle(0x3f); //z���̊p�x��ǂݎ��A�h���X�𑗐M
		gyro.Address(0x50,1); //J901�Ƃ̒ʐM�J�n
		gyro.Stop(false); //���M�I��
		Wire.requestFrom(0x50, 2); //�f�[�^�v��
	}*/
	yawl = gyro.ReadSingle(1); //1byte�ڎ�M
	yawh = gyro.ReadSingle(0); //2byte�ڎ�M
	angle = (float)((yawh << 8) | yawl)  / 32768 * 180;
	serial.putdec(angle);
	_delay_ms(10);
}


void gyro_cali(void){
	twi gyro(&TWIC,2000000);
	//Z���������[�h
	gyro.Address(0x50,0); //J901�Ƃ̒ʐM�J�n
	gyro.WriteSingle(0x01); //�L�����u���[�V�������[�h�I��
	gyro.WriteSingle(0x03); //���ʃr�b�g
	gyro.WriteSingle(0); //��ʃr�b�g
	gyro.Stop(); //�ʐM�I��
	_delay_ms(1000); //�ҋ@���ԁj�i�C�Ӂj
	//���C�Z���T�L�����u���[�V�������[�h
	gyro.Address(0x50,0); //J901�Ƃ̒ʐM�J�n
	gyro.WriteSingle(0x01); //�L�����u���[�V�������[�h�I��
	gyro.WriteSingle(0x02); //���ʃr�b�g
	gyro.WriteSingle(0); //��ʃr�b�g
	gyro.Stop(); //�ʐM�I��
	_delay_ms(5000); //�L�����u���[�V�������ԁi�C�Ӂj
	//�L�����u���[�V�������[�h�I��
	gyro.Address(0x50,0); //J901�Ƃ̒ʐM�J�n
	gyro.WriteSingle(0x01); //�L�����u���[�V����
	gyro.WriteSingle(0); //���ʃr�b�g
	gyro.WriteSingle(0); //��ʃr�b�g
	gyro.Stop(); //�ʐM�I��
}