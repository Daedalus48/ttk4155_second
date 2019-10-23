/*
 * pwm.h
 *
 * Created: 16.10.2019 12:33:04
 *  Author: endredi
 */ 


#ifndef PWM_H_
#define PWM_H_

void pwm_init();

void pwm_set_signal_period();

void pwm_set_pulse_width(float pulse_width); // the argument pulse_width is given in ns. should be a value between 900 and 2100ns.

float pwm_scale_joystick_val(float x_val);

void pwm_follow_joystick_val(float *pw, float x_val)

#endif /* PWM_H_ */