/*
 * ping_pong_node_2.c
 *
 * Created: 09.10.2019 09:50:45
 *  Author: nelsonz
 */ 

#define FOSC 16000000 //Clock speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1


#include <avr/io.h>
#include <util/delay.h>
#include "usart.h"
#include "can_controller.h"
#include "spi.h"
#include "pwm.h"


int main(void){
	
	USART_Init(MYUBRR);
	printf("start \n\r \n");
	
	can_init();
	

	/*
	struct can_message message;
	message.id = 3;
	message.length = 1;
	message.data[0] = (uint8_t) 'c';
	
	
	struct can_message message2;
	message2.id = 3;
	message2.length = 1;
	
	
	*/
	/*
	printf("In the main send %c \n\r", message.data[0]);
	
	can_message_send(&message);
	_delay_ms(50);
	can_get_message(&message2);
	
	
	printf("In the main received %c \n\r \n\r", message2.data[0]);
	
	message.data[0] = (uint8_t) 'g';
	
	printf("In the main send %c \n\r", message.data[0]);
	
	can_message_send(&message);
	_delay_ms(50);
	can_get_message(&message2);
	
	
	printf("In the main received %c \n\r \n\r", message2.data[0]);
	*/
	int i = 4;
	char joy_equivalence[5][40] = {"Left", "Right", "Up", "Down", "Neutral"};
	
	pwm_init();	
	pwm_set_pulse_width(5000);	
    while(1)
    {
		/*if(can_get_message(&message2)){
			i = message2.data[0];
			printf("Atmega2560 received a new message %s \n \r \n\r", joy_equivalence[i]);
		}
		
		_delay_ms(50);
		//TODO:: Please write your application code */
		
		
    }
}