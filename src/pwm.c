/*
 * pwm.c
 *
 * Created: 2019-03-19 22:24:22
 *  Author: Rafal
 */ 
#include <util/delay.h>
#include "../inc/pwm.h"

#define MAX_8BIT 255
#define MAX_9BIT 511
#define MAX_10BIT 1023

#define MODE_8BIT 0
#define MODE_9BIT 1
#define MODE_10BIT 2



void PWM_init() //konfiguracja fast PWM propozycja uzycia 16 bitowego Timera o rozdzieczlosci 8 bit. 0-255 ze sta³¹ czêstotliwosci¹ 31,25 Khz, zmiana wypelnienia 0,39 %
{
	DDRB |= (1<<PB1);
	PORTB |= (1<<PB1);
	TCCR1A |= (1<<COM1A1); //| (1<<WGM10) ;// timer 16 bitowy
	//TCCR1B |= (1<<WGM12);
	TCCR1B |= (1<<CS10);  // preskaler 8000000 :1
	OCR1A = 0; //PWM 50%
	_mode = 0;
}
void PWM_ICR()
{
	 	 DDRB |= (1<<PB1);
	 	 PORTB |= (1<<PB1);
        TCCR1A |= (1<<COM1A1);
		TCCR1B |= (1<<CS10); 
		ICR1 |= 200;   //z 65535
		OCR1A = 1;
		
}
void Timer0_stop()
{
	TCCR1B &= ~((1<<CS10)|(1<<CS11)|(1<<CS12));//wylaczenie timera
	TCNT1 = 0;
}
void PWM_select_mode(uint8_t selector)
{
	//PWM_init();
	_mode = selector;
	//selector = 0;
	switch(selector)
	{
		case MODE_8BIT:
		TCCR1A &= ~(1<<WGM11);
		TCCR1A |= (1<<WGM10);
		TCCR1B |= (1<<WGM12);
		OCR1A=20; //fast o rozdz 8 bit
		break;
		case MODE_9BIT:
		TCCR1A &=~(1<<WGM10);
		TCCR1A |= (1<<WGM11);
		TCCR1B |= (1<<WGM12); //fast 9 bit
		OCR1A = 20;
		break;
		case MODE_10BIT:
		TCCR1A |= (1<<WGM10)| (1<<WGM11);
		TCCR1B |= (1<<WGM12); // fast 10 bit
		OCR1A = 10;
		break;
		default:
		;//do nothing
		break;
	}
}