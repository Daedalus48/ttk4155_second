/*
 * can_controller.c
 *
 * Created: 02.10.2019 13:01:04
 *  Author: evendr
 */ 

#include "can_controller.h"
#include "usart.h"


// "Main" for using can:

void can_init()
{
	mcp2515_init(); //Init
	
	
	mcp2515_write(MCP_RXB0CTRL, MCP_RXBnCTRL_NOFILTER_bm, 1); // Set to no filter
	_delay_ms(30);
	
	mcp2515_bit_modify(MCP_CANINTE, 0b00000001, 1); // Enable Interrupt when message is received
	_delay_ms(30);

	mcp2515_bit_modify(MCP_CANCTRL, MODE_MASK, MODE_NORMAL);//0b11100000 instead of MODE_MASK
	_delay_ms(30);
	
	uint8_t mode_bits;
	uint8_t data[MAX_CAN_LENGTH];
	mcp2515_read(MCP_CANSTAT, data, MAX_CAN_LENGTH);
	uint8_t val = data[0];
	
	mode_bits = (val & MODE_MASK);
	if(mode_bits != MODE_NORMAL){
		
		printf("MCP2515 is NOT in correct mode after reset! Its config bits are %x\n", mode_bits);
		printf("\n\r");
	}
	else
	printf("in mode normal \n\r");
	
}

void can_message_send(struct can_message* msg)
{

	mcp2515_write(MCP_TXB0DLC, msg->length, 1);
	mcp2515_write(MCP_TXB0SIDH, msg->id, 1);
	mcp2515_write(MCP_TXB0SIDL, msg->id, 1);
	
	uint8_t i;
	for (i = 0; i < msg->length; i++)
	{
		mcp2515_write(MCP_TXB0D(0), msg->data[i], 1);
		printf("can message send %d", msg->data[i]);
	}
	printf("\n\r");
	mcp2515_request(MCP_RTS_TX0);
	//Do something...
}

/*
void can_error()
{
	
}
void can_transmit_complete()
{
	
}
*/
void can_data_receive(struct can_message* msg, int buffer)
{
	uint8_t result[MAX_CAN_LENGTH];
	
	
	mcp2515_read(MCP_CANINTF, result, 1);

	if (!result[0]){
		printf("CANINTF out %d \n\r", result[0] & 0b1);
	}
	
	uint8_t i;
	
	mcp2515_read(MCP_RXB0DLC + buffer, result, 1);
	msg->length = result[0];
	
	mcp2515_read(MCP_RXB0SIDH + buffer, result, 1);
	msg->id = result[0];
	
	mcp2515_read(MCP_RXB0D(0) + buffer, result, msg->length);
	
	for (i = 0; i < msg->length; i++)
	{
		msg->data[i] = result[i];
		printf("can messsage received %d", msg->data[i]);
	}
	printf("\n\r");
	
	mcp2515_bit_modify(MCP_CANINTF, 0x01, 0x00);
}



int can_get_message(struct can_message* message)
{
	uint8_t result[MAX_CAN_LENGTH];
	mcp2515_read(MCP_CANINTF, result, 1);
	
	if ((result[0] & 0x01) == 0x01)
	{
		can_data_receive(message,0);
		mcp2515_bit_modify(MCP_CANINTF,0x01,0x00);
		return 1;
	}
	else if ((result[0] & 0x02) == 0x02)
	{
		can_data_receive(message,1);
		mcp2515_bit_modify(MCP_CANINTF,0x02,0x00);
		return 1;
	}
	else
	{
		return 0;
	}
	
}

/*
void can_int_vect()
{
	
}*/