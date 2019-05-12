/*
 * button.c
 *
 * Created: 2019-04-10 11:45:04
 *  Author: Rafal
 */ 
#include <util/delay.h>
#include "../inc/pwm.h"
#include "../inc/button.h"

void BUTTON_Init()
{
	DDRC &= ~ ((SWITCH) | (SWITCH2));
	PORTC |= ((SWITCH) | (SWITCH2)); //rezystor wewnetrzny
}

void BUTTON_check_pressed()
{
	static int8_t selector = 0;
	
	if(SWITCH_ON)
	{
		_delay_ms(200);
		
		selector++;
		if (selector>2)
		{
			selector=2;
		}
		while(SWITCH_ON){}
		PWM_select_mode(selector);
		
	}
	else if(SWITCH_ON2)
	{
		_delay_ms(200);
		
		selector--;
		if (selector<0)
		{
			selector=0;
		}
		while(SWITCH_ON2){}
		PWM_select_mode(selector);
	}
}