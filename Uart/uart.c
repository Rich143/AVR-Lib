#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "common.h"
#include "../LCD_Driver/LCD_Driver.h"
#include "uart.h"

// use a buffer and pointers to create a circular queue to buffer data to transmit
#define BUFFER_SIZE 1024
PRIVATE char buffer[BUFFER_SIZE] = {0}; // UART buffer 
PRIVATE int bufferStart; // next byte to transmit
PRIVATE int bufferEnd; // next empty space to add byte
PRIVATE int size; // current amount of data in buffer

// PRIVATE boolean txEnable; // transmit ISR currently active 

void uartTxInterrupt(boolean enable) {
    if (enable) {
        UCSR0B |= _BV(UDRIE0); // enable UDR empty interrupts
    } else {
        UCSR0B &= ~_BV(UDRIE0); // disable UDR empty interupts
    }
}    

ISR(USART_UDRE_vect) {
    if (size == 0) {
        // buffer is empty, so shut down transmit ISR
        uartTxInterrupt(false);
        
//         LCD_Clear();
//         LCD_ReturnHome();
//         LCD_SendString("ISR OFF");
//         _delay_ms(300);
    } else {
        char transmit = buffer[bufferStart];
        UDR0 = transmit;
    
//         LCD_Clear();
//         LCD_ReturnHome();
//         LCD_SendString("ISR");
//         LCD_SetCursor(0,1);
//         LCD_SendData(buffer[bufferStart]);
//         _delay_ms(100);   

        bufferStart++;
        size--;

        if (bufferStart >= BUFFER_SIZE) {
            bufferStart = 0; // wrap around to start of array
        }
    }
}

// Initialize the UART
// Need to have #define BAUD to desired baud rate for this function to work
PUBLIC tStatus UART_Init_9600() {
//     int32_t baudPrescale = (F_CPU/(baudRate*16)) - 1;
//     baudPrescale = 103;

    // Baud prescale is a 16 bit value in 2 8 bit registers
    // Store the high byte then the low byte
//     UBRR0H = (uint8_t) baudPrescale >> 8;
//     UBRR0L = (uint8_t) baudPrescale;

    // baud rate prescaler calculate by util/setbaud.h macros
    #undef BAUD
    #define BAUD 9600
    #include <util/setbaud.h>
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

    #if USE_2X
    UCSR0A |= (1 << U2X0);
    #else
    UCSR0A &= ~(1 << U2X0);
    #endif
    
    // enable transmit, and data register empty interupt
    UCSR0B = _BV(TXEN0) | _BV(UDRIE0); 
    // Set Frame format: 8 data bits, 1 stop bit
    UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
    
    bufferStart = 0;
    bufferEnd = 0;
    size = 0;
   
//     LCD_Clear();
//     LCD_ReturnHome(); 
//     LCD_SendString("UART Prescale");
//     LCD_SetCursor(0,1);
//     LCD_Send_uint(baudPrescale);
    
    // enable global interrupts
    sei();

    return SUCCESS;
}

// Initialize the UART
PUBLIC tStatus UART_Init_57600() {
//     int32_t baudPrescale = (F_CPU/(baudRate*16)) - 1;
//     baudPrescale = 103;

    // Baud prescale is a 16 bit value in 2 8 bit registers
    // Store the high byte then the low byte
//     UBRR0H = (uint8_t) baudPrescale >> 8;
//     UBRR0L = (uint8_t) baudPrescale;

    // baud rate prescaler calculate by util/setbaud.h macros
    #undef BAUD
    #define BAUD 57600
    #include <util/setbaud.h>
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

    #if USE_2X
    UCSR0A |= (1 << U2X0);
    #else
    UCSR0A &= ~(1 << U2X0);
    #endif

//     LCD_Send_uint(UBRRH_VALUE);
//     LCD_SetCursor(0,1);
//     LCD_Send_uint(UBRRL_VALUE);

    // enable transmit, and data register empty interupt
    UCSR0B = _BV(TXEN0) | _BV(UDRIE0); 
    // Set Frame format: 8 data bits, 1 stop bit
    UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
    
    bufferStart = 0;
    bufferEnd = 0;
    size = 0;
   
//     LCD_Clear();
//     LCD_ReturnHome(); 
//     LCD_Send_uint(UBRRH_VALUE);
//     LCD_SetCursor(0,1);
//     LCD_Send_uint(UBRRL_VALUE);
    
    // enable global interrupts
    sei();

    return SUCCESS;
}
PUBLIC tStatus UART_Transmit(char* data) {
    while (*data) {
        if (UART_putchar(*data) != SUCCESS) {
            return ERROR;
        }
        
        data++;
    }

    return SUCCESS;
}

