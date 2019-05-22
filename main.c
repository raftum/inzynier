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
int main(void)
{
	message msg;
	msg.status = 0;
	uint8_t OCR0B_val = 0;
	
	USART_Init(BAUD_PRESCALE);
	LCD_Initalize();
    //PWM_init() ;
	ADC_init();
    ADC_start();
	BUTTON_Init();
    //TIMER0_init();
	//PWM_select_mode(0);
	PWM_ICR();
	//Timer0_stop();
sei(); // wlaczenie globalnych przerwan
	
	uint16_t oblicz=0;
	
	
	/*while (1)
	{
		msg = USART_MessageManager();
		if (msg.status)
		{
			LCD_GoTo(12,0);
			switch (msg.len)
			{
				case(1):
			    LCD_WriteText(msg.buffer);
				break;
				case(2):
				 LCD_WriteText(msg.buffer);
				break;
				case (3):
				 LCD_WriteText(msg.buffer);
				break;
			}
		}
	}*/
	while (1) //P?tla g?ówna
	{
	   		msg = USART_MessageManager();
	   		
	   		if (msg.status)
	   		{
		   		char temp1[2];
		   		char temp2[3];
		   		char temp3[4];
		   		
		   		LCD_GoTo(12,0);
		   		//LCD_Clear();
		   		switch (msg.len)
		   		{
			   		case(1):
			   		LCD_WriteData(msg.buffer[0]);
			   		temp1[0] = msg.buffer[0]; temp1[1] = 'X';
			   		OCR0B_val = (uint8_t)atoi(temp1);
			   		break;
			   		
			   		case(2):
			   		LCD_WriteData(msg.buffer[0]);LCD_WriteData(msg.buffer[1]);
			   		temp2[0] = msg.buffer[0]; temp2[1] = msg.buffer[1]; temp2[2] = 'X';
			   		OCR0B_val = (uint8_t)atoi(temp2);
			   		break;
			   		
			   		case (3):
			   		LCD_WriteData(msg.buffer[0]);LCD_WriteData(msg.buffer[1]);LCD_WriteData(msg.buffer[2]);
			   		temp3[0] = msg.buffer[0]; temp3[1] = msg.buffer[1]; temp3[2] = msg.buffer[2]; temp3[3] = 'X';
			   		OCR0B_val = (uint8_t)atoi(temp3);
			   		break;
		   		}
		   		PWM_UpdateOCR0B(OCR0B_val);
	   		
		}
       BUTTON_check_pressed(); //sprawdz przycisk
		LCD_GoTo(0,0);
        itoa(converter->raw_voltage_input,bufor,10);
		LCD_WriteText(bufor);
	    LCD_WriteText("  ");
        LCD_GoTo(0,1);
		oblicz = (converter->raw_voltage_input)*250; // 768 * 250     
		oblicz = oblicz/127.5; //369   // w przypadku timer0 to bedzie polowa z 255 czyli 127.5
		itoa(oblicz/100,bufor,10);      //konwersja wyniku do tablicy char
		LCD_WriteText(bufor);//3
		LCD_WriteText(",");//
		itoa(oblicz%100,bufor,10);
		LCD_WriteText(bufor);//69
		LCD_WriteText("V");
       
		//--
		LCD_GoTo(6,0);
		itoa(converter->raw_voltage_output,bufor,10);
		LCD_WriteText(bufor);
		LCD_WriteText("  ");
		LCD_GoTo(7,1);
		oblicz = (converter->raw_voltage_output)*568; // 768 * 250   //1500
		oblicz = oblicz/25; //369   // w przypadku timer0 to bedzie polowa z 255 czyli 127.5
		itoa(oblicz/100,bufor,10);      //konwersja wyniku do tablicy char
		LCD_WriteText(bufor);//3
		
		LCD_WriteText(",");//
		itoa(oblicz%100,bufor,10);
		LCD_WriteText(bufor);//69
		LCD_WriteText("V");
		//_delay_ms(200);         //opó?nienie
		//_delay_ms(200); 

		//LCD_Clear();
		//_delay_us(5);
		
	  /* if( timerCount > 1)
	   {
		   OCR1B =20;
		   timerCount = 0;
	   }*/
		/*if(converter->raw_voltage_output <= converter->raw_voltage_input )//512 //wart zad napiecie procesora
		{
			OCR1A++;
			if(OCR1A > 20) OCR1A = 20;            //ograniczenie wype?nienia zalozyl jaka wartosc nie chce przekroczyc, np 220
			blok = 0;
		}
		else
		{
			if(blok == 0) OCR1A --;
			if(OCR1A < 1) blok = 1;
		}*/

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