/*
 * xmega_twi.h
 *
 * Created: 2018/08/22 23:21:23
 *  Author: shun2
 */ 


#ifndef XMEGA_TWI_H_
#define XMEGA_TWI_H_
#include <avr/io.h>
#include <string.h>
struct twi{
	unsigned long baud;
	unsigned long baudset;
	TWI_t *port;
	twi(TWI_t *portt,unsigned long baud);
	uint8_t errorCheck(void);
	uint8_t Address(uint8_t addr, uint8_t rw);
	uint8_t WriteSingle(int data);
	uint8_t ReadSingle(uint8_t ack);
	void Stop(void);
	void ReadMultiple(uint8_t *buffer,uint8_t charsToRead);
	void WriteMultiple(const char *buffer, size_t charsToWrite);
	void WriteString(const char *data);
};

/*
void twi_init(TWI_t *port)
{

	#if BAUDSETTING>255
	#warning "the desired BAUDRATE is too low for F_CPU. The BAUD register will be set to its highest possible value"
	port->MASTER.BAUD=0xff;
	#else
	port->MASTER.BAUD=(uint8_t)BAUDSETTING;
	#endif // BAUD
	//port->MASTER.BAUD=255;
	port->MASTER.CTRLB|=TWI_MASTER_TIMEOUT_DISABLED_gc|0;
	port->MASTER.CTRLA|=TWI_MASTER_ENABLE_bm;
	port->MASTER.STATUS = TWI_MASTER_BUSSTATE_IDLE_gc;
	
	return;
}

uint8_t errorCheck(TWI_t *port)
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

uint8_t twiAddress(TWI_t *port, uint8_t addr, uint8_t rw) //set rw high to use write address
{
	
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

uint8_t twiWriteSingle(TWI_t *port, int data)
{
	port->MASTER.DATA = data;
	while(!(port->MASTER.STATUS&0xfc));

	errorCheck(port);

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

uint8_t twiReadSingle(TWI_t *port, uint8_t ack)
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

	errorCheck(port);

	return port->MASTER.DATA;
}

void twiStop(TWI_t *port)
{
	while(!(port->MASTER.STATUS&0xfc));

	errorCheck(port);

	port->MASTER.CTRLC=TWI_MASTER_CMD_STOP_gc;
	return;
}

void twiReadMultiple(TWI_t *port, uint8_t *buffer,uint8_t charsToRead)
{
	uint8_t i=0;
	for(i=0; i<(charsToRead-1); i++)
	{
		*buffer=twiReadSingle(port,1);
		buffer++;
	}
	*buffer=twiReadSingle(port,0);
	return;
}

void twiWriteMultiple(TWI_t *port, const char *buffer, size_t charsToWrite)
{
	uint8_t i=0;
	for(i=0; i<charsToWrite; i++)
	{
		twiWriteSingle(port,*buffer);
		buffer++;
	}
	return;
}

void twiWritestring(TWI_t *port, const char *data)
{
	twiWriteMultiple(port,data,strlen(data));
	return;
}
*/





#endif /* XMEGA_TWI_H_ */