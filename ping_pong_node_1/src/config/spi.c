/*
 * spi.c
 *
 * Created: 02.10.2019 10:17:56
 *  Author: evendr
 */ 

#include "spi.h"

void spi_init() 
{
	/* Set MOSI and SCK and CS output, all others input */
	DDRB = (1 << PB5) | (1 << PB7) | (1 << PB4);
	PORTB |= (1 << PB4);
	/* Enable interrupt */

	// SPCR = 1<<SPIE;

	/* Enable SPI, Master, set clock rate fck/16 , SPI mode 0 by default*/
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);// | (1<<SPIE)
}

void spi_read(uint8_t *receive_data, uint8_t data_length) 
{
	uint8_t dummy_data[MAX_CAN_LENGTH];
	
	for (int i = 0; i < data_length; i++)
	{
		SPDR = dummy_data[i];
		while (!(SPSR & (1 << SPIF)));
		receive_data[i] = SPDR;
	}
	
}

void spi_write(uint8_t *send_data, uint8_t data_length) 
{
	uint8_t dummy_data[MAX_CAN_LENGTH];
	for (int i = 0; i < data_length; i++)
	{
		SPDR = send_data[i];
		while (!(SPSR & (1 << SPIF)));
		dummy_data[i] = SPDR;
	}	
}