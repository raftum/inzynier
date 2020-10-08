
#include <util/delay.h>
#include "../inc/pwm.h"

#define MODE_100KHZ 0
#define MODE_40KHZ 1
#define MODE_1KHZ 2
#include <util/atomic.h>



void PWM_init() //konfiguracja fast PWM propozycja uzycia 16 bitowego Timera o rozdzieczlosci 8 bit. 0-255 ze sta?? cz?stotliwosci? 31,25 Khz, zmiana wypelnienia 0,39 %
{
	DDRB |= (1<<PB2);
	PORTB |= (1<<PB2);
	TCCR1A |= (1<<COM1B1) | (1<<COM1B0) |(1<<WGM10) |  (1<<WGM11) ;// timer 16 bitowy Fast PWM, 8-bit
	TCCR1B |= (1<<WGM12)|(1<<WGM13);
	TCCR1B |= (1<<CS10);  // preskaler 20000000 :1
	OCR1A = 199; // czestotliwosc
	OCR1B = 1; // wypelnienie
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
	//oba rejestry porównuj¹ wzgledem takiego samego licznka i OCR0A od czestotl a OCROB czs trwania impulsu/
	TCCR0A |= (1<<COM0A1)|(1 << COM0B1); // zerowal dla wypelnienie, 
	//TIMSK0 = TIMSK0 | 1 << OCIE0A;
	TCCR0A |= (1 << WGM00) | (1 << WGM01);
	TCCR0B |= (1 << WGM02);
	OCR0A = 199; // licznik, czestotliwosc ustala      255 - 78 KhZ
	TCCR0B |= (1 << CS10); //preskaler
	OCR0B = 1; // licznik, wypelnienie ustala
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
	PWM_init();
	int m = selector-48;
	//selector = 0;
	switch(m)
	{
		case MODE_100KHZ: //100khz
			PWM_UpdateOCR1A(199);
		break;
		
		case MODE_40KHZ: //40khz
			PWM_UpdateOCR1A(500);
		break;
		
		case MODE_1KHZ: //10khz
			PWM_UpdateOCR1A(2000);
		break;
		default:
			;//do nothing
		break;
	}
}

void PWM_UpdateOCR0B(uint8_t value)
{
	OCR0B = value;
}

void PWM_UpdateOCR1B(uint16_t value)
{
	cli();

	OCR1B = value;
		
	sei();
}

void PWM_UpdateOCR1A(uint16_t value)
{
	cli();
	OCR1A = value;
	sei();
}