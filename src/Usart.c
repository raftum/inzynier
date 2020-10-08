/*
 * Usart.c
 *
 * Created: 2019-05-12 12:09:52
 *  Author: Rafal
 */ 
#include "../inc/Usart.h"
#include <avr/io.h>
#include <stdlib.h>


#define Error 255
#define USART_Init_State 0
#define USART_Get_Len_State 1
#define USART_Get_Data_State 2
#define USART_Message_Timeout 10000

#define USART_Message_OK 1
#define USART_Message_NOK 0
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

unsigned char USART_Receive(void)
{
	//while (!(UCSR0A & (1<<RXC0)));	
	if(UCSR0A & (1<<RXC0))
	{
		return UDR0;
	}
		
	return Error;
}

message USART_MessageManager()
{
	static uint8_t state = USART_Init_State;
	static message ms;
	static message dummyMessage;
	uint8_t datapart = 0;
	static uint8_t cnt = 0;
	static uint16_t noMessageCnt = 0;

	switch (state)
	{
		case(USART_Init_State):
			ms.len = 0;
			ms.buffer[0] = '0'; ms.buffer[1] = '0'; ms.buffer[2] = '0'; ms.buffer[3] = '0';
			ms.status = USART_Message_NOK;
			cnt = 0;
			dummyMessage.len = 0;//informuje w main ze ramka/dane jeszcze nie przyszla

			state = USART_Get_Len_State;//przenosi do nowego stanu 
			break;

		case(USART_Get_Len_State):

			ms.len = USART_Receive();
			if (ms.len != Error && ms.len != 0 && ms.len <= 4) //zabezpieczenie
			{
				state = USART_Get_Data_State;
				noMessageCnt = 0;
			}
			noMessageCnt++;
			break;

		case(USART_Get_Data_State):
			datapart = USART_Receive();

			if (datapart != Error && cnt < ms.len)
			{
				ms.buffer[cnt] = datapart;
				cnt++;
				noMessageCnt = 0;
			}
			if (cnt == ms.len)
			{
				ms.status = USART_Message_OK;
				state = USART_Init_State;
			}
			noMessageCnt++;
			break;
	}

	if (noMessageCnt > USART_Message_Timeout)
	{
		state = USART_Init_State;
		noMessageCnt = 0;
	}

	if (ms.status == USART_Message_OK)
	{
		return ms;
	}
	else
	{
		return dummyMessage;
	}
}

/*uint8_t* USART_GetMessage(void)
{
	char local_temp = USART_Receive();
	uint8_t len = 0;
	uint8_t cnt = 0;
	uint8_t buffer = 0;
	char templen[1] = {'0'};
	
	if(local_temp != Error)
	{
		templen[0] = local_temp;
		len = atoi(templen);
	}
	
	while (len > 0)
	{
		local_temp = USART_Receive();
		if(local_temp != Error)
		{
			buffer[cnt] = local_temp;
			len--;
			cnt++;
		}
	}

	return buffer;
}*/