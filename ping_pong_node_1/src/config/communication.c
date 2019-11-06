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

int old_val = 1;
enum menu{main_menu, gain_menu, high_score_menu};
int current_menu = main_menu;
int score = 1;
int back = 0;

void com_actualise_system(){
	
	int pin = 1;
	int temp_value = NEUTRAL;
	int new_val;
	
	pin = (PINB & 0b0100)>>2;
	
	struct can_message message_servo;
	message_servo.id = 1;
	message_servo.length = 1;
	
	struct can_message message_motor;
	message_motor.id = 2;
	message_motor.length = 1;
	
	struct can_message message_solenoid;
	message_solenoid.id = 3;
	message_solenoid.length = 1;
	
	
	if(pin == 0){
		printf("string number %d selected \n \r", oled_get_joy_pos());
	}
	
	temp_value = adc_joy_pos_changed();
	
	if(temp_value == UP || temp_value == DOWN)
	{
		oled_actualise_joy_pos(temp_value, main_menu);
		//oled_display_activity();
		oled_navigate_gain_menu();
		_delay_ms(400);
	}
	
	message_servo.data[0] = adc_read(2);
	printf("servo send %d \n\r", message_servo.data[0]);
	can_message_send(&message_servo);
	
	message_motor.data[0] = adc_read(1);
	printf("motor send %d \n\r", message_motor.data[0]);
	can_message_send(&message_motor);
	
	new_val = (PINB & 0b0100)>>2;
	
	if( (old_val == 0) && (new_val == 1) )
	{
		old_val = 1;
		_delay_ms(50);
	}
	else if( (old_val == 1) && (new_val == 0) )
	{
		printf("string number %d selected \n \r", oled_get_joy_pos());
		message_solenoid.data[0] = 1;
		can_message_send(&message_solenoid);
		old_val = 0;
		_delay_ms(50);
	}
	

}

void com_navigate_display(){
	
}

void com_play_game(){
	int new_val;
	
	struct can_message message_servo;
	message_servo.id = 1;
	message_servo.length = 1;
	
	struct can_message message_motor;
	message_motor.id = 2;
	message_motor.length = 1;
	
	struct can_message message_solenoid;
	message_solenoid.id = 3;
	message_solenoid.length = 1;
	
	
	message_servo.data[0] = adc_read(2);
	printf("servo send %d \n\r", message_servo.data[0]);
	can_message_send(&message_servo);
	
	message_motor.data[0] = adc_read(1);
	printf("motor send %d \n\r", message_motor.data[0]);
	can_message_send(&message_motor);
	
	new_val = (PINB & 0b0100)>>2;
	
	if( (old_val == 0) && (new_val == 1) )
	{
		old_val = 1;
		_delay_ms(50);
	}
	else if( (old_val == 1) && (new_val == 0) )
	{
		message_solenoid.data[0] = 1;
		can_message_send(&message_solenoid);
		old_val = 0;
		_delay_ms(50);
	}
	
}

void com_gain_menu(){
	
}

void com_reset_score(){
	score = 0;
}
