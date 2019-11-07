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
int score = 0;
int back = 0;
int start_game = 0;

/*
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
		oled_display_activity();
		//oled_navigate_gain_menu();
		_delay_ms(400);
	}
	
	message_servo.data[0] = adc_read(2);
	//printf("servo send %d \n\r", message_servo.data[0]);
	can_message_send(&message_servo);
	
	message_motor.data[0] = adc_read(1);
	//printf("motor send %d \n\r", message_motor.data[0]);
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
	

}*/

void com_navigate_display(){
	int pin = 1;
	int temp_value = NEUTRAL;
	int new_val;
	
	pin = (PINB & 0b0100)>>2;	
	
	
	temp_value = adc_joy_pos_changed();
	
	if(temp_value == UP || temp_value == DOWN)
	{
		oled_actualise_joy_pos(temp_value, current_menu);
		switch (current_menu){
			case main_menu:
				oled_display_activity();
				break;
			case gain_menu:
				oled_navigate_gain_menu();
				break;
			case high_score_menu:
				oled_print_high_score();
				break;
		}
		_delay_ms(400);
	}
	
	new_val = (PINB & 0b0100)>>2;
	
	if( (old_val == 0) && (new_val == 1) )
	{
		old_val = 1;
		_delay_ms(50);
	}
	else if( (old_val == 1) && (new_val == 0) )
	{
		old_val = 0;
		_delay_ms(50);
		if ((current_menu == main_menu)&&(!oled_get_joy_pos())){ start_game = 1;}
		else if ((current_menu == main_menu)&&(oled_get_joy_pos() == 1)){
			current_menu = gain_menu;
			oled_navigate_gain_menu();
		}else if ((current_menu == main_menu)&&(oled_get_joy_pos() == 2)){
			current_menu = high_score_menu;
			oled_print_high_score();
		}else if (current_menu == high_score_menu){
			oled_reset_hs();
			oled_print_high_score();
		}else if (current_menu == gain_menu){
			int joy_pos = oled_get_joy_pos();
			
			struct can_message message_gain;
			message_gain.id = 4;
			message_gain.length = 1;
			message_gain.data[0] = 0;
			
			switch (joy_pos){
				case 0:
				oled_print_difficulty(0);
				message_gain.data[0] = 0;
				break;
				case 1:
				oled_print_difficulty(1);
				message_gain.data[0] = 1;
				break;
				case 2:
				oled_print_difficulty(2);
				message_gain.data[0] = 2;
				break;
				default:
				break;
			}
			
			can_message_send(&message_gain);
			_delay_ms(5000);
			oled_navigate_gain_menu();
		}
	}
	
	if ((PINB & 0b0010)>>1){
		if ((current_menu == main_menu)&&(!oled_get_joy_pos())){ start_game = 1;}
		else if ((current_menu == main_menu)&&(oled_get_joy_pos() == 1)){
			current_menu = gain_menu;
			oled_navigate_gain_menu();
		}else if ((current_menu == main_menu)&&(oled_get_joy_pos() == 2)){
			current_menu = high_score_menu;
			oled_print_high_score();
		}else if(current_menu == high_score_menu){
			oled_reset_hs();
			oled_print_high_score();
		}else if (current_menu == gain_menu){
			int joy_pos = oled_get_joy_pos();
			
			struct can_message message_gain;
			message_gain.id = 4;
			message_gain.length = 1;
			message_gain.data[0] = 0;
			
			switch (joy_pos){
				case 0:
					oled_print_difficulty(0);
					message_gain.data[0] = 0;
					break;
				case 1:
					oled_print_difficulty(1);
					message_gain.data[0] = 1;
					break;
				case 2:
					oled_print_difficulty(2);
					message_gain.data[0] = 2;
					break;
				default:
					break;
			}
			
			can_message_send(&message_gain);
			_delay_ms(5000);
			oled_navigate_gain_menu();
		}
	}
	
	
	if ((PINB & 0b0001)&&((current_menu == gain_menu ) || (current_menu == high_score_menu))){
		current_menu = main_menu;
		oled_display_activity();
	}
	
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
	
	
	message_servo.data[0] = adc_read(1);
	can_message_send(&message_servo);
	
	message_motor.data[0] = adc_read(4);
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
	oled_in_game_mode();
	if ((oled_get_lives()==0)||(PINB & 0b0001)){
		back = 1;
		oled_game_over();
		_delay_ms(3000);
		oled_display_activity();
		oled_reset_lives();
		com_reset_score();
	}
}

void com_gain_menu(){
	
}

void com_reset_score(){
	score = 0;
}

int com_get_score(){
	return score;
}

void com_set_score(int score_node2){
	score = score_node2;
}

int com_get_back(){
	return back;
}

void com_set_back(){
	back = 0;
}

int com_get_start_game(){
	return start_game;
}

void com_set_start_game(){
	start_game = 0;
}