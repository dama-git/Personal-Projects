#ifndef __UART_H__
#define __UART_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void uart_init (void);
unsigned char uart_recieve (void);
void uart_transmit (char data);
void uart_string_transmit (char *str);
int usart_tx_char (char ch, FILE *unused);

#endif // __UART_H__



