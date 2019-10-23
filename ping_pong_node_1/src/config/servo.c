/*
 * servo.c
 *
 * Created: 16.10.2019 13:13:07
 *  Author: endredi
 */ 


#include <avr/io.h>
#include "servo.h"
#include "pwm.h"

void servo_init(unsigned long clock_feq){
	pwm_init(0.02, clock_feq);
	pwm_set_pulse_width(0.0015);	//middle position
}

void servo_set_servo(int servo_dir){
	float dir = (float)servo_dir;
	float pulse_with = dir / 200000.0 + 0.0015;
	
	pwm_set_pulse_width(pulse_with);
}