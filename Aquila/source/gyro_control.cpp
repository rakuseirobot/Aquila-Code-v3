/*
 * gyro_control.cpp
 *
 * Created: 2018/08/28 11:18:39
 *  Author: shun2
 */ 

#include "gyro_control.hpp"
twi gyro(&TWIC,2000000);

void gyro_send(uint8_t reg,uint8_t dh,uint8_t dl){
	gyro.Address(0x50<<1,0); //J901�Ƃ̒ʐM�J�n
	gyro.WriteSingle(reg);
	gyro.WriteSingle(dh);
	gyro.WriteSingle(dl);
	gyro.Stop(); //�ʐM�I��
}


void gyro_mes(void){
	int yawl =0, yawh=0;
	uint16_t angle=0.0;
	gyro.Address(0x50<<1,0); //J901�Ƃ̒ʐM�J�n
	gyro.WriteSingle(0x3f); //z���̊p�x��ǂݎ��A�h���X�𑗐M
	gyro.Address(0x50<<1,1); //J901�Ƃ̒ʐM�J�n
	yawl = gyro.ReadSingle(1); //1byte�ڎ�M
	yawh = gyro.ReadSingle(0); //2byte�ڎ�M
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
	//Z���������[�h
	gyro_send(0x01,0x01,0);//�L�����u���[�V�������[�h�I��
	_delay_ms(1000); //�ҋ@���ԁj�i�C�Ӂj
	//���C�Z���T�L�����u���[�V�������[�h
	gyro_send(0x01,0x02,0);//�L�����u���[�V�������[�h�I��
	_delay_ms(5000); //�L�����u���[�V�������ԁi�C�Ӂj
	gyro_send(0x01,0,0);//�L�����u���[�V�������[�h�I��
	_delay_ms(1000); //�ҋ@���ԁj�i�C�Ӂj
	gyro_send(0,0,0);//�L�����u���[�V�������[�h�I��
	_delay_ms(1000); //�ҋ@���ԁj�i�C�Ӂj
}