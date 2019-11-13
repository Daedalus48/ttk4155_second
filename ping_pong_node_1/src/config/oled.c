/*
 * oled.c
 *
 * Created: 18.09.2019 11:21:32
 *  Author: nelsonz
 */ 

#define OLEDC_OFFSET 0x1000
#define OLEDD_OFFSET 0x1200

#define JOY_POS_ADDRESS 0x1802
#define HS_POS_ADDRESS 0x1804

#include <avr/pgmspace.h>
#include "oled.h"
#include "fonts.h"
#include "xmem.h"
#include "communication.h"

enum oled_font_size{FONT_SIZE_LARGE, FONT_SIZE_MEDIUM, FONT_SIZE_SMALL};
enum adc_joystick_dir{LEFT, RIGHT, UP, DOWN, NEUTRAL};
enum menu{main_menu, gain_menu, high_score_menu};
	
//struct oled_activity may_spring = {"Birds \n", "Sun rays \n", NULL, NULL};
//struct oled_activity july_summer = {"Beach \n", "Sun burnt \n", NULL, NULL}; 

//struct oled_activity oled_main = {"Spring \n", "Summer \n", NULL, NULL};
	
//struct oled_activity *current_activity = NULL;

int joy_pos;
int high_score = 0;
int lives = 3;


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
	high_score = xmem_read(HS_POS_ADDRESS);
	oled_display_activity();
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
	for (int c = 0; c<lenght; c++){
		oled_print_char(text[c]);
	}
}

void oled_printf_inverted(char text[]){
	int lenght = strlen(text);
	for (int c = 0; c<lenght; c++){
		oled_print_char_inverted(text[c]);
	}
}

void oled_display_activity(){
	oled_clear_screen();
	int joy_pos = oled_get_joy_pos();
	
	oled_page_select(1);
	if(joy_pos == 0){
		oled_printf_inverted("Play Game \n");
	}else{
		oled_printf("Play Game \n");
	}
	oled_page_select(2);
	if(joy_pos == 1){
		oled_printf_inverted("Set Gain \n");
	}else{
		oled_printf("Set Gain \n");
	}
	oled_page_select(3);
	if(joy_pos == 2){
		oled_printf_inverted("High Score \n");
	}else{
		oled_printf("High Score \n");
	}
	oled_page_select(4);
	if(joy_pos == 3){
		oled_printf_inverted("PID Control \n");
		}else{
		oled_printf("Pid Control \n");
	}
}

void oled_actualise_joy_pos(int joy_direction, int cur_menu){
	int number_of_pages = 2;
	if (cur_menu == main_menu){
		number_of_pages = 3;
	}else if (cur_menu == gain_menu){
		number_of_pages = 2;
	}else if (cur_menu == high_score_menu){
		number_of_pages = 1;
	}
	
	
	int joy_pos = oled_get_joy_pos();	
	if(joy_direction == UP)
		if (joy_pos==0){
			joy_pos=number_of_pages;
		}else{
			joy_pos = (joy_pos - 1);
		}
	else if(joy_direction == DOWN)
		if (joy_pos==number_of_pages){
			joy_pos=0;
		}else{
			joy_pos = (joy_pos + 1)%(number_of_pages+1);
		}
	oled_set_joy_pos(joy_pos);
	printf("pos: %d\n\r",  joy_pos);

}

int oled_get_joy_pos(){
	return xmem_read(JOY_POS_ADDRESS);
}

void oled_set_joy_pos(int joy_pos){
	xmem_write(joy_pos, JOY_POS_ADDRESS);
}

void oled_set_high_score(int score){
	high_score = score;
}

void oled_print_high_score(){
	oled_clear_screen();
	oled_page_select(1);
	oled_printf("High score :\n");
	
	oled_page_select(2);
	char high_score_char [4];
	sprintf(high_score_char, "%d", high_score);
	oled_printf(high_score_char);
	
	oled_page_select(3);
	oled_printf_inverted("Reset");
}

