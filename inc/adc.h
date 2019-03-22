/*
 * adc.h
 *
 * Created: 2019-03-20 16:40:47
 *  Author: Rafal
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>
#include <stdint.h>

volatile struct str_ADC_measure //struktura ADC zmierzon
{
	uint16_t raw_voltage_input; // nap wejsc
	uint16_t raw_voltage_output; //nap wyjsc
	uint8_t  adc_switch;
	uint8_t  adc_ready_flag;
		
};
extern struct str_ADC_measure obADC_measure, *converter;


void ADC_init();
void ADC_start();
void ADC_select_channel();


#endif /* ADC_H_ */
