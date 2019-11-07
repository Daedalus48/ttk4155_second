/*
 * oled.h
 *
 * Created: 18.09.2019 11:21:07
 *  Author: nelsonz
 */ 


#ifndef OLED_H_
#define OLED_H_

#define OLED_PAGES 8
#define OLED_COLUMBS 128

#define OLED_COLUMB_SET 0x21
#define OLED_PAGE_SET 0x22

#include <avr/io.h>
#include <stdio.h>
#include <string.h>

struct oled_activity 
{
	char oled_string[2][OLED_COLUMBS]; //precise column and rows in the [][]
	struct oled_activity *sub_activity[2];
}oled_activity;

void oled_write_c(uint8_t data);

void oled_write_data(uint8_t data);

int oled_init();

void oled_columb_range_select(uint8_t start, uint8_t end);

void oled_page_select(uint8_t page);

void oled_clear_page(uint8_t page);

void oled_clear_screen();

void oled_print_char_of_size(char letter, uint8_t size);

void oled_print_char_of_size_inverted(char letter, uint8_t size);

uint8_t oled_print_char(char letter);

uint8_t oled_print_char_inverted(char letter);

void oled_printf(char text[]);

void oled_printf_inverted(char text[]);

void oled_display_activity();

void oled_actualise_joy_pos(int joy_direction, int cur_menu);

int oled_get_joy_pos();

void oled_set_joy_pos(int joy_pos);

void oled_set_high_score(int score);

void oled_print_high_score();

void oled_navigate_gain_menu();

void oled_in_game_mode(int difficulty);

int oled_get_lives();

void oled_reduce_lives();

void oled_reset_lives();

void oled_actualise_high_score();

void oled_reset_hs();

void oled_game_over();

void oled_print_difficulty(int difficulty);

void oled_in_game_with_gain_control(int page, int kp, int ki, int kd);

#endif /* OLED_H_ */
