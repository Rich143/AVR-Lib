## AVR-Lib
A collection of drivers for the Atmega 328p microcontroller

#LCD Driver
LCD Driver code to interface with an hd44780 display. The Atmega 328p connects to a port expander over I2C, which then connects to the display. This reduces the number of pins needed on the atmega while maintaining full functionallity of the display.

#TWI Driver
TWI, which is Atmel's version of I2C, Master driver code. Allows sending and receiving of bytes, as well as writing/reading to an address or series of addresses on a slave device. When writing/reading a address on a slave device, it first sends the address and then either sends or receives one or more bytes.

#UART Driver
UART Driver code. Implements a circular buffer to allow interrupt driven data transfer. When sending using the buffer, strings are added to the buffer at one end and transmitted a byte at a time from the other end using the data register empty interrupt
