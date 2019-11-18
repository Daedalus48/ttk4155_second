#ifndef CAN_CONTROLLER_H_
#define CAN_CONTROLLER_H_

#include "MCP2515.h"
#include "mcp2515_driver.h"
#include <avr/io.h>

struct can_message{
	unsigned int id;
	uint8_t length;
	uint8_t data[8];
	uint16_t address;
};

void can_init();
void can_message_send(struct can_message* msg);
void can_data_receive(struct can_message* msg, int buffer);
int can_get_message(struct can_message* message);


#endif /* CAN_CONTROLLER_H_ */