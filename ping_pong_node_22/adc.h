/*
 * adc.h
 *
 * Created: 24.10.2019 09:12:56
 *  Author: nelsonz
 */ 
#ifndef ADC_H
#define ADC_H

#define THRESHOLD 200

void adc_init(void);
int adc_read( void );

#endif /* ADC_H */