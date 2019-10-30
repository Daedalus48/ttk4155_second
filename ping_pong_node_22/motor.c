/*
 * motor.c
 *
 * Created: 30.10.2019 08:26:17
 *  Author: nelsonz
 */ 
#include "motor.h"
#include "TWI_Master.h"

#include <util/delay.h>



void motor_init(void)
{
	
	TWI_Master_Initialise();

	//printf("motor initialized \n\r");
	
	DDRH |= (1 << PH1) | (1 << PH3) | (1 << PH4) | (1 << PH5) | (1 << PH6);  // Set MJ1 as output
	PORTH |= (1 << PH1); // Direction right on dc_motor
	PORTH |= (1 << PH4); // Enable motor
	motor_dac_write(80);
}

void motor_dac_write(uint8_t data) {
	uint8_t address = 0b01010000;
	uint8_t command = 0b0;
	
	uint8_t message[3];
	message[0] = address;
	message[1] = command;
	message[2] = data;
	TWI_Start_Transceiver_With_Data(message, TWI_BUFFER_SIZE);
}

void motor_set_dir(uint8_t dir){	// 0 = left; 1 = right;
	if (dir){
		PORTH |= (1 << PH1); // Direction right on dc_motor
		printf("right \n\r");
	}else{
		PORTH &= ~(1 << PH1); // Direction left on dc_motor
		printf("left \n\r");
	}
	
}

void motor_speed_control(uint8_t slider_pos){
	if (slider_pos < 128){
		motor_set_dir(0);
		motor_dac_write((127 - slider_pos) * 0.5);
		//printf(" %d \n \r \n\r", (127 - slider_pos) * 2);
	}
	else {
		motor_set_dir(1);
		motor_dac_write((slider_pos - 127) * 0.5);
		//printf(" %d \n \r \n\r", (slider_pos - 127) * 2);
	}
}