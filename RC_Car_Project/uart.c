#include "common.h"
#include "uart.h"


void uart_init (void)
{
    // Double the USART Transmission Speed Mode
    UCSR0A |= (1 << U2X0);

    // Baud Rate = 16 MHz / 8 * (UBRRn + 1)
    // (16 * 10^6) / (8 * 208) = 9615.38461538
    UBRR0H = 0x00;
    UBRR0L = 207;

    UCSR0C |= 0x6;	// Asynchronous, Data Bit 8, Parity Bit 0, Stop Bit 1

    UCSR0B |= (1 << RXEN0);
    UCSR0B |= (1 << TXEN0);
}

unsigned char uart_recieve (void)
{
    while (!(UCSR0A & (1 << RXC0)));

    return UDR0;
}

void uart_transmit (char data)
{
    while (!(UCSR0A & (1 << UDRE0)));

    UDR0 = data;
}

void uart_string_transmit (char *str)
{
    while (*str != '\0')
    {
        uart_transmit(*str);
        str++;
    }
}

int usart_tx_char (char ch, FILE *fp)
{
    while (!(UCSR0A & (1 << UDRE0)));

    UDR0 = ch;

    return 0;
}

