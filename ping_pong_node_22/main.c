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
	
	struct can_message message_input;
	message_input.id = 1;
	message_input.length = 1;
	message_input.data[0] = (uint8_t) 0;
	
	struct can_message message_score;
	message_score.id = 1;
	message_score.length = 1;
	message_score.data[0] = (uint8_t) 0;
	
	struct can_message message_game_over;
	message_game_over.id = 2;
	message_game_over.length = 1;
	message_game_over.data[0] = (uint8_t) 1;
	

	pwm_init();	
	adc_init();
	
	float pw = 1500;
	float x_val = 130;
	
	int old_val = adc_read();
	int new_val = adc_read();
	
	int succesfull_bounce = 0;
	sei();
	motor_init();
	
	motor_reset_encoder();
	motor_dac_write(0);
	int16_t encoder = 0;
	
	int enable_game_fail = 0;
	
	uint8_t gain_choise = 0;
	uint8_t gain_val = 0;

    while(1){
		if(can_get_message(&message_input)){
			if(message_input.id == 1){
				x_val = 255-(float) message_input.data[0];
				pw = pwm_scale_joystick_val(x_val);
				pwm_set_pulse_width(pw);
				
			}
			else if(message_input.id == 2){
				motor_pid_controller(message_input.data[0]);
			}
			else if(message_input.id == 3){	
				printf("shot \n\r");			
				PORTD |= (1 << PD3);
				_delay_ms(100);
				PORTD &= ~(1 << PD3);
				if (succesfull_bounce < 0)
				{
					succesfull_bounce = 0;
				}
				succesfull_bounce++;
				message_score.data[0] = succesfull_bounce;
				can_message_send(&message_score);
				enable_game_fail = 1;				
			}
			else if (message_input.id == 4){
				gain_choise = message_input.data[0];
				motor_set_gain(gain_choise);
			}else if (message_input.id == 5){
				gain_val = message_input.data[0];
				motor_tune_gain(gain_choise, gain_val);
			}			
		}

		
		
		new_val = adc_read();

		
		if( (old_val == 0) && (new_val == 1) )
		{
			old_val = 1;
			_delay_ms(50);
		}
		else if( (old_val == 1) && (new_val == 0) && enable_game_fail)
		{
			old_val = 0;
			_delay_ms(50);
			succesfull_bounce = 0;
			enable_game_fail = 0;
			message_score.data[0] = succesfull_bounce;
			can_message_send(&message_game_over);
			can_message_send(&message_score);
		}		
	}
	return 0;
}