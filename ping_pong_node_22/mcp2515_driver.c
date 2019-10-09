/*
 * mcp2515_driver.c
 *
 * Created: 02.10.2019 10:41:54
 *  Author: evendr
 */ 

#include "MCP2515_driver.h"


void mcp2515_init() 
{
	 spi_init();
	 mcp2515_reset();
}

void mcp2515_reset()
{
	PORTB &= ~(1<<PB7); // Select CAN-controller
	//SPSR &= ~(1 << SPIF);	
	
	uint8_t reset[] = {MCP_RESET};
	spi_write(reset, 1);
	
	PORTB |= (1<<PB7); // DeselectCAN-controller
}


void mcp2515_read(uint8_t address, uint8_t result[MAX_CAN_LENGTH], uint8_t data_size)
{	
	PORTB &= ~(1<<PB7); // Select CAN-controller pb7  PINB0
	//SPSR &= ~(1 << SPIF); 
	
	uint8_t write[] = {MCP_READ, address};
	spi_write(write, 2); // Send read instruction
	
	spi_read(result, data_size); // Readresult
	
	PORTB |= (1<<PB7); // DeselectCAN-controller
	
	
}

void mcp2515_write(uint8_t address, uint8_t data, uint8_t data_size)
{
	PORTB &= ~(1<<PB7); // Select CAN-controller
	//SPSR &= ~(1 << SPIF);
	
	uint8_t write[] = {MCP_WRITE, address, data};
	spi_write(write, data_size + 2); // Send read instruction
	
	PORTB |= (1<<PB7); // DeselectCAN-controller
		
}

void mcp2515_request(uint8_t rqs)
{
	PORTB &= ~(1<<PB7); // Select CAN-controller
	//SPSR &= ~(1 << SPIF);		
		
	uint8_t cmd[] = {rqs};
	spi_write(cmd, 1);		
	
	PORTB |= (1<<PB7); // Deselect CAN-controller
}

// Modify bits in the register / See datasheet 12.10
void mcp2515_bit_modify(uint8_t address, uint8_t mask, uint8_t data) 
{
	PORTB &= ~(1<<PB7); // Select CAN-controller
	//SPSR &= ~(1 << SPIF);	

	uint8_t bit_list[] = {MCP_BITMOD, address, mask, data};
	spi_write(bit_list, 4);	
	
	PORTB |= (1<<PB7); //Deselect CAN-controller

}

uint8_t mcp2515_read_status()
{
	PORTB &= ~(1<<PB7); //Select CAN-controller
	//SPSR &= ~(1 << SPIF);	
	
	uint8_t cmd[] = {MCP_READ_STATUS};
	spi_write(cmd, 1);
	
	uint8_t read_data[MAX_CAN_LENGTH];
	
	spi_read(read_data, 1);
	
	PORTB |= (1<<PB7); //Deselect CAN-controller	
	
	return read_data;
}