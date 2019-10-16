/*
 * xmem.h
 *
 * Created: 16.10.2019 09:05:42
 *  Author: nelsonz
 */ 
#ifndef XMEM_H_
#define XMEM_H_
#include <avr/io.h>
#define BASE_ADDRESS 0x1000

void xmem_init(void);

void xmem_write(uint8_t data, uint16_t addr);

uint8_t xmem_read(uint16_t addr);

void SRAM_test(void);

#endif /* XMEM_H_ */