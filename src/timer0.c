/*
 * timer0.c
 *
 * Created: 2019-03-20 16:44:29
 *  Author: Rafal
 */ 
#include "../inc/timer0.h"

void TIMER0_init()
{
	TCCR0B|=(1<<CS01); //taktowanie 8 mln preskaler 8, czyli 1 mlm
	TIMSK0|=(1<<TOIE0);
	TCNT0 = 0;
	
};
