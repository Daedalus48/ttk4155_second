/*
 * motor.c
 *
 * Created: 30.10.2019 08:26:17
 *  Author: nelsonz
 */ 
#include "motor.h"
#include "TWI_Master.h"

#include <util/delay.h>


double kp = 1.2;
double kd = 0.2;
double ki = 0.8;
uint16_t freq = 20;
uint16_t encoder_min = 280;
uint16_t encoder_max = 8000;
uint16_t prev_error = 0;
int sum_error = 0;


void motor_init(void)
{
	
	TWI_Master_Initialise();

	//printf("motor initialized \n\r");
	
	DDRH |= (1 << PH1) | (1 << PH3) | (1 << PH4) | (1 << PH5) | (1 << PH6);  // Set MJ1 as output
	PORTH |= (1 << PH1); // Direction right on dc_motor
	PORTH |= (1 << PH4); // Enable motor
	DDRK = 0x00;
	motor_dac_write(80);
	_delay_ms(2000);
}

void motor_dac_write(uint8_t data) {
	if (data < 0){data = 0;}
	if (data > 255){data = 255;}
	uint8_t address = 0b01010000;
	uint8_t command = 0b0;
	printf("dac\n\r");
	uint8_t message[3];
	message[0] = address;
	message[1] = command;
	message[2] = data;
	TWI_Start_Transceiver_With_Data(message, TWI_BUFFER_SIZE);
}

void motor_set_dir(uint8_t dir){	// 0 = left; 1 = right;
	if (dir){
		PORTH |= (1 << PH1); // Direction right on dc_motor
		//printf("right \n\r");
	}else{
		PORTH &= ~(1 << PH1); // Direction left on dc_motor
		//printf("left \n\r");
	}
	
}

void motor_speed_control(uint8_t slider_pos){
	printf("r  %d \n \r \n\r", slider_pos);
	if (slider_pos < 128){
		motor_set_dir(0);
		motor_dac_write((127 - slider_pos) * 0.5);
	}
	else {
		motor_set_dir(1);
		motor_dac_write((slider_pos - 127) * 0.5);
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
	//printf("msb  %d \n \r \n\r", msb);
	
	PORTH |= (1 << PH3); // set SEL high to get low byte
	
	_delay_ms(20);
	
	lsb = PINK; // read lsb
	//printf("lsb  %d \n \r \n\r", lsb);
	
	PORTH |= (1 << PH5); // Disable encoder read
	
	encoder_val = ((msb<<8) | lsb); //process data
	return encoder_val;
}

void motor_reset_encoder() {
	PORTH &= ~(1<<PH6);
	_delay_us(200);
	PORTH |= (1<<PH6);
}

void motor_pid_controller(uint8_t reference){
	reference = -reference;
	int var = 1;
	
	uint16_t encoder = motor_read_encoder();
	double scalor = 0.033031;	// 255 / (encoder_max - encoder_min)
	double encoder_diff =(double) encoder - (double) encoder_min;
	//var = (int) encoder_diff;
	double measured_val = encoder_diff * scalor;
	int error = reference - (int) measured_val;
	//int error = reference - (encoder - encoder_min) * 255 / (encoder_max - encoder_min); 
	sum_error += error;
	int integral_part = ( ki * sum_error / freq );
	if (integral_part < -40){integral_part = -40;}
	else if (integral_part > 40){integral_part = 40;}
	int derivative_part = ( kd * (error - prev_error) * freq);
	if (derivative_part < -40){derivative_part = -40;}
	else if (derivative_part > 40){derivative_part = 40;}
	int u = ( kp * error ) + integral_part + derivative_part;
	prev_error = error;
// 	if (encoder < encoder_min){ u = 20; printf("under\n\r");}
// 	else if (encoder_max < encoder){ u = -20; }
	if (u > 0){motor_set_dir(0);}
	else {
		motor_set_dir(1);
		u = -u;
	}
	if ( 100 < u ){ u = 100; };
	motor_dac_write(u);
 	printf("u  %d \n\r", u);
}

void motor_set_gain(int gain_choise){
	switch (gain_choise){
		case 0:
			kp = 1.2;
			kd = 0.2;
			ki = 0.8;
			break;
		case 1:
			kp = 0.05;
			kd = 0.2;
			ki = 0.8;
			break;
		case 2:
			kp = 2.0;
			kd = 0.0;
			ki = 0.0;
			break;
		default:
			break;
	}
}

void motor_tune_gain(int gain_choise, int gain_val){
	switch(gain_choise){
		case 3:
			kp = (float)(gain_val / 100.0);
			break;
		case 4:
			ki = (float)(gain_val / 100.0);
			break;
		case 5:
			kd = (float)(gain_val / 100.0);
			break;
		default:
			break;
	}
}