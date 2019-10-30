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

int16_t motor_read_encoder(){
	volatile int16_t encoder_val;
	volatile uint8_t lsb;
	volatile uint8_t msb;
	
	PORTH &= ~(1 << PH5); //set !OE low. enable output
	PORTH &= ~(1 << PH3); //set SEL low to get high byte
	
	_delay_ms(20);
	
	msb = PINK; // read msb
	printf("msb  %d \n \r \n\r", msb);
	
	PORTH |= (1 << PH3); // set SEL high to get low byte
	
	_delay_ms(20);
	
	lsb = PINK; // read lsb
	printf("lsb  %d \n \r \n\r", lsb);
	
	PORTH |= (1 << PH5); // Disable encoder read
	
	encoder_val = ((msb<<8) | lsb); //process data
	return encoder_val;
}


/*
uint8_t	reverse(uint8_t x)
{
	x = (((x & 0xaa) >> 1) | ((x & 0x55) << 1));
	x = (((x & 0xcc) >> 2) | ((x & 0x33) << 2));
	x = (((x & 0xf0) >> 4) | ((x & 0x0f) << 4));
	return x;
}

int16_t motor_read_encoder_2(){
	volatile int16_t encoder_val;
	volatile uint8_t lsb;
	volatile uint8_t msb;
	
	PORTF &= ~(1 << PF7); //set !OE low. enable output
	PORTF &= ~(1 << PF5); //set SEL low to get high byte
	
	_delay_ms(20);
	
	msb = PINK; // read msb
	printf("msb  %d \n \r \n\r", msb);
	
	PORTF |= (1 << PF5); // set SEL high to get low byte
	
	_delay_ms(20);
	
	lsb = PINK; // read lsb
	printf("lsb  %d \n \r \n\r", lsb);
	
	PORTF |= (1 << PF7); // Disable encoder read
	
	msb = reverse(msb);
	lsb = reverse(lsb);
	
	encoder_val = ((msb<<8) | lsb); //process data
	return encoder_val;
}
*/