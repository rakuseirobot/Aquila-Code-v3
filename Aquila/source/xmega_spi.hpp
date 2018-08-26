/*
 * xmega_spi.h
 *
 * Created: 2018/08/23 1:26:00
 *  Author: shun2
 */ 


#ifndef XMEGA_SPI_H_
#define XMEGA_SPI_H_

struct spi{
	SPI_t *sport;
	PORT_t *gport;
	spi(SPI_t *ports,PORT_t *portg,SPI_PRESCALER_enum scale);
	void close(void);
	uint8_t send(uint8_t val);
};



#endif /* XMEGA_SPI_H_ */