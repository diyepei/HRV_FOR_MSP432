#ifndef __UART2__H_
#define __UART2__H_
#include "driverlib.h"

void UART2_init(void);
bool UART2_send(char *str);
char * UART2_receive(void);
void UART2_test_send(char *str);
char * UART2_test_receive(void);

#endif

