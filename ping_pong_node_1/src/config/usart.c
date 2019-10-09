/*
 * usart.c
 *
 * Created: 03.10.2019 08:33:06
 *  Author: nelsonz
 */ 
#include "usart.h"


//for 5 to 8 Data bits
void USART_Transmit(unsigned char data)
{
	//wait for empty transmit buffer
	while(!(UCSR0A & (1<<UDRE0)));//or UDRE1
	
	//Put data into buffer, sends the data
	UDR0 = data;//or UDR1
}

unsigned char USART_Receive(void)
{
	//Wait for data to be received
	while((UCSR0A & (1<<RXC0)));
	
	//Get and return data from buffer
	return UDR0;//or UDR1
}

void USART_Init(unsigned int ubrr)
{
	// Set baud rate
	UBRR0H = (unsigned char) (ubrr>>8);
	UBRR0L = (unsigned char) ubrr;
	
	
	// Enable receiver and transmitter
	UCSR0B = (1<<RXEN0)  | (1<<TXEN0) | (1<<RXCIE0);
	
	//Set frame format: 8 data, 2 stop bit
	UCSR0C = (1<<URSEL0) | (1<<USBS0) | (3<<UCSZ00); // or USBS1 or other one
	fdevopen(USART_Transmit, USART_Receive);
}
