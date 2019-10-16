/*
 * oled.c
 *
 * Created: 18.09.2019 11:21:32
 *  Author: nelsonz
 */ 

#define OLEDC_OFFSET 0x1000
#define OLEDD_OFFSET 0x1200

#define JOY_POS_ADDRESS 0x1802

#include <avr/pgmspace.h>
#include "oled.h"
#include "fonts.h"
#include "xmem.h"

enum oled_font_size{FONT_SIZE_LARGE, FONT_SIZE_MEDIUM, FONT_SIZE_SMALL};
enum adc_joystick_dir{LEFT, RIGHT, UP, DOWN, NEUTRAL};
	
//struct oled_activity may_spring = {"Birds \n", "Sun rays \n", NULL, NULL};
//struct oled_activity july_summer = {"Beach \n", "Sun burnt \n", NULL, NULL}; 

//struct oled_activity oled_main = {"Spring \n", "Summer \n", NULL, NULL};
	
//struct oled_activity *current_activity = NULL;

int joy_pos;


void oled_write_c(uint8_t data) {
	volatile uint8_t *adress = OLEDC_OFFSET;
	*adress = data;
}

void oled_write_data(uint8_t data) {
	volatile uint8_t *adress = OLEDD_OFFSET;
	*adress = data;
}

int oled_init(){
	oled_write_c(0xae);	//display off
	oled_write_c(0xa1);	//segment remap
	oled_write_c(0xda);	//common pads hardware: alternative
	oled_write_c(0x12);
	oled_write_c(0xc8);	//common output scan direction:com63
	oled_write_c(0xa8);	//multiplex ration mode:63
	oled_write_c(0x3f);
	oled_write_c(0xd5);	//display divide ratio/osc. freq. mode
	oled_write_c(0x80);
	oled_write_c(0x81);	//contrast control
	oled_write_c(0x50);
	oled_write_c(0xd9);	//set pre-change period
	oled_write_c(0x21);
	oled_write_c(0x20);	//set memory addressing mode
	oled_write_c(0x02);
	oled_write_c(0xdb);	//vcom deselect level mode
	oled_write_c(0x30);
	oled_write_c(0xad);	//master configuration
	oled_write_c(0x00);
	oled_write_c(0xa4);	//out follows RAM content
	oled_write_c(0xa6);	//set normal display
	oled_write_c(0xaf);	//display on
	oled_clear_screen();
	//current_activity = &oled_main;
	joy_pos = 0;
	//oled_display_activity();
}

void oled_columb_range_select(uint8_t start, uint8_t end) {
	oled_write_c(OLED_COLUMB_SET);
	if (start < OLED_COLUMBS) {
		oled_write_c(start);
		} else {
		oled_write_c(OLED_COLUMBS - 1);
	}

	if (end < OLED_COLUMBS) {
		oled_write_c(end);
		} else {
		oled_write_c(OLED_COLUMBS - 1);
	}
}

void oled_page_select(uint8_t page) {
	oled_write_c(OLED_PAGE_SET);
	if (page < OLED_PAGES) {
		oled_write_c(page);
		oled_write_c(page);
		} else {
		oled_write_c(OLED_PAGES - 1);
		oled_write_c(OLED_PAGES - 1);
	}
}

void oled_clear_page(uint8_t page) {
	if (page < OLED_PAGES) {
		oled_page_select(page);
		oled_columb_range_select(0, OLED_COLUMBS - 1);
		for (int i = 0; i < OLED_COLUMBS; i++) {
			oled_write_data(0x00);
		}
	}
}

void oled_clear_screen() {
	for (int i = 0; i < OLED_PAGES; i++) {
		oled_clear_page(i);
	}
}

