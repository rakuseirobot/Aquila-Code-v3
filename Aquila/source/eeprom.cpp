/*
 * eeprom.cpp
 *
 * Created: 2019/02/02 18:17:20
 *  Author: shun2
 */ 

#include "eeprom.hpp"
#include "xmega_twi.hpp"
#include "gyro_control.hpp"
#include <util/delay.h>
extern twi gyro;
twi eeprom = gyro;
#define EEPROM_ADDR 0xAE

void erom::WriteSingle(uint16_t eeaddress,uint8_t data ) //�O~31999
{
    eeprom.Address(EEPROM_ADDR,0);
    eeprom.WriteSingle(eeaddress >> 8);   // MSB
    eeprom.WriteSingle(eeaddress & 0xFF); // LSB
    eeprom.WriteSingle(data);
    eeprom.Stop();
    _delay_ms(20);
}
void erom::WritePage(uint16_t pages,uint8_t *data )  //�y�[�W�P�ʂł̑��!�A�h���X����Ȃ���!  0-500!!
{   
    uint16_t eeaddress=pages*64;
    eeprom.Address(EEPROM_ADDR,0);
    eeprom.WriteSingle(eeaddress >> 8);   // MSB
    eeprom.WriteSingle(eeaddress & 0xFF); // LSB
    for (uint8_t i=0;i<63;i++){
        eeprom.WriteSingle(*data);
        data++;
    }
    eeprom.WriteSingle(*data);
    eeprom.Stop();
    _delay_ms(20);
}

uint8_t erom::ReadSingle(uint16_t eeaddress) 
{
    uint8_t rdata = 0xFF;
    eeprom.Address(EEPROM_ADDR,0);
    eeprom.WriteSingle(eeaddress >> 8);   // MSB
    eeprom.WriteSingle(eeaddress & 0xFF); // LSB
    eeprom.Address(EEPROM_ADDR,1); 
	rdata = eeprom.ReadSingle(0); //1byte�ڎ�M
	eeprom.Stop();
    return rdata;
}

void erom::ReadMulti(uint16_t eeaddress,uint8_t *d,uint16_t val) //�ǂ��w�肵�Ă�����!
{
	eeprom.Address(EEPROM_ADDR,0);
	eeprom.WriteSingle(eeaddress >> 8);   // MSB
	eeprom.WriteSingle(eeaddress & 0xFF); // LSB
	eeprom.Address(EEPROM_ADDR,1);
    for (uint16_t i=0;i<val-1;i++){       
        *d = eeprom.ReadSingle(1); //1byte�ڎ�M
        d++;
    }
	*d = eeprom.ReadSingle(0); //�ŏIbyte�ڎ�M
	eeprom.Stop();
}
