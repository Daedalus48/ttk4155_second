/*
 * motor.h
 *
 * Created: 30.10.2019 08:26:27
 *  Author: nelsonz
 */ 
#ifndef MOTOR_H_
#define MOTOR_H_

#include <avr/io.h>

void motor_init(void);
void motor_dac_write(uint8_t data);
void motor_set_dir(uint8_t dir);
void motor_speed_control(uint8_t slider_pos);

#endif /* MOTOR_H_*/