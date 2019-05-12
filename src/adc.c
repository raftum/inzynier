/*
 * adc.c
 *
 * Created: 2019-03-20 16:40:08
 *  Author: Rafal
 */ 


#include "../inc/adc.h" // sciezka zrodlowa z folderu

#define isDataReady (!(ADCSRA & (0 << ADSC)))
#define SAMPLES 4

enum enum_channel
{
	ADC2 = 2,
	ADC3
};
enum enum_channel channel;

struct str_ADC_measure obADC_measure; 
struct str_ADC_measure *converter = &obADC_measure;

void ADC_init()
{
	converter->adc_switch = ADC2;
	
	//DDRC |= (1<<PC3) | (1<<PC2);
    ADCSRA |= (1<<ADEN); //ustawienie tego bitu na 1 w?cza przetwornik ADC mikrokontrolera.

	ADCSRA |=/* (1<<ADATE)|*/(1<<ADIF)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1); // tryb ciaglej konwercji free run oraz ustawienie preskalera =32

	ADMUX |= (1<<REFS0) | (1<<ADLAR) | (converter->adc_switch);
	//ADCSRB &= ~( (1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0) );
	//ADCSRB = (1 << ADTS2);
	converter->raw_voltage_input = 0;
	converter->raw_voltage_output = 0;
}

void ADC_start()  //str 191 start konwersji
{
	ADCSRA |= (1<<ADSC);
}

void ADC_select_channel()
{
	static uint8_t counter = 0;
	static uint16_t sampleADC2 = 0;
	static uint16_t sampleADC3 = 0;
	
	if(counter < SAMPLES)
	{	
		switch(converter->adc_switch)
		{
			case ADC2://gdy PC2
				//converter->adc_ready_flag = 0;
				sampleADC2 += ADCH;     //odczytaj tylko starszy bajt pomiaru
				converter->adc_switch = ADC3;
				break;			
			case ADC3://gdy PC3
				//converter->adc_ready_flag = 0;
				sampleADC3 += ADCH;     //odczytaj tylko starszy bajt pomiaru
				converter->adc_switch = ADC2;
				counter++;
				break;
		}
		ADMUX &= ~(0x03);  //kasowanie converter->adc_switch

		ADMUX  |= (converter->adc_switch); //Ustawianie nowych warto?ci
	}
	else
	{
		converter->raw_voltage_input = sampleADC2 / SAMPLES;
		converter->raw_voltage_output = sampleADC3 / SAMPLES;
		counter = 0;
		sampleADC2 = 0;
		sampleADC3 = 0;
		converter->adc_ready_flag = 1;
	}
		
	// if((converter->adc_switch) < 3)
	// {
		// (converter->adc_switch)++;
	// }		
	// else
	// {
		// (converter->adc_switch) = ADC2;
	// }
}