PUBLIC tStatus UART_putchar(char data) {
    // disable interupts while in critical region
    uartTxInterrupt(false);
        
    // check that buffer isn't full
    if (size >= BUFFER_SIZE) {
        uartTxInterrupt(false);
        
        size = 0;
        bufferStart = 0;
        bufferEnd = 0;
        
        UART_Transmit("\n\n***Buffer Full***\n\n");
        
        uartTxInterrupt(true);

        return ERROR;
    }
    
    // add data to buffer
    buffer[bufferEnd] = data;
    
    // increment position in circular queue, and increment size
    bufferEnd++;
    size++;

    // wrap pointer around if past end of array
    if (bufferEnd >= BUFFER_SIZE) {
        bufferEnd = 0;
    }

    if (size != 0) {
        uartTxInterrupt(true);
    }

    // need to send Carriage return as well as new line
    // do that for the user
    if (data == '\n') {
        UART_putchar('\r');
    }

    return SUCCESS;
}

// PUBLIC tStatus UART_Transmit(char* data) {
//     static boolean bufferError = false;
// 
//     if (bufferError && size !=0) {
//         return ERROR;
//     } else if (bufferError) {
//         bufferEnd = bufferStart;
//         bufferError = false;
//     }
// 
//     uartTxInterrupt(false);
// 
//     // loop through the whole string
//     while (*data) {
//         // buffer shared between interupt and normal code
//         // disable interupt while entering critical region
// //         uartTxInterrupt(false);
// 
//         // check that buffer isn't full
//         if (size >= BUFFER_SIZE) {
//             char error[11] = "Buffer Full";
//             for (int i = bufferStart; i<=bufferStart+11; i++) {
//                 buffer[i] = error[i];
//             }
//             size = 10;
//             bufferError = true;
//             uartTxInterrupt(true);
//             return ERROR;
//         }
//         
//         buffer[bufferEnd] = (*data);
//         
//         // need to send Carriage return as well as new line
//         // do that for the user
//         if (*data == '\n') {
//             *data = '\r';
//             data--; // resend current character to send the CR
//         }
// 
//         bufferEnd++;
//         size++;
//         data++;
// 
//         // wrap pointer around if past end of array
//         if (bufferEnd >= BUFFER_SIZE) {
//             bufferEnd = 0;
//         }
//         
// //         LCD_Clear();
// //         LCD_ReturnHome();
// //         LCD_SendString("Transmit Fn");
// //         LCD_SetCursor(0,1);
// // 
// //         for (int i = 0; i<size; i++) {
// //             LCD_SendData(buffer[i]);
// //         }
// //         _delay_ms(1000);
// 
//         txEnable = true;
// //         uartTxInterrupt(true);
//     }
// 
// //     if (!txEnable) {
// //         // if the transmit ISR isn't active, enable it
// //         txEnable = true;
// //         uartTxInterrupt(true);
// //     }
//     if (size != 0) {
//         uartTxInterrupt(true);
//     }
// 
//     return SUCCESS;
// }

PUBLIC tStatus UART_Transmit_Poll(char data) {
   // Wait for empty transmit buffer 
    while ( !( UCSR0A & (1<<UDRE0)) )
        ;
    // Put data into buffer, sends the data
    UDR0 = data;

    return SUCCESS;
}
