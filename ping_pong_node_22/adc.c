/*
 * adc.c
 *
 * Created: 24.10.2019 09:12:42
 *  Author: nelsonz
 */ 
#include "adc.h"
#include <avr/io.h>

void adc_init(){
	ADCSRA |= (1 << ADPS0 | 1 << ADPS1 | 1 << ADPS2);	
	ADCSRA |= (1 << ADEN);
}

int adc_read(){
	ADMUX |= (1 << REFS1);
	ADCSRA |= (1 << ADSC);
	
	while (ADCSRA & (1 << ADSC));
	
	if(ADC < THRESHOLD){
		return 0;
	}
	return 1;
}


