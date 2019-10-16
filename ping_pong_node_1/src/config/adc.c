/*
 * adc.c
 *
 * Created: 12.09.2019 10:16:31
 *  Author: nelsonz
 */ 
#include "adc.h"
#include "xmem.h"

#define JOY_DIR_ADDRESS 0x1800




void adc_init(){
	set_joy_adc_direction(NEUTRAL);
}

volatile int adc_read(uint8_t channel){
	if((channel <= 4 )&&( channel > 0)){
		volatile uint8_t *p = ADC_OFFSET;
		//p[0] = channel;
		*p = channel + 3;//+3
		_delay_us(160);
		//volatile uint8_t read = p[0];
		
		return *p;
		//return read;
	}
	else {
		return 0;
	}
}
/*
int adc_slider_adjust(int value){
	int ans = value * 200;
	ans = ans/250;
	ans = ans - 100;
	if (ans<-100){
		ans = -100;
	}else if (ans>100){
		ans = 100;
	}return ans;
}

int adc_joystick_adjust(int value){
	int ans = value * 200;
	
	ans = ans/250;
	
	ans = ans - 100;
	
	if (ans<-100){
		ans = -100;
	}else if (ans>100){
		ans = 100;
	}else if ((ans<20) && (ans>-20)){
		ans = 0;
	}
	return ans;
}*/

int adc_get_slider_pos(int *left_s, int *right_s){
	*left_s = adc_read(3);
	*right_s = adc_read(4);
	//*left_s = adc_slider_adjust(left_temp);
	//*right_s = adc_slider_adjust(right_temp);
	return 0;
}

int adc_get_joystick_pos(int *y, int *x){
	*y = adc_read(2);
	*x = adc_read(1);
	//*y = adc_joystick_adjust(y_temp);
	//*x = adc_joystick_adjust(x_temp);
	return 0;
}

int adc_joystick_angle(){	//still problematic
	int x = 0, y = 0;
	x = adc_read(1)-140;
	y = adc_read(2)-140;
	int ans = atan(x/y);
	if (abs(x)<30 && abs(y)<30){
		ans = 0;
	}
	return ans;
}
/*
int adc_joystick_direction(){
	int x = 0, y = 0;
	int ans = NEUTRAL;
	adc_get_joystick_pos(&y, &x);
	if (abs(x)<abs(y)){
		if (y<0){
			ans = DOWN;
		}else if(y>0){
			ans = UP;
		}
	}else if (abs(y)<abs(x)){
		if (x<0){
			ans = LEFT;
		}else if(x>0){
			ans = RIGHT;
		}
	}return ans;
}*/

int adc_joystick_direction(){
	int x = 0, y = 0;
	x = adc_read(1)-140;
	y = adc_read(2)-140;
	int ans = NEUTRAL;
	if (abs(x)<abs(y)){
		if (y<0){
			ans = DOWN;
		}else if(y>0){
			ans = UP;
		}
	}else if (abs(y)<abs(x)){
		if (x<0){
			ans = LEFT;
		}else if(x>0){
			ans = RIGHT;
		}
	}if (abs(x)<30 && abs(y)<30){
		ans = NEUTRAL;
	}
	return ans;
}

int adc_joy_pos_changed(){
	int ans = 0;
	int joy_adc_direction = get_joy_adc_direction();
	int previous_joy_adc_direction = joy_adc_direction;
	joy_adc_direction = adc_joystick_direction();

	
	if(previous_joy_adc_direction == joy_adc_direction){
		ans = 5;
	}
	else{
		ans = joy_adc_direction;
	}
	set_joy_adc_direction(joy_adc_direction);
	return ans;
}

int adc_joy_pos_changed_up_down(){
	/*int ans = 0;
	int previous_joy_adc_direction = joy_adc_direction;
	joy_adc_direction = adc_joystick_direction();
	/*printf("in adc joy pos changed \n \r");
	printf("previous joy adc %d \n \r", previous_joy_adc_direction);
	printf("joy adc direction %d \n \r", joy_adc_direction);
	
	if(previous_joy_adc_direction == joy_adc_direction || joy_adc_direction == LEFT || joy_adc_direction == RIGHT || joy_adc_direction == NEUTRAL){
		ans = 0;
	}
	else{
		ans = joy_adc_direction;
	}
	//printf("answer % d \n \r \n \r", ans);
	return ans;*/
}

int get_joy_adc_direction(){
	return xmem_read(JOY_DIR_ADDRESS);
}

void set_joy_adc_direction(int joy_adc_direction){
	xmem_write(joy_adc_direction, JOY_DIR_ADDRESS);
}

int adc_test_function(){
	int x = 0, y = 0, left_s = 0, right_s = 0;
	
	adc_get_joystick_pos(&y, &x);
	adc_get_slider_pos(&left_s, &right_s);
	
	printf("x =  %d \r\n", x);
	printf("y =  %d \r\n", y);
	printf("left slider =  %d \r\n", left_s);
	printf("right slider =  %d \r\n", right_s);
	printf("joystick angle =  %d \r\n", adc_joystick_angle());
	printf("joystick direction =  %d \r\n", adc_joystick_direction());
	return 0;
}