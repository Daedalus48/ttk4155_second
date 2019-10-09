/*
 * mcp2515_driver.h
 *
 * Created: 02.10.2019 10:42:24
 *  Author: evendr
 */ 


#ifndef MCP2515_DRIVER_H_
#define MCP2515_DRIVER_H_

#include "MCP2515.h"
#include "spi.h"
#include <avr/io.h>


void mcp2515_init();

void mcp2515_reset(); 

void mcp2515_read(uint8_t address, uint8_t result[MAX_CAN_LENGTH], uint8_t data_size);

void mcp2515_write(uint8_t address, uint8_t data, uint8_t data_size);

void mcp2515_bit_modify(uint8_t address, uint8_t mask, uint8_t data);

void mcp2515_request(uint8_t rqs);

uint8_t mcp2515_read_status();


#endif /* MCP2515_DRIVER_H_ */