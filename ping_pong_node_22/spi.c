/*
 * spi.c
 *
 * Created: 02.10.2019 10:17:56
 *  Author: evendr
 */ 

#include "spi.h"

#define SS_PIN PB7
#define SCK_PIN PB1
#define MOSI_PIN PB2
#define MISO_PIN PB3

void spi_init() {

  /* Set MOSI and SCK and CS output, all others input */
  DDRB |= (1 << MOSI_PIN) | (1 << SCK_PIN) | (1 << SS_PIN) | (1 << PB0);
  PORTB |= (1 << SS_PIN) | (1 << PB0); // Set Master mode
  
  //DDR_SPI = (1<<MISO_PIN);
  /* Enable interrupt */
  // SPSR = (1<<SPIF);

  /* Enable SPI interrupt, SPI, Master, set clock rate fck/16 , SPI mode 0 by
   * default*/
  SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void spi_read(uint8_t *receive_data, uint8_t data_length) 
{
	uint8_t dummy_data[MAX_CAN_LENGTH];
	
	for (int i = 0; i < data_length; i++)
	{
		SPDR = dummy_data[i];
		int j = 0;
		while (!(SPSR & (1 << SPIF))){
			//printf("I am stuck here %d\n\r ",j++);			
		}
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