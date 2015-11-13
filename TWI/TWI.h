/* TWI Library to interface to a peripheral over a TWI interface 
	Supports writing or reading from a 8 bit register or series of registers
    SDA = A4, SCL = A5
*/


//#include "Arduino.h"
#include "../common.h"
#include <avr/io.h>
#include <util/twi.h>

#ifndef TWI_CONSTS
#define TWI_CONSTS

/*
typedef int8_t tStatus;

#define ERROR -1
#define SUCCESS 0
*/

/*** Master Status Codes ***/

//Start has been sent
#define START 0x08

//Repeated Start sent
#define REP_START 0x10

//Acknowledge bit received after sending address in transmit mode
#define MT_SLA_ACK 0x18

//Not Ack received after sending address in transmit mode
#define MT_SLA_NACK 0x20

//Acknowledge bit received after sending address in receive mode
#define MR_SLA_ACK 0x40

//Not Acknowledge bit sent after reading data in receive mode
#define MR_DATA_NACK 0x58

// Acknowledge bit sent after reading data in receive mode
#define MR_DATA_ACK 0x50

//Acknowledge bit received after sending data in transmit mode
#define MT_DATA_ACK 0x28

// Not Ack received after sending byte in transmit mode
#define MT_DATA_NACK 0x30


/*** Slave Status Codes ***/

// In Slave transmit mode, acknowledge sent to master
#define ST_SLA_ACK 0xA8

// In slave transmit mode, data byte in TWDR sent, acknowledge received
#define ST_DATA_ACK 0xB8

// In slave transmit mode, data byte in TWDR sent, not acknowledge received
#define ST_DATA_NACK 0xC0


// In slave receive mode, acknowledge sent to master
#define SR_SLA_ACK 0x60

// In Slave receive mode, data byte received, acknowledge sent to master
#define SR_DATA_ACK 0x80

// In slave receive mode, data byte received, not acknowledge sent to master
#define SR_DATA_NACK 0x88


#define WRITE 1
#define READ 0

#define TWI_RECONNECT_DELAY 40 // the time required before reconnecting to the TWI bus

// extern unsigned long last_connection; // time since last connected to TWI bus. Used to ensure bus has time to process

/* Prototypes */

// Write to a register over a TWI interface.
// Initiates a TWI connection to the specified slave, sends the register address, then sends the data to write to the register, then closes the connection
// u8addr is the address of the register that will be written to
// u8data is the data to send
// slaveAddress is the 7bit TWI address of the slave, in the lower seven bits of the byte
tStatus TWI_RegisterWrite(uint8_t u8addr, uint8_t u8data, uint8_t slaveAddress);

// Write to multiple registers over a TWI interface.
// Initiates a TWI connection to the specified slave, sends the register address, then sends the data to write to the register, then closes the connection
// u8addr is the address of the register that will be written to
// u8data is a pointer to the start of the array of data to send
// slaveAddress is the 7bit TWI address of the slave, in the lower seven bits of the byte
tStatus TWI_RegisterWrite_Multiple(uint8_t u8addr, uint8_t* u8data, uint8_t numBytes, uint8_t slaveAddress);

// Write to a register over a TWI interface.
// Initiates a TWI connection to the specified slave, sends the register address, reads data from the register, then closes the connection
// u8addr is the address of the register that will be written to
// u8data is the location where the received data will be stored
// slaveAddress is the 7bit TWI address of the slave, in the lower seven bits of the byte
tStatus TWI_RegisterRead(uint8_t u8addr, uint8_t* u8data, uint8_t slaveAddress);

// Write to a register over a TWI interface.
// Initiates a TWI connection to the specified slave, sends the register address, reads data from the register, then closes the connection
// u8addr is the address of the register that will be written to
// u8data is the start of the array where the received data will be stored
// slaveAddress is the 7bit TWI address of the slave, in the lower seven bits of the byte
tStatus TWI_RegisterRead_Multiple(uint8_t u8addr, uint8_t* u8data, uint8_t numBytes, uint8_t slaveAddress);

// Sets up the TWI interface
// if the internal pullup flag is set, internal pullups will be enabled
// if internal pullups are not set, external pullups must be used
#define NO_INTERNAL_PULLUP 0
#define INTERNAL_PULLUP 1
tStatus TWI_Init(boolean internal_pullup);


void TWI_Start();

void TWI_Stop();

void TWI_Write(uint8_t u8data);

uint8_t TWI_ReadACK();

uint8_t TWI_ReadNACK();

uint8_t TWI_GetStatus();

uint8_t TWI_GetStatus_Serial();

void do_nothing();

#endif
