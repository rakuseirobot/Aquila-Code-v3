/*
 * eeprom.cpp
 *
 * Created: 2019/02/02 18:17:20
 *  Author: shun2
 */ 

#include "eeprom.hpp"
#include "xmega_twi.hpp"
#include "gyro_control.hpp"
extern twi gyro;
twi eeprom = gyro;
#define EEPROM_ADDR 0x50

void erom::WriteSingle(unsigned int eeaddress,uint8_t data ) 
{
    eeprom.Address(EEPROM_ADDR<<1,0);
    eeprom.WriteSingle((int)(eeaddress >> 8));   // MSB
    eeprom.WriteSingle((int)(eeaddress & 0xFF)); // LSB
    eeprom.WriteSingle(data);
    eeprom.Stop();
    _delay_ms(5);
}

uint8_t wrom::ReadSingle(uint16_t eeaddress) 
{
    uint8_t rdata = 0xFF;
    eeprom.Address(EEPROM_ADDR<<1,0);
    eeprom.WriteSingle((int)(eeaddress >> 8));   // MSB
    eeprom.WriteSingle((int)(eeaddress & 0xFF)); // LSB
    eeprom.Stop();
    eeprom.Address(0x50<<1,1); //J901との通信開始
	rdata = eeprom.ReadSingle(0); //1byte目受信
	eeprom.Stop();
    return rdata;
}
