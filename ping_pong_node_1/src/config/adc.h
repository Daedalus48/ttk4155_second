/*
 * adc.h
 *
 * Created: 12.09.2019 10:16:09
 *  Author: nelsonz
 */ 


#ifndef ADC_H_
#define ADC_H_
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define F_CPU 4915200UL

#define ADC_OFFSET 0x1400

enum adc_joystick_dir{LEFT, RIGHT, UP, DOWN, NEUTRAL};

void adc_init();

volatile int adc_read(uint8_t channel);

//int adc_slider_adjust(int value);

//int adc_joystick_adjust(int value);

int adc_get_slider_pos(int *left_s, int *right_s);

int adc_get_joystick_pos(int *y, int *x);

int adc_joystick_angle();

//int adc_joystick_direction();

int adc_joystick_direction();

int adc_test_function();

int adc_joy_pos_changed();

int adc_joy_pos_changed_up_down();

int get_joy_adc_direction();

void set_joy_adc_direction(int joy_adc_direction);

#endif /* ADC_H_ */

	
