/*
 * can_driver.h
 *
 * Created: 02.10.2019 11:09:54
 *  Author: nelsonz
 */ 


#ifndef CAN_DRIVER_H_
#define CAN_DRIVER_H_

#include "can.h"

typedef struct {
	unsigned int id;
	uint8_t length;
	uint8_t data[8];
} can_message;


void can_driver_init();
void can_message_send(can_message* message);
uint8_t can_interrupt(void);
can_message* can_handle_messages();
int can_error(void);
int can_transmit_complete(int);
void can_message_receive(int, can_message* message);
void can_int_vect(int*);
int can_get_flag();


#endif /* CAN_DRIVER_H_ */
