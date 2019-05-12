/*
 * Usart.c
 *
 * Created: 2019-05-12 12:09:52
 *  Author: Rafal
 */ 
#include "../inc/Usart.h"
char buffor[3] = {'0','0','0'};

void USART_Init( unsigned int baud )
{
	UBRR0H = (unsigned char)(baud>>8);
	UBRR0L = (unsigned char)baud;
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}
void USART_Transmit( unsigned char data )
{
	while ( !( UCSR0A & (1<<UDRE0)) );
	UDR0 = data;
}

char* USART_Receive(void)
{
	static uint8_t i = 0 ;
	while (!(UCSR0A & (1<<RXC0)));
	if (UDR0!='q')
	{
		buffor[i]=UDR0;
		i++;
		USART_Receive();
	}
	else
	{
		i=0;
		UDR0=0;
	}
	return buffor;
	
	//return (char)UDR0;
}