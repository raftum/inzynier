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
//nowy projekt

volatile char bufor[5];
volatile int timerCount = 0;
int main(void)
{
	LCD_Initalize();
    //PWM_init() ;
    ADC_start();
    ADC_init();
    TIMER0_init();
	//PWM_select_mode(0);
	PWM_ICR();
sei(); // wlaczenie globalnych przerwan
	
	uint16_t oblicz=0;
	//uint8_t blok = 0; //zmienna blokujaca dalsze zmniejszenie OCR1A

    //uint32_t wynik_pomiaru=0;
	/*LCD_GoTo(0, 0);
	LCD_WriteText("1:");
	LCD_GoTo(7, 0);
	LCD_WriteText("2:");
	LCD_GoTo(0, 1);
	LCD_WriteText("3:");
	LCD_GoTo(11, 1);
	LCD_WriteText("4:");*/
	while (1) //Pêtla g³ówna
	{
		//BUTTON_check_pressed(); //sprawdz przycisk
		//adc_wartosc_z_przetwornika=ADC_init(2);
         
	   
         
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
		oblicz = (converter->raw_voltage_output)*250; // 768 * 250   //1500
		oblicz = oblicz/25; //369   // w przypadku timer0 to bedzie polowa z 255 czyli 127.5
		itoa(oblicz/100,bufor,10);      //konwersja wyniku do tablicy char
		LCD_WriteText(bufor);//3
		
		LCD_WriteText(",");//
		itoa(oblicz%100,bufor,10);
		LCD_WriteText(bufor);//69
		LCD_WriteText("V");
		_delay_ms(200);         //opóŸnienie
		_delay_ms(200); 

		LCD_Clear();
	   if( timerCount > 1)
	   {
		   OCR1B =20;
		   timerCount = 0;
	   }
		/*if(converter->raw_voltage_output <= converter->raw_voltage_input )//512 //wart zad napiecie procesora
		{
			OCR1A++;
			if(OCR1A > 20) OCR1A = 20;            //ograniczenie wype³nienia zalozyl jaka wartosc nie chce przekroczyc, np 220
			blok = 0;
		}
		else
		{
			if(blok == 0) OCR1A --;
			if(OCR1A < 1) blok = 1;

		}*/

	}
}

ISR(TIMER0_OVF_vect)//przerwanie przepe³nienie timer0
{
	ADC_select_channel();
	if(converter->adc_ready_flag == 1)
	{
		converter->adc_ready_flag = 0;
		//pwm_select_algorithm();
	}
	TCNT0 = 0;  //Pocz¹tkowa wartoœæ licznika
}
ISR(TIMER1_COMPA_vect)
{
	++timerCount;
}