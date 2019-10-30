/*
 * communication.c
 *
 * Created: 23.10.2019 10:34:31
 *  Author: nelsonz
 */ 
#include "communication.h"
#include "adc.h"
#include "oled.h"
#include "can_controller.h"

void com_actualise_system(){
	
	int pin = 1;
	int temp_value = NEUTRAL;
	
	pin = (PINB & 0b0100)>>2;
	
	struct can_message message;
	message.id = 3;
	message.length = 1;
	message.data[0] = (uint8_t) 'c';
	
	if(pin == 0){
		printf("string number %d selected \n \r", oled_get_joy_pos());
	}
	
	temp_value = adc_joy_pos_changed();
	
	if(temp_value == UP || temp_value == DOWN)
	{
		oled_actualise_joy_pos(temp_value);
		oled_display_activity();
		_delay_ms(400);
	}
	
	message.data[0] = adc_read(4);
	printf("send message %d \n\r", message.data[0]);
	can_message_send(&message);
		

}

