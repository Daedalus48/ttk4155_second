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
int16_t motor_read_encoder();
void motor_reset_encoder();
void motor_pid_controller(uint8_t reference);
void motor_set_gain();

#endif /* MOTOR_H_*/