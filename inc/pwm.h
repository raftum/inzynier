/*
 * pwm.h
 *
 * Created: 2019-03-19 22:23:55
 *  Author: Rafal
 */ 


#ifndef PWM_H_
#define PWM_H_

#include <avr/io.h>
#include <stdint.h>

void PWM_init();
void PWM_count_duty(uint32_t PWM_duty);
//void PWM_frequency();
//void PWM_change_duty(uint16_t percent);
void PWM_select_mode(uint8_t selector);
//uint8_t PWM_get_mode();
void PWM_ICR();
void PWM_UpdateOCR0B(uint8_t value);
static uint8_t _mode;
#endif // PWM_H