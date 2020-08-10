/*---------------------------------UART0_H---------------------------------*/

#ifndef UART_H
#define UART_H

void UART0_init(void);
void UART0_send_str(char *string);
void UART0_send_int(int x);
void UART0_send_float(float x);

#endif
