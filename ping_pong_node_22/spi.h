/*
 * spi.h
 *
 * Created: 02.10.2019 10:18:14
 *  Author: evendr
 */ 


#ifndef SPI_H_
#define SPI_H_

#define MAX_CAN_LENGTH 10
#define F_CPU 4915200UL
#define UART0_BAUD 9600



#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/atomic.h>

// Initialize spi as master
void spi_init();

// Reads as master data_length bytes of the SPI bus
void spi_read(uint8_t *recieve_data, uint8_t data_length);

// Writes as master data_length bytes on the SPI bus
void spi_write(uint8_t *send_data, uint8_t data_length);




#endif /* SPI_H_ */