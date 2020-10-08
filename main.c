/*
 * praca_inzynierska.c
 *
 * Created: 2019-03-19 17:44:41
 * Author : Rafal
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>
#include <util/delay.h>
#include "inc/HD44780.h"
#include "inc/pwm.h"
#include "inc/timer0.h"
#include "inc/adc.h"
#include "inc/boost_converter_algorithm.h"
#include "inc/button.h"
#include "inc/Usart.h"
//nowy projekt
#define USART_BAUDRATE 9600
#define Prescaler 20000000
#define BAUD_PRESCALE (((Prescaler/(USART_BAUDRATE * 16UL))) - 1)
volatile char bufor[5];
//volatile int timerCount = 0;

long map(long x, long in_min, long in_max, long out_min, long out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int main(void)
{
	message msg;
	msg.status = 0;
	uint16_t OCR1B_val = 0;
	
	USART_Init(BAUD_PRESCALE);
	LCD_Initalize();
	
    PWM_init();
	ADC_init();
    ADC_start();
	//BUTTON_Init();
    TIMER0_init();
	//PWM_select_mode(0);
	//PWM_ICR();
	//Timer0_stop();
sei(); // wlaczenie globalnych przerwan
	
	while (1) //P?tla g?ówna
	{
	   		msg = USART_MessageManager();
			   
	   		if (msg.status)
	   		{
		   		char temp1[1];
		   		char temp2[2];
		   		char temp3[3];	
				char liczba[4];
				   			
		   		OCR1B_val = 0;
		   		//LCD_GoTo(12,0);
		   		//LCD_Clear();
		   		switch (msg.len)
		   		{			   		
					case(2):

			   		//LCD_WriteData(msg.buffer[0]);
			   			temp1[0] = msg.buffer[0]; 
						//temp1[1] = '\0';
			   			_mode = msg.buffer[1]; // mode			   			
			   			OCR1B_val = (uint16_t)atoi(temp1);

			   		break;
			   		
			   		case(3):
			   		//LCD_WriteData(msg.buffer[0]);LCD_WriteData(msg.buffer[1]);
			   			temp2[0] = msg.buffer[0]; 
						temp2[1] = msg.buffer[1];
						//temp2[2] = '\0';
						_mode = msg.buffer[2]; //mode
						OCR1B_val = (uint16_t)atoi(temp2);	
			   	 
			   		break;
			   		
			   		case(4):
			   		//LCD_WriteData(msg.buffer[0]);LCD_WriteData(msg.buffer[1]);LCD_WriteData(msg.buffer[2]);
			   			temp3[0] = msg.buffer[0]; 
						temp3[1] = msg.buffer[1];
						temp3[2] = msg.buffer[2];
						//temp3[3] = '\0';
						_mode = msg.buffer[3]; //mode
						OCR1B_val = (uint16_t)atoi(temp3);
    
			   		break;
		   		}				   
				
				if(_mode == '1') //40khz
				{
					OCR1B_val = map(OCR1B_val, 0, 199, 0, 500);
				}
				if(_mode == '2') //10khz
				{
					OCR1B_val = map(OCR1B_val, 0, 199, 0, 2000);
				}
				
				sprintf(liczba, "%d", OCR1B_val);
				LCD_WriteText(liczba);
				
				PWM_select_mode(_mode);
		   		PWM_UpdateOCR1B(OCR1B_val/10);			  
				     		
		}
		  
	} 
}

ISR(TIMER0_OVF_vect)//przerwanie przepe?nienie timer0
{
	
	TCNT0 = 0;  //Pocz?tkowa warto?? licznika
}
ISR(ADC_vect)
{
	//LCD_GoTo(10,0);
	//LCD_WriteText("test");
	ADC_select_channel();
	 if(converter->adc_ready_flag == 1)
	 {
		 converter->adc_ready_flag = 0;
		//pwm_algorithm();
	 }
	
	ADC_start();
}
/*ISR(TIMER1_COMPA_vect)
{
	++timerCount;
}*/