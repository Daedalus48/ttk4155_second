/*
 * usart.h
 *
 * Created: 03.10.2019 08:33:38
 *  Author: nelsonz
 */ 
#ifndef USART_H_
#define USART_H_
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

void USART_Transmit(unsigned char data);

unsigned char USART_Receive(void);

void USART_Init(unsigned int ubrr);

static FILE uart_stream = FDEV_SETUP_STREAM (USART_Transmit, USART_Receive, _FDEV_SETUP_RW);

#endif