//
//  LCD_Driver.h
//
//
//  Created by Richard Matthews on 2015-03-07.
//
//  A library to control a 2x16 HD44780 based LDC character display mounted on the adafruit RGB shield
//  Requires only two connections, SDA and SCL, since it communicates to the display through TWI
//
//

#ifndef ____LCD_Driver__
#define ____LCD_Driver__

#include <avr/io.h>
#include "../common.h"
// #include <Arduino.h>


#define PE_Addr 0x20 // The TWI slave address of the port expander chip (this is what it is configured to on the adafruit shield, it can be changed)

// The memory addresses of the control registers used on the port expander
#define IODIRA 0x00 // Read/Write control (0 is write, 1 is read)
#define IODIRB 0x01
#define GPIOA 0x12 // IO control
#define GPIOB 0x13

// The value to add to a DDRAM address to set it as the next DDRAM to write to
#define LCD_DDRAM 0x80

// Variable used to store previous value of the ports so don't change other bits than what you want
extern uint8_t lPORTA;
extern uint8_t lPORTB;


/******  API Functions *******/


// Set the colour of the backlight. True turns on the LED of the associated colour, False turn it off
void LCD_SetColour(boolean red, boolean green, boolean blue);

// Initialize the LCD display. Must be called before the Display can be used
// Assumes there was a hardware reset (power reset) first.
void LCD_Init();

// Display a string on the LCD. Takes a normal C style ASCII string (must be null terminated)
void LCD_SendString(char *  var);

// Puts a delay between each character sent
void LCD_SendString_Typewriter(char * var);

// Make cursor invisible
void LCD_NoCursor();

// Clear the display
void LCD_Clear();

// Set the cursor to the home (0th column and 0th row) position
void LCD_ReturnHome();

// Set the cursor position
// Starts at 0,0. Max column is 15, max row is 1 (can actualy set farther but just wont be displayed, useful if shifting display)
void LCD_SetCursor(uint8_t pos, uint8_t line);

// Blank the display, maintaining the DDRAM
void LCD_BlankDisplay();

// Restore the display after blanking it
void LCD_RestoreDisplay();

//Send a command to the LCD (i.e. RS set to 0)
void LCD_SendCommand(uint8_t command);

// Send a character to the LCD
void LCD_SendData(uint8_t data);

// Turns up to a 32bit number into decimal characters to send to the LCD
void LCD_Send_uint(uint32_t num);

// Called by LCD_Send_uint, don't call directly
// Used to allow LCD_Send_uint to print '0' if num == 0
void LCD_Send_uint_recursive(uint32_t num);

/******* Low level functions, not to be called by the user *********/


// Send a nibble (4 bits) to the LCD, plus either RS high or low
void PE_SendNibble(uint8_t nibble, uint8_t rs);


// Debug helper function, prints to the arduino serial interface a byte in binary form
void print_binary(uint8_t bin);

// Reverses a byte of data (i.e. MSB becomes LSB and so on)
// Used because the Adafruit shield has the data wires reversed from the order in the control registers
uint8_t reverse_byte(uint8_t val);



#endif
