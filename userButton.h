#ifndef __USERBUTTON__H_
#define __USERBUTTON__H_
#include "driverlib.h"

void userButton_restore(void); 
static inline bool get_user_button(void)
{
    return !GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN6);    
}

#endif

