/*
 * Usart.h
 *
 * Created: 2019-05-12 12:09:32
 *  Author: Rafal
 */ 


#ifndef USART_H_
#define USART_H_
#include <avr/io.h>
#include <stdint.h>
typedef struct message
{
	uint8_t len;
	unsigned char buffer[3];
	uint8_t status;
}message;
void USART_Init( unsigned int baud);
void USART_Transmit( unsigned char data);
unsigned char USART_Receive(void);
uint8_t* USART_GetMessage(void);
message USART_MessageManager();


#endif /* USART_H_ */