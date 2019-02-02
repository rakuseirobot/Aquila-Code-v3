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
#define EEPROM_ADDR 0x50

void erom::WriteSingle(uint16_t eeaddress,uint8_t data ) 
{
    eeprom.Address(EEPROM_ADDR<<1,0);
    eeprom.WriteSingle(eeaddress >> 8);   // MSB
    eeprom.WriteSingle(eeaddress & 0xFF); // LSB
    eeprom.WriteSingle(data);
    eeprom.Stop();
    _delay_ms(5);
}
void erom::WritePage(uint16_t eeaddress,uint8_t *data ) 
{
    eeprom.Address(EEPROM_ADDR<<1,0);
    eeprom.WriteSingle(eeaddress >> 8);   // MSB
    eeprom.WriteSingle(eeaddress & 0xFF); // LSB
    for (uint8_t i=0;i<64;i++){
        eeprom.WriteSingle(*data);
        data++;
    }
    eeprom.WriteSingle(*data);
    eeprom.Stop();
    _delay_ms(5);
}

uint8_t erom::ReadSingle(uint16_t eeaddress) 
{
    uint8_t rdata = 0xFF;
    eeprom.Address(EEPROM_ADDR<<1,0);
    eeprom.WriteSingle(eeaddress >> 8);   // MSB
    eeprom.WriteSingle(eeaddress & 0xFF); // LSB
    eeprom.Address(0x50<<1,1); 
	rdata = eeprom.ReadSingle(0); //1byte目受信
	eeprom.Stop();
    return rdata;
}

void erom::ReadMulti(uint16_t eeaddress,uint8_t *d,uint16_t val)
{
	eeprom.Address(EEPROM_ADDR<<1,0);
	eeprom.WriteSingle(eeaddress >> 8);   // MSB
	eeprom.WriteSingle(eeaddress & 0xFF); // LSB
	eeprom.Address(0x50<<1,1);
    for (uint16_t i=0;i<val;i++){       
        *d = gyro.ReadSingle(1); //1byte目受信
        d++;
    }
	rdata = eeprom.ReadSingle(0); //最終byte目受信
	eeprom.Stop();
}
