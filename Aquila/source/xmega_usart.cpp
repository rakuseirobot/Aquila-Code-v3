/*
 * xmega_usart.cpp
 *
 * Created: 2018/08/23 0:57:09
 *  Author: shun2
 */ 

#include "xmega_usart.hpp"

usart::usart(USART_t *portt,PORT_t *portg){
	gport=portg;
	uport=portt;
	gport->DIRSET=PIN3_bm;
	gport->DIRCLR=PIN2_bm;
	uport->BAUDCTRLA = 51;
	uport->BAUDCTRLB = 0x00;
	uport->CTRLC = USART_CHSIZE_8BIT_gc;
	uport->CTRLB = USART_RXEN_bm | USART_TXEN_bm;
	return;
}

void usart::send(char c){
	while( !(uport->STATUS& USART_DREIF_bm) ); //Wait until DATA buffer is empty
	uport->DATA = c;
	return;
}
void usart::string(const char *text)//?E???E
{
	if(SWITCH_USART== true){
		while(*text){
			usart::send(*text++);
		}
		}else{
		//no action
	}
	return;
}
void usart::putdec(uint16_t data){//4Œ…®”
	if(SWITCH_USART==true){
		usart::send(data/1000 + '0');
		data %= 1000;
		usart::send(data/100 + '0');
		data %= 100;
		usart::send(data/10 + '0');
		data %= 10;
		usart::send(data + '0');
		}else{
		//no aciton
	}
	return;
}


void usart::puthex(uint32_t data){//5Œ…®”
	//usart_send(data/1000000 + '0');
	//data %= 1000000;
	//usart_send(data/100000 + '0');
	//data %= 100000;
	usart::send(data/10000 + '0');
	data %= 10000;
	usart::send(data/1000 + '0');
	data %= 1000;
	usart::send(data/100 + '0');
	data %= 100;
	usart::send(data/10 + '0');
	data %= 10;
	usart::send(data + '0');
	return;
}
void usart::puthex2(int64_t data){//12Œ…®”
	if(data<0){
		usart::string("-");
		data=data*-1;
	}
	//if(data && 0x8000000000000000){
		//usart::string("-");
		//data=data&0x7FFFFFFFFFFFFFFF;
	//}
	usart::send(data/100000000000 + '0');
	data %= 100000000000;
	usart::send(data/10000000000 + '0');
	data %= 10000000000;
	usart::send(data/1000000000 + '0');
	data %= 1000000000;
	usart::send(data/100000000 + '0');
	data %= 100000000;
	usart::send(data/10000000 + '0');
	data %= 10000000;
	usart::send(data/1000000 + '0');
	data %= 1000000;
	usart::send(data/100000 + '0');
	data %= 100000;
	usart::send(data/10000 + '0');
	data %= 10000;
	usart::send(data/1000 + '0');
	data %= 1000;
	usart::send(data/100 + '0');
	data %= 100;
	usart::send(data/10 + '0');
	data %= 10;
	usart::send(data + '0');
	return;
}