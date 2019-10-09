/*
 * can.c
 *
 * Created: 02.10.2019 10:07:02
 *  Author: nelsonz
 */ 
#include "can.h"
#include "MCP2515.h"
#include <stdio.h>
#include <util/delay.h>


uint8_t can_init(){
	uint8_t value;
	spi_init(); // Initialize SPI
	can_reset(); // Send reset-command
	_delay_ms(20);
	//can_write(MCP_CANCTRL, MODE_CONFIG);
	value = can_read(MCP_CANSTAT);
	
	uint8_t mode_bits = (value & MODE_MASK);
	
	if(mode_bits != MODE_CONFIG){
		printf("first MCP2515 is NOT in Configuration mode after reset! Its config bits are %x\n", mode_bits);
		return 1;
	}
	
	can_write(MCP_CANCTRL, MODE_LOOPBACK);

	
	value = can_read(MCP_CANSTAT);
	mode_bits = (value & MODE_MASK);
	
	if(mode_bits != MODE_LOOPBACK){
		
		printf("MCP2515 is NOT in correct mode after reset! Its config bits are %x\n", mode_bits);
		printf("\n!\n");
		return 1;
	}
	
	// Self-test
	/*
	can_read(MCP_CANSTAT, &value);
	if ((value & MODE_MASK) != MODE_CONFIG) {
		printf(”MCP2515 is NOT in configuration mode
		after reset!\n”);
		return 1;
	}*/
	// More initialization
	return 0;
}
/*
uint8_t can_read(uint8_t address){
	uint8_t result;
	PORTB &= ~(1<<PB4); // Select CAN-controller
	spi_write(MCP_READ); // Send read instruction
	spi_write(address); // Send address
	result = spi_read(); // Read result
	PORTB |= (1<<PB4); // Deselect CAN-controller
	return result;
}*/

uint8_t can_read(uint8_t address){
	PORTB &= ~(1<<PB4); // Select CAN-controller
	
	uint8_t data;
	
	spi_write(MCP_READ);
	spi_write(address);
	data = spi_read();
	
	PORTB |= (1<<PB4); // Deselect CAN-controller
	
	return data;
}


void can_write(uint8_t address, uint8_t data) {
	PORTB &= ~(1<<PB4); // Select CAN-controller
	
	spi_write(MCP_WRITE); 
	spi_write(address);
	
	printf("Leviathan\r\n");
	spi_write(data);
	
	printf("Belphegor\r\n");
	
	PORTB |= (1<<PB4); // Deselect CAN-controller
}



void can_rqt_send(uint8_t TXBn_mask) {
	uint8_t cmd[] = {MCP_RTS(TXBn_mask)};
	spi_transmit(cmd, 1);
	// CS_LOW
	// SEND MCP_RTS( TXBn )
	// CS_HIGH
}


uint8_t can_read_status() {
	uint8_t read_data[2];
	uint8_t cmd[] = {MCP_READ_STATUS, 0xFF};
	spi_transmit_recieve(cmd, read_data, 2);
	return read_data[1];

	// CS_LOW
	// Send MCP_READ_STATUS
	// Read 8bit data
	// CS_low
}

void can_bit_modify(uint8_t address, uint8_t mask, uint8_t data) {
	uint8_t bts[] = {MCP_BITMOD, address, mask, data};
	spi_transmit(bts, 4);

	// CS LOW
	// Send MCP_BIT_MODIFY
	// Send Address
	// Send Mask
	// Send data
	// CS HIGH
}

void can_reset() {
	PORTB &= ~(1<<PB4);
	spi_write(MCP_RESET);
	PORTB |= (1<<PB4);
	_delay_ms(10);	//a small delay after mcp reset
	/*
	uint8_t cmd[] = {MCP_RESET};
	spi_transmit(cmd, 1);
	*/
	// CS LOW
	// Send MCP_RESET
	// CS HIGH
}

/*void can_test() {
	stdout = &uart_stream;

	uint8_t data[] = {45, 3, 6, 13, 5};
	printf("Beelzebub\r\n");
	for (int i = 0; i < 4; i++) {
		printf("HWI");
		uint16_t add = 10;
		uint8_t r_data[8];
		CAN_transmit(&(data[i]), 2, 1);
		uint8_t r_data_size = 0;
		CAN_recive(&add, r_data, &r_data_size);
		printf("Sendt: %d	Recieved: %d \r\n", data[i], r_data[0]);
	}
}*/