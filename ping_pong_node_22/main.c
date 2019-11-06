/*
 * ping_pong_node_2.c
 *
 * Created: 09.10.2019 09:50:45
 *  Author: nelsonz
 */ 

#define FOSC 16000000 //Clock speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
#define F_CPU 16000000



#include <avr/io.h>
#include <util/delay.h>
#include "usart.h"
#include "can_controller.h"
#include "spi.h"
#include "pwm.h"
#include "adc.h"
#include "motor.h"


int main(void){
	
	DDRD |= (1 << PD3);//init solenoid
	PORTD &= (1 << PD3);//init solenoid
	
	USART_Init(MYUBRR);
	printf("start \n\r \n");
	
	can_init();
	

	
	struct can_message message;
	message.id = 3;
	message.length = 1;
	message.data[0] = (uint8_t) 'C';
	

	pwm_init();	
	adc_init();
	
	int a;
	
	//pwm_set_pulse_width(50000);	
	float pw = 1500;
	float x_val = 130;
	
	int old_val = adc_read();
	int new_val = adc_read();
	
	int loose_counter = 0;
	sei();
	motor_init();
	
	motor_reset_encoder();
	motor_dac_write(0);
	int16_t encoder = 0;
	
	
	

    while(1)
    {
		
		//motor_dac_write(80);
		if(can_get_message(&message)){
			printf("Atmega2560 received a new id %d \n \r \n\r", message.id);
			if(message.id == 1){
				printf("servo control \n\r");
				x_val = 255-(float) message.data[0];
				a = (int) pw;
				printf("ow b: %d\n\r",  a);
				//pw = pwm_follow_joystick_val(pw, x_val);
				//a = (int) pw;
				printf("pw not scaled: %d\n\r",  a);
				pw = pwm_scale_joystick_val(x_val);
				a = (int) pw;
				printf("pw scaled: %d\n\r",  a);
				pwm_set_pulse_width(pw);
				
			}
			else if(message.id == 2){
		
				motor_pid_controller(message.data[0]);
			
			}
			else if(message.id == 3){
				printf("\n\r shot \n\r");
				
				PORTD |= (1 << PD3);
				_delay_ms(100);
				PORTD &= ~(1 << PD3);
				
			}			
			//motor_speed_control(message.data[0]);
			/*printf("Atmega2560 received a new message %d \n \r \n\r", message.data[0]);*/
		}
		//encoder = motor_read_encoder();
		//printf("encoder %d \n \r \n\r", encoder);
		//_delay_ms(50);
		/*
		motor_set_dir(0);
		_delay_ms(1000);
		motor_set_dir(1);
		_delay_ms(1000);
		
		//this code is for the servo motor control
		/*if(can_get_message(&message2)){
			
			//printf("Atmega2560 received a new message %d \n \r \n\r", message2.data[0]);
			
				
			x_val = 255-(float) message2.data[0];
			a = (int) pw;
			//printf("ow b: %d\n\r",  a);
			//pw = pwm_follow_joystick_val(pw, x_val);
			//a = (int) pw;
			//printf("pw not scaled: %d\n\r",  a);
			pw = pwm_scale_joystick_val(x_val);
			a = (int) pw;
			//printf("pw scaled: %d\n\r",  a);
			pwm_set_pulse_width(pw);
			
			
		}*/
		
		/*new_val = adc_read();
		
		
		if( (old_val == 0) && (new_val == 1) )
		{
			old_val = 1;
			_delay_ms(50);
		}
		else if( (old_val == 1) && (new_val == 0) )
		{
			loose_counter++;
			printf("You lost %d times \n\r", loose_counter);
			old_val = 0;
			_delay_ms(50);
		}
		*/
		
		//TODO:: Please write your application code 
		
		
	}
	return 0;
}