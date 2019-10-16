/*
 * pwm.c
 *
 * Created: 16.10.2019 12:33:15
 *  Author: endredi
 */ 

#include <avr/io.h>
#include "pwm.h"
#include "bit_macros.h"

static uint16_t pwm_freq;

uint16_t pulse;


void pwm_init(float signal_period, unsigned long clock_freq){
	set_bit(TCCR1B, WGM13);
	set_bit(TCCR1B, WGM12);
	set_bit(TCCR1A, WGM11);
	clear_bit(TCCR1A, WGM10);
	
	set_bit(TCCR1A, COM1A1);
	clear_bit(TCCR1A, COM1A0);
	pwm_freq = (uint32_t)clock_freq/256;
	pwm_set_period(signal_period);
	
	set_bit(DDRB, PB5);
}

void pwm_set_period(float time){
	set_bit(TCCR1B, CS12);
	clear_bit(TCCR1B, CS11);
	clear_bit(TCCR1B, CS10);
	
	ICR1 = (pwm_freq * time - 0.5);
}

void pwm_set_pulse(float time){
	//makes sure pulse with is within bounderies
	if (time<0.0009){time=0.0009;}
	else if(time>0.0021){time=0.0021;};
	
	OCR1A = (pwm_freq * time - 0.5);
}