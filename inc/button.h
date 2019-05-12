#ifndef BUTTON_H_
#define BUTTON_H_

#include <avr/io.h>
#include <stdint.h>

#define SWITCH (1<<PC5) //definicja wyprowadzenia do którego podpiety jest microswitch
#define SWITCH_ON !(PINC & SWITCH) //makro do obslugi zwartego/wci?ni?tego przycisku SW
#define SWITCH2 (1<<PC4) //definicja wyprowadzenia do którego podpiety jest microswitch
#define SWITCH_ON2 !(PINC & SWITCH2) //makro do obslugi zwartego/wci?nietego przycisku SW

void BUTTON_Init();
void BUTTON_check_pressed();

#endif //BUTTON_H_