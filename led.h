/*---------------------------------LED_H---------------------------------*/
#ifndef LED_H
#define LED_H

#include<stdint.h>

#define RGB_RED    1
#define RGB_GREEN  2
#define RGB_BLUE   4
#define RGB_OFF    0

void LED_CONTROL_initRGB(void);
void LED_CONTROL_setRGB(uint8_t x  , uint32_t duration);

#endif
