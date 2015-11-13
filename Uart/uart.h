#ifndef __UART_H_
#define __UART_H_

#include "common.h"

// note, do not put semicolon after macro
#define STDOUT_TO_UART() \
    FILE uart_output = FDEV_SETUP_STREAM(UART_putchar, NULL, _FDEV_SETUP_WRITE); \
    stdout = &uart_output

PUBLIC tStatus UART_Init_9600();
PUBLIC tStatus UART_Init_57600();

PUBLIC tStatus UART_Transmit(char* data);

PUBLIC tStatus UART_putchar(char data);

PUBLIC tStatus UART_Transmit_Poll(char data);

#endif
