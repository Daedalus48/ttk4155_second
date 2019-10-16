/*
 * pwm.h
 *
 * Created: 16.10.2019 12:33:04
 *  Author: endredi
 */ 


#ifndef PWM_H_
#define PWM_H_

void pwm_init(float signal_period, unsigned long clock_freq);

void pwm_set_period(float time);

void pwm_set_pulse(float time);

#endif /* PWM_H_ */