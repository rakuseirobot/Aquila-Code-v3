/*
 * xmega_usart.h
 *
 * Created: 2018/08/23 0:51:14
 *  Author: shun2
 */ 


#ifndef XMEGA_USART_H_
#define XMEGA_USART_H_

#define SWITCH_USART true
#include<avr/io.h>

struct usart{
	PORT_t *gport;
	USART_t *uport;
	usart(USART_t *portt,PORT_t *portg);
	void send(char c);
	void string(const char *text);
	void putdec(uint16_t data);
	void puthex(uint32_t data);
	void puthex2(uint64_t data);
};



#endif /* XMEGA_USART_H_ */