void oled_navigate_gain_menu(){
	oled_clear_screen();
	int joy_pos = oled_get_joy_pos();
	
	oled_page_select(1);
	if(joy_pos == 0){
		oled_printf_inverted("Easy \n");
	}else{
		oled_printf("Easy \n");
	}
	oled_page_select(2);
	if(joy_pos == 1){
		oled_printf_inverted("Medium \n");
	}else{
		oled_printf("Medium \n");
	}
	oled_page_select(3);
	if(joy_pos == 2){
		oled_printf_inverted("Hard \n");
	}else{
	oled_printf("Hard \n");
	}
}

void oled_in_game_mode(int difficulty){
	oled_clear_screen();
	oled_page_select(1);
	oled_printf("GAME ACTIVE \n");
	oled_page_select(2);
	oled_printf("lives left: ");
	char lives_left [1];
	sprintf(lives_left, "%d", lives);
	oled_printf(lives_left);
	oled_printf("\n");
	oled_page_select(3);
	oled_printf("score: ");
	char scr [4];
	sprintf(scr, "%d", com_get_score());
	oled_printf(scr);
	oled_printf("\n");
	oled_page_select(4);
	oled_printf("high score: ");
	char hs [4];
	sprintf(hs, "%d", high_score);
	oled_printf(hs);
	oled_printf("\n");
	oled_page_select(5);
	if (difficulty == 0){
		oled_printf("easy \n");
	}else if (difficulty == 1){
		oled_printf("medium \n");
	}else{
		oled_printf("hard \n");
	}
}

int oled_get_lives(){
	return lives;
}

void oled_reduce_lives(){
	lives--;
}

void oled_reset_lives(){
	lives = 3;
}

void oled_actualise_high_score(){
	int score = com_get_score();
	if(score > high_score){
		high_score = score;
	}
	xmem_write(high_score, HS_POS_ADDRESS);
}

void oled_reset_hs(){
	high_score = 0;
	xmem_write(high_score, HS_POS_ADDRESS);
}

void oled_game_over(){
	oled_clear_screen();
	oled_page_select(1);
	oled_printf("GAME OVER... \n");
}

void oled_print_difficulty(int difficulty){
	oled_clear_screen();
	switch(difficulty){
		case 0:
			oled_page_select(1);
			oled_printf("EASY selected \n");
			oled_page_select(2);
			oled_printf("Kp = 1.2\n");
			oled_page_select(3);
			oled_printf("Ki = 0.8\n");
			oled_page_select(4);
			oled_printf("Kd = 0.2");
			break;
		case 1:
			oled_page_select(1);
			oled_printf("MEDIUM selected \n");
			oled_page_select(2);
			oled_printf("Kp = 0.6\n");
			oled_page_select(3);
			oled_printf("Ki = 0.8\n");
			oled_page_select(4);
			oled_printf("Kd = 0.2");
			break;
		case 2:
			oled_page_select(1);
			oled_printf("HARD selected \n");
			oled_page_select(2);
			oled_printf("Kp = 2.0\n");
			oled_page_select(3);
			oled_printf("Ki = 0.0\n");
			oled_page_select(4);
			oled_printf("Kd = 0.0");
			break;
	}
}

void oled_in_game_with_gain_control(int page, int kp, int ki, int kd){
	oled_clear_screen();
	oled_page_select(1);
	oled_printf_inverted("PID CONTROL\n");
	
	char str[4];
	
	oled_page_select(2);
	if(page == 2){
		oled_printf_inverted("Kp = ");
	}else{
		oled_printf("Kp = ");
	}sprintf(str, "%d", kp/100);
	oled_printf(str);
	oled_printf(".");
	sprintf(str, "%d", kp%100);
	oled_printf(str);
	oled_printf("\n");
	oled_page_select(3);
	if(page == 3){
		oled_printf_inverted("Ki = ");
	}else{
		oled_printf("Ki = ");
	}sprintf(str, "%d", ki/100);
	oled_printf(str);
	oled_printf(".");
	sprintf(str, "%d", ki%100);
	oled_printf(str);
	oled_printf("\n");
	oled_page_select(4);
	if(page == 4){
		oled_printf_inverted("Kd = ");
	}else{
		oled_printf("Kd = ");
	}sprintf(str, "%d", kd/100);
	oled_printf(str);
	oled_printf(".");
	sprintf(str, "%d", kd%100);
	oled_printf(str);
	oled_printf("\n");
}
