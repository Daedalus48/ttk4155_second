/*
 * communication.h
 *
 * Created: 23.10.2019 10:34:45
 *  Author: nelsonz
 */ 
#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_
#include <avr/io.h>

//void com_actualise_system();
void com_navigate_display();
void com_play_game();
void com_gain_menu();
void com_reset_score();
void com_set_score(int score_node2);
int com_get_score();
int com_get_back();
void com_set_back();
int com_get_start_game();
void com_set_start_game();
void com_play_game_with_gain_control();
int com_get_start_pid_control();
void com_reset_start_pid_control();

#endif