void oled_print_char_of_size(char letter, uint8_t size) {
	uint8_t char_length = 0;
	unsigned char *letter_bitmap;
	if (size == FONT_SIZE_SMALL) {
		char_length = 4;
		letter_bitmap = font4[letter - ' '];
		} else if (size == FONT_SIZE_MEDIUM) {
		char_length = 5;
		letter_bitmap = font5[letter - ' '];
		} else {
		char_length = 8;
		letter_bitmap = font8[letter - ' '];
	}
	for (int i = 0; i < char_length; i++) {
      oled_write_data(pgm_read_byte(&(letter_bitmap[i])));
  }
}

void oled_print_char_of_size_inverted(char letter, uint8_t size) {
	uint8_t char_length = 0;
	unsigned char *letter_bitmap;
	if (size == FONT_SIZE_SMALL) {
		char_length = 4;
		letter_bitmap = font4[letter - ' '];
		} else if (size == FONT_SIZE_MEDIUM) {
		char_length = 5;
		letter_bitmap = font5[letter - ' '];
		} else {
		char_length = 8;
		letter_bitmap = font8[letter - ' '];
	}
	for (int i = 0; i < char_length; i++) {
      oled_write_data(~pgm_read_byte(&(letter_bitmap[i])));
  }
}

uint8_t printf_page = 0;
uint8_t printf_size = 0;

uint8_t oled_print_char(char letter) {
	if (letter == '\n') {
		printf_page += 1;
		oled_page_select(printf_page);
		oled_columb_range_select(0, OLED_COLUMBS);
		} else {
		oled_print_char_of_size(letter, printf_size);
		uint8_t columb_adder = 0;
	}
	return 0;
}

uint8_t oled_print_char_inverted(char letter) {
	if (letter == '\n') {
		printf_page += 1;
		oled_page_select(printf_page);
		oled_columb_range_select(0, OLED_COLUMBS);
	} else {
		oled_print_char_of_size_inverted(letter, printf_size);
		uint8_t columb_adder = 0;
	}
	return 0;
}

void oled_printf(char text[]){
	int lenght = strlen(text);
	//printf("size =  %d \r\n", lenght);
	for (int c = 0; c<lenght; c++){
		//printf("letter =  %d \r\n", c);
		oled_print_char(text[c]);
	}
}

void oled_printf_inverted(char text[]){
	int lenght = strlen(text);
	//printf("size =  %d \r\n", lenght);
	for (int c = 0; c<lenght; c++){
		//printf("letter =  %d \r\n", c);
		oled_print_char_inverted(text[c]);
	}
}

void oled_display_activity(){
	oled_clear_screen();
	int joy_pos = oled_get_joy_pos();
	
	/*for(int i = 0; i < 2; i++)
	{
		oled_page_select(i+1);
		if(joy_pos == i)
			oled_printf_inverted(current_activity->oled_string[i]);
		else
			oled_printf(current_activity->oled_string[i]);
	}*/
	
	oled_page_select(1);
	if(joy_pos == 0)
		oled_printf_inverted("Spring \n");
	else
		oled_printf("Spring \n");
	
	oled_page_select(2);
	if(joy_pos == 1)
		oled_printf_inverted("Summer \n");
	else
		oled_printf("Summer \n");
	
	oled_page_select(3);
	if(joy_pos == 2)
		oled_printf_inverted("Return \n");
	else
		oled_printf("Return \n");
}

void oled_actualise_joy_pos(int joy_direction)
{
	int joy_pos = oled_get_joy_pos();	
	if(joy_direction == UP)
		if (joy_pos==0){
			joy_pos=2;
		}else{
			joy_pos = (joy_pos - 1);
		}
	else if(joy_direction == DOWN)
		if (joy_pos==2){
			joy_pos=0;
		}else{
			joy_pos = (joy_pos + 1)%3;
		}
	oled_set_joy_pos(joy_pos);

}

int oled_get_joy_pos(){
	return xmem_read(JOY_POS_ADDRESS);
}

void oled_set_joy_pos(int joy_pos){
	xmem_write(joy_pos, JOY_POS_ADDRESS);
}