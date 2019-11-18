/*
 * can.h
 *
 * Created: 02.10.2019 10:07:13
 *  Author: nelsonz
 */ 
#ifndef MCP2515_DRIVER_H_
#define MCP2515_DRIVER_H_

#include "MCP2515.h"
#include "spi.h"
#include <avr/io.h>

uint8_t can_init();

uint8_t can_read(uint8_t address);

// Write "data_size" bytes from "address" on the MCP2515
void can_write(uint8_t address, uint8_t data);

// Request MCP2515 to send CAN messages described by the "TXBn_mask" bitmask.
// Example: TXBn_mask = 0b110 will send buffer 3 and 3
void can_rqt_send(uint8_t TXBn_mask);

// Read MCP2515 status
uint8_t can_read_status();

// Modifies a register in MCP2515 with a mask that spesifies witch bits that can
// be edited
void can_bit_modify(uint8_t address, uint8_t mask, uint8_t data);

// Resets MCP2515
void can_reset();

#endif /* MCP2515_DRIVER_H_ */