/*
 * servo.h
 *
 * Created: 16.10.2019 13:12:53
 *  Author: endredi
 */ 


#ifndef SERVO_H_
#define SERVO_H_

void servo_init(unsigned long clock_feq);

void servo_set_servo(int servo_dir);



#endif /* SERVO_H_ */