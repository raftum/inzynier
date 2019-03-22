/*
 * adc.c
 *
 * Created: 2019-03-20 16:40:08
 *  Author: Rafal
 */ 


#include "../inc/adc.h" // sciezka zrodlowa z folderu

#define isDataReady (!(ADCSRA & (0 << ADSC)))

enum enum_channel
{
	ADC3 = 2,
	ADC2
};
enum enum_channel channel;

struct str_ADC_measure obADC_measure; 
struct str_ADC_measure *converter = &obADC_measure;

void ADC_init()
{
	converter->adc_switch = ADC2;
	
	//DDRC |= (1<<PC3) | (1<<PC2);
    ADCSRA |= (1<<ADEN);

	ADCSRA |= (1<<ADATE)|(1<<ADIF)|(1<<ADPS2)|(1<<ADPS0); // tryb ciaglej konwercji free run oraz ustawienie preskalera =32

	ADMUX |= (1<<REFS0) | (1<<ADLAR) | (converter->adc_switch);
	//ADCSRB &= ~( (1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0) );
	ADCSRB = (1 << ADTS2);
}

void ADC_start()  //str 191 start konwersji
{
	ADCSRA |= (1<<ADSC);
}

void ADC_select_channel()
{	
	//ADMUX = converter->adc_switch;	
	if(isDataReady)
	{		
		switch(converter->adc_switch)
		{
			case ADC2://gdy PC2
				converter->adc_ready_flag = 1;
				(converter->raw_voltage_input) = ADCH;     //odczytaj tylko starszy bajt pomiaru
				converter->adc_switch = ADC3;
				break;			
			case ADC3://gdy PC3
				converter->adc_ready_flag = 0;
				(converter->raw_voltage_output) = ADCH;     //odczytaj tylko starszy bajt pomiaru
				converter->adc_switch = ADC2;
				break;
		}
		ADMUX &= ~(0x03);  //kasowanie converter->adc_switch
	
		ADMUX  |= (converter->adc_switch); //Ustawianie nowych warto?ci
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