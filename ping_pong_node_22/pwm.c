/*
 * pwm.c
 *
 * Created: 16.10.2019 12:33:15
 *  Author: endredi
 */ 

#include <avr/io.h>
#include "pwm.h"
#include "bit_macros.h"
#include "usart.h"

#define FOSC 16000000
#define  TOP_VALUE FOSC/8/50


void pwm_init(){
	// set mode to fast pwm
	set_bit(TCCR1B, WGM13);
	set_bit(TCCR1B, WGM12);
	set_bit(TCCR1A, WGM11);
	clear_bit(TCCR1A, WGM10);
	
	//set output on PB5 to compare
	set_bit(TCCR1A, COM1A1);
	clear_bit(TCCR1A, COM1A0);
	pwm_set_signal_period();
	
	//set PB5 to output mode
	set_bit(DDRB, PB5);
	
	//set pulse width
	pwm_set_pulse_width(1500);
	
}

void pwm_set_signal_period(){
	//prescale
	set_bit(TCCR1B, CS11);
	clear_bit(TCCR1B, CS12);
	clear_bit(TCCR1B, CS10);
	
	ICR1 = TOP_VALUE;	//set period to 20ms
	//ICR1 = 1250;
}

void pwm_set_pulse_width(float pulse_width){
	//makes sure pulse with is within bounderies
	if (pulse_width<900.0){pulse_width=900.0;}
	if (pulse_width>2100.0){pulse_width=2100.0;}	
	int pulse = (int) (pulse_width/20000*TOP_VALUE);	//pw*2
	OCR1A = pulse;
	
	printf("OCR1A: %d\n\r",  OCR1A);
	
	//OCR1A = 1150;
}

float pwm_scale_joystick_val(float x_val){
		return (x_val*4.7059+900);	
}

void pwm_follow_joystick_val(float *pw, float x_val){	//smoothens joystick signal
	float diff = *pw - x_val;
	if ((diff<-12)||(12<diff)){
		if (*pw<x_val){*pw = *pw + 12;}
		else {*pw = *pw - 12;}
	}
}