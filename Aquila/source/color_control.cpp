/*
 * color_control.cpp
 *
 * Created: 2018/08/23 18:28:32
 *  Author: shun2
 */ 
/*
PF0:RANGE
PF1:CK
PF2:GATE
PF3:DOUT
*/
#include "color_control.hpp"

uint16_t red=0, green=0, blue=0;	//�l
uint16_t high=800, low=500;	//�������l ��>800  700<��<=800

void init_color(void){	//initialize
	PORTF.DIRSET = PIN0_bm|PIN1_bm|PIN2_bm;
}
void color_read(void){	//�Z���T�[�̒l��gree,red,blue�ɑ�����Ă����
	red=0, green=0, blue=0;
	PORTF.OUTCLR = PIN0_bm|PIN1_bm|PIN2_bm|PIN3_bm;
	_delay_us(2000);
	PORTF.OUTSET = RANGE;
	PORTF.OUTSET = GATE;
	_delay_ms(20);	//�����odelay
	PORTF.OUTCLR = GATE;
	_delay_us(4);
	red=color_shift();
	green=color_shift();
	blue=color_shift();
	PORTF.OUTSET = GATE;
}

uint16_t color_shift(void){//12�r�b�g���̃p���X���M�Ɠǂݍ��ݏ���
	uint16_t result=0;//���o���ʗp�̕ϐ���p�Ӂi0�F�������j
	for(uint8_t i=0;i<12;i++){//12�r�b�g���̌J��Ԃ�����
		PORTF.OUTSET = CK;//1�r�b�g���̃N���b�N�p���X�o�́iHIGH�j
		_delay_us(1);
		if((PORTF.IN & DOUT)!=0){//Dout�[�q����̏o�͂�High�̏ꍇ
			result+=(1<<i);//12�r�b�g��i���ڂ�1�����ii�����������ɃV�t�g�j
		}
		PORTF.OUTCLR = CK;//1�r�b�g���̃N���b�N�p���X�o�́iLOW�j
		_delay_us(1);
	}
	_delay_us(3);//3�}�C�N���b�ҋ@
	return result;//���ʂ��o��
}

uint8_t color_check(void){	//�J���[�`�F�b�N 0:�� 1:�N�� 2:��i�`�F�b�N�|�C���g�j�@�@return�ŋA���Ă��܂��B  //�܂��l��܂�Ȃ��I
	color_read();
	if(blue+green+red>700){
		return 2;
	}
	//else if(blue+green+red>=100){
		//return 0;
	//}
	else{
		return 1;
	}
}