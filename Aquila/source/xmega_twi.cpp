/*
 * xmega_twi.cpp
 *
 * Created: 2018/08/23 0:28:47
 *  Author: shun2
 */ 

#include "xmega_twi.hpp"

twi::twi(TWI_t *portt,unsigned long baud){
	port=portt;
	baudset=(F_CPU/(2*baud))-5;
	#if baudset>255
	#warning "the desired BAUDRATE is too low for F_CPU. The BAUD register will be set to its highest possible value"
	port->MASTER.BAUD=0xff;
	#else
	port->MASTER.BAUD=(uint8_t)baudset;
	#endif // BAUD
	//port->MASTER.BAUD=255;
	port->MASTER.CTRLB|=TWI_MASTER_TIMEOUT_200US_gc|0;  //TWI_MASTER_TIMEOUT_DISABLED_gc
	port->MASTER.CTRLA|=TWI_MASTER_ENABLE_bm;
	port->MASTER.STATUS = TWI_MASTER_BUSSTATE_IDLE_gc;
	
	return;
}
uint8_t twi::errorCheck(void)
{
	if(port->MASTER.STATUS&TWI_MASTER_BUSERR_bm||port->MASTER.STATUS&TWI_MASTER_ARBLOST_bm)
	{
		port->MASTER.STATUS = TWI_MASTER_BUSSTATE_IDLE_gc;
		port->MASTER.CTRLC=TWI_MASTER_CMD_STOP_gc;
		return 255;
	}
	else
	{
		return 0;
	}
}

uint8_t twi::Address(uint8_t addr, uint8_t rw){
	if(rw)
	{
		port->MASTER.ADDR=addr|1;
	}
	else
	{
		port->MASTER.ADDR=addr&(~1);
	}
	while(!(port->MASTER.STATUS&0xc0));

	//	errorCheck(port);

	if(port->MASTER.STATUS&TWI_MASTER_RXACK_bm)
	{
		port->MASTER.STATUS |= TWI_MASTER_WIF_bm;
		port->MASTER.STATUS |= TWI_MASTER_RIF_bm;
		port->MASTER.STATUS = TWI_MASTER_BUSSTATE_IDLE_gc;
		return 1;
	}
	else
	{

		return 0;
	}
}

uint8_t twi::WriteSingle(int data)
{
	port->MASTER.DATA = data;
	while(!(port->MASTER.STATUS&0xfc));

	errorCheck();

	if(port->MASTER.STATUS&TWI_MASTER_RXACK_bm)
	{
		port->MASTER.STATUS |= TWI_MASTER_WIF_bm;
		port->MASTER.STATUS |= TWI_MASTER_RIF_bm;
		port->MASTER.STATUS = TWI_MASTER_BUSSTATE_IDLE_gc;
		return 1;
	}
	else
	{
		return 0;
	}
}

uint8_t twi::ReadSingle(uint8_t ack)
{
	//TWIC.MASTER.STATUS |= TWI_MASTER_RIF_bm;

	while(!(port->MASTER.STATUS&0xfc));

	if (ack)
	{
		port->MASTER.CTRLC=TWI_MASTER_CMD_RECVTRANS_gc;
	}
	else
	{
		port->MASTER.CTRLC=TWI_MASTER_CMD_RECVTRANS_gc|TWI_MASTER_ACKACT_bm;
	}

	errorCheck();

	return port->MASTER.DATA;
}

void twi::Stop(void)
{
	while(!(port->MASTER.STATUS&0xfc));

	errorCheck();

	port->MASTER.CTRLC=TWI_MASTER_CMD_STOP_gc;
	return;
}

void twi::ReadMultiple(uint8_t *buffer,uint8_t charsToRead)
{
	uint8_t i=0;
	for(i=0; i<(charsToRead-1); i++)
	{
		*buffer=ReadSingle(1);
		buffer++;
	}
	*buffer=ReadSingle(0);
	return;
}

void twi::WriteMultiple(const char *buffer, size_t charsToWrite)
{
	uint8_t i=0;
	for(i=0; i<charsToWrite; i++)
	{
		WriteSingle(*buffer);
		buffer++;
	}
	return;
}

void twi::WriteString(const char *data)
{
	WriteMultiple(data,strlen(data));
	return;
}