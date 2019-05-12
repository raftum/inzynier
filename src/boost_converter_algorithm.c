/*
 * boost_converter_algorithm.c
 *
 * Created: 2019-03-21 21:26:38
 *  Author: Rafal
 */ 
#include "../inc/boost_converter_algorithm.h"
#include "../inc/adc.h"
#include "../inc/pwm.h"
uint8_t max_dute = 50;
void pwm_algorithm ()
{
	static uint8_t blok = 0;
	//static uint8_t max_dute = 0;
	if((converter->raw_voltage_output) <=(converter->raw_voltage_input) )//512 //wart zad napiecie procesora
	{
		
		OCR0B++;
		if(OCR0B > max_dute) OCR0B= max_dute;      //ograniczenie wype³nienia zalozyl jaka wartosc nie chce przekroczyc, np 220
		blok = 0;
	}
	else
	{
		if(blok == 0) OCR0B --;
		
		if(OCR0B < 1) blok = 1;
		
	}
};
void pwm_select_algorithm()
{
	switch(_mode)
	{
		case 0:
		max_dute=40;
		break;
		case 1:
		max_dute = 30 ;
		break;
		case 2:
		max_dute = 10;
		break;
	}
	pwm_algorithm ();
}
