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
void USART_Init( unsigned int baud);
void USART_Transmit( unsigned char data);
char *USART_Receive(void);


#endif /* USART_H_ */