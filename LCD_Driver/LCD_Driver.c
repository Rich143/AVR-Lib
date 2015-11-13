//  LCD_Driver.cpp
//  
//
//  Created by Richard Matthews on 2015-03-07.
//
//  A library to control a 2x16 HD44780 based LDC character display mounted on the adafruit RGB shield
//  Requires only two connections, SDA and SCL, since it communicates to the display through TWI
//

#include "LCD_Driver.h"
#include "../TWI/TWI.h"
// #include <math.h>
#include <util/delay.h>

// Variable used to store previous value of the ports so don't change other bits than what you want
uint8_t lPORTA = 0;
uint8_t lPORTB = 0;

void LCD_Init()
{
    TWI_Init(false); // Initialize the TWI interface, no internal pullups
    
    TWI_RegisterWrite(IODIRB, 0x00, PE_Addr); // set PORT B and A two outputs
    TWI_RegisterWrite(IODIRA, 0x00, PE_Addr);
    
    TWI_RegisterWrite(GPIOA, 0x0, PE_Addr); // clear the ports
    TWI_RegisterWrite(GPIOB, 0x0, PE_Addr);
    
    
    _delay_ms(17); // delay for 15 ms to allow LCD to self reset
    
    // software reset
    PE_SendNibble(0b0011,0); // command, function set intialize
    
    _delay_ms(5);

    PE_SendNibble(0b0011,0);

    _delay_ms(1);

    PE_SendNibble(0b0011,0); // end software reset

    _delay_ms(1);

    PE_SendNibble(0b0010, 0); // set to 4 bit mode, (NOTE: 1 nibble operation)
    
    _delay_ms(5); // allow the LCD to process
    
    LCD_SendCommand(0b00101000); // Function set, 4 bit
    
    _delay_ms(1);
    
    LCD_SendCommand(0b00001111); // Display ON, Cursor ON, Cursor Blinking
    
    _delay_ms(1);
    
    LCD_SendCommand(0b00000001); // Clear Display
    
    _delay_ms(1);
    
    LCD_SendCommand(0b00000110); // Entry mode, Increment cursor position, No display shift
    
    _delay_ms(1);
    
    LCD_SetColour(1,1,1); // set the backlight to all LEDs on
    
    _delay_ms(1);
}

void LCD_SendString(char *  var)
{
    while(*var) // keep sending until the string ends, will stop since strings are null terminated
    {
        LCD_SendData(*var);
        var++;
    }
}

void LCD_SendString_Typewriter(char *  var)
{
    while(*var) // keep sending until the string ends, will stop since strings are null terminated
    {
        LCD_SendData(*var);
        _delay_ms(300);
        var++;
    }
}

void LCD_NoCursor()
{
    LCD_SendCommand(0x0C);
}

void LCD_Clear()
{
    LCD_SendCommand(0b00000001);
    _delay_ms(2); // this command takes longer than others to excecute
}

void LCD_ReturnHome()
{
    LCD_SendCommand(0b00000010);
    _delay_ms(2); // this command takes longer than others to excecute
}

void LCD_SetCursor(uint8_t pos, uint8_t line)
{
    if (line == 1)
        LCD_SendCommand(pos+0x40+LCD_DDRAM);
    else
        LCD_SendCommand(pos+LCD_DDRAM);
}

void LCD_BlankDisplay()
{
    LCD_SendCommand(0x08);
}

void LCD_RestoreDisplay()
{
    LCD_SendCommand(0x0C);
}

void LCD_SendCommand(uint8_t command)
{
    ////Serial.println("Sending First Nibble");
    
    PE_SendNibble(command>>4, 0); // send the first (upper 4 bits) nibble with RS low
    
    ////Serial.println("Sending Second Nibble");
    
    PE_SendNibble(command&0x0F, 0); // send the second (lower 4 bits) nibble with RS low
}

void LCD_SendData(uint8_t data)
{
    PE_SendNibble(data>>4, 1); // send the first (upper 4 bits) nibble with RS High
    
    PE_SendNibble(data&0x0F, 1); // send the second (lower 4 bits) nibble with RS high
    
    _delay_us(40);
}

void LCD_SetColour(boolean red, boolean green, boolean blue)
{
    uint8_t newPortA = 0;
    uint8_t newPortB = 0;
    
    
    if (!red)
        newPortA |= 0b01000000;
    if (!green)
        newPortA |= 0b10000000;
    if (!blue)
        newPortB |= 0b00000001;
    
    lPORTA = (lPORTA & 0b00111111)  | newPortA;
    lPORTB = (lPORTB & 0b11111110) | newPortB;
    
    
    TWI_RegisterWrite(GPIOA, lPORTA, PE_Addr);
    TWI_RegisterWrite(GPIOB, lPORTB, PE_Addr);
}


void LCD_Send_uint(uint32_t num)
{
    if (num==0) {
        LCD_SendData('0');    
        return;
    }
        
    int32_t remainder = num/10;

    LCD_Send_uint_recursive(remainder);
        
    LCD_SendData( ( num - (remainder*10) ) + '0');
}

void LCD_Send_uint_recursive(uint32_t num) {
    if (num == 0) {
        return;
    }

    int32_t remainder = num/10;

    LCD_Send_uint_recursive(remainder);
    LCD_SendData((num - (remainder*10)) + '0');
}

void PE_SendNibble(uint8_t nibble, uint8_t rs)
{
    uint8_t newPORTB;
    
    // reverse the nibble, since the LCD data wires are the reverse of the PE pinout
    // then shift it to the first 4 bits
    uint8_t rev_nibble = (reverse_byte(nibble)>>4);
    
    // The desired value for the bits we want to change in PORTB
    newPORTB = ((rev_nibble&0x0F)<<1) | ((rs&1)<<7);
    
    // mask out the bits we don't want to change, and OR in the new data and RS bits, setting enable high
    lPORTB = (lPORTB & 0x41) | newPORTB | 0b00100000;
    
    TWI_RegisterWrite(GPIOB, lPORTB, PE_Addr); // write the new value for GPIOB to the port expander
    
    _delay_ms(1); // delay 1 millisecond to let the pins change
    
    // mask out the bits we don't want to change, and OR in the new data and RS bits, setting enable low
    lPORTB = (lPORTB & 0x41) | newPORTB;
    
    TWI_RegisterWrite(GPIOB, lPORTB, PE_Addr); // write the new value for GPIOB to the port expander
    
    //_delay_ms(1);  // delay 1 millisecond to let the pins change and for the data to be processed
    
    _delay_us(5);
}



void print_binary(uint8_t bin)
{
//     uint8_t z = 0b10000000;
//     while (z>0)
//     {
//         if (bin & z)
//             //Serial.print("1");
//         else
//             //Serial.print("0");
//         z>>=1;
//     }
//     
//     //Serial.print("\n");
//     
}

uint8_t reverse_byte(uint8_t val)
{
    uint8_t result = 0;
    
    int counter = 8;
    while (counter-- > 0)
    {
        result <<= 1;         
        result |= (uint8_t)(val & 1);         
        val = (uint8_t)(val >> 1);
    }
    
    return result;
}
