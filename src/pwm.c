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
#include <util/atomic.h>



void PWM_init() //konfiguracja fast PWM propozycja uzycia 16 bitowego Timera o rozdzieczlosci 8 bit. 0-255 ze sta?? cz?stotliwosci? 31,25 Khz, zmiana wypelnienia 0,39 %
{
	DDRB |= (1<<PB1);
	PORTB |= (1<<PB1);
	TCCR1A |= (1<<COM1A1); //| (1<<WGM10) ;// timer 16 bitowy
	//TCCR1B |= (1<<WGM12);
	TCCR1B |= (1<<CS10);  // preskaler 8000000 :1
	OCR1A = 0; //PWM 50%
	_mode = 0;
    value_0CR0B=0;
}
void PWM_ICR()
{  
	       /*ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	       {
		       CLKPR = (1<<CLKPCE);
		       CLKPR = (0<<CLKPCE) | (1<<CLKPS3)|(0<<CLKPS2)|(0<<CLKPS1)
		       |(0<<CLKPS0); //Prescaler division = 1
	       }*/

			 DDRD |= (1<<PD5);
			 PORTD |= (1<<PD5);
			 //oba rejestry por�wnuj� wzgledem takiego samego licznka i OCR0A od czestotl a OCROB czs trwania impulsu/
	         TCCR0A |= (1<<COM0A1)|(1 << COM0B1); // zerowal dla wypelnienie, 
	         //TIMSK0 = TIMSK0 | 1 << OCIE0A;
	         TCCR0A |= (1 << WGM00) | (1 << WGM01);
	         TCCR0B |= (1 << WGM02);
	         OCR0A = 199; // licznik, czestotliwosc ustala      255 - 78 KhZ
	        TCCR0B |= (1 << CS10); //preskaler
			 OCR0B = value_0CR0B; // licznik, wypelnienie ustala
			 /*DDRD = DDRD | 1 << DDD5;
	         TCCR0A = TCCR0A | 1 << COM0B1;
	         //TIMSK0 = TIMSK0 | 1 << OCIE0A;
	         TCCR0A = (TCCR0A | 1 << WGM00) | 1 << WGM01;
	         TCCR0B = TCCR0B | 1 << WGM02;
	         OCR0A = 199;
	        
	         TCCR0B |= (1 << CS10);
			 OCR0B = 1;*/
}

void Timer0_stop()
{
	
	TCCR0B &=~((1<<CS10)|(1<<CS11)|(1<<CS12));//wylaczenie timera
	TCNT0 = 0;
}
void PWM_select_mode(uint8_t selector)
{
	//PWM_init();
	_mode = selector;
	//selector = 0;
	switch(selector)
	{
		case MODE_8BIT:
		//TCCR0B |= (1 << CS10);
		OCR0A = 199;
		break;
		
		case MODE_9BIT:
		//TCCR0B |= (1 << CS10);
		OCR0A = 255;
		break;
		
		case MODE_10BIT:
		//TCCR0B |= (1 << CS10);
		OCR0A = 255;
		break;
		default:
		;//do nothing
		break;
	}
}