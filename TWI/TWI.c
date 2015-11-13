//#include "Arduino.h"
#include "avr/io.h"
#include "util/twi.h"
#include "TWI.h"

tStatus TWI_RegisterWrite(uint8_t u8addr, uint8_t u8data, uint8_t slaveAddress)
{
    TWI_Start(); // send a start code to begin the write
    
    uint8_t status = TWI_GetStatus();
    if (status != START && status != REP_START) // start not sent/acknowledged
    {
        //Serial.print("TWI Write failed, start not sent.");
        //Serial.print(" Status code is: 0x"); Serial.println(status,HEX);
        return ERROR;
    }
    
    
    TWI_Write( (slaveAddress<<1) ); // write the address shifted so that last bit is 0, meaning write request
    
    status = TWI_GetStatus();
    if (status != MT_SLA_ACK) // SLA+W was not acknowledged
    {
        TWI_Stop(); // send a stop to end failed transmission
        
        //Serial.print("TWI Write failed, sla address not sent.");
        //Serial.print(" Status code is: 0x"); Serial.println(status,HEX);
        return ERROR;
    }
    
    TWI_Write(u8addr); // send the address to write to
    
    status = TWI_GetStatus();
    if (status != MT_DATA_ACK) // the address was not sent
    {
//         Serial.print("TWI Write failed, register address not sent");
//         Serial.print(" Status code is: 0x"); Serial.println(status,HEX);
        return ERROR;
    }
    
    TWI_Write(u8data); // send the data to write
    
    status = TWI_GetStatus();
    if (status != MT_DATA_ACK)
    {
//         Serial.print("TWI Write failed, data not sent");
//         Serial.print(" Status code is: 0x"); Serial.println(status,HEX);
        return ERROR;
    }
    
    
    TWI_Stop();

    return SUCCESS; 
}

tStatus TWI_RegisterWrite_Multiple(uint8_t u8addr, uint8_t* u8data, uint8_t numBytes, uint8_t slaveAddress)
{
	TWI_Start(); // send a start code to begin the write
    
    uint8_t status = TWI_GetStatus();
    if (status != START && status != REP_START) // start not sent/acknowledged
	{
//    Serial.print("TWI Write failed, start not sent");
//         Serial.print(" Status code is: 0x"); Serial.println(status,HEX);
		return ERROR;
	}
		
	TWI_Write( (slaveAddress<<1) ); // write the address shifted so that last bit is 0, meaning write request
    status = TWI_GetStatus();
	if (status != MT_SLA_ACK) // SLA+W was not acknowledged
	{
//         Serial.print("TWI Write failed, sla address not sent");
//         Serial.print(" Status code is: 0x"); Serial.println(status,HEX);
		return ERROR;
	}
		
	TWI_Write(u8addr); // send the address to write to
    status = TWI_GetStatus();
	if (status != MT_DATA_ACK) // the address was not sent
	{
//         Serial.print("TWI Write failed, register address not sent");
//         Serial.print(" Status code is: 0x"); Serial.println(status,HEX);
		return ERROR;
	}
	
	for (int i=0; i<numBytes; i++)	
	{	
		TWI_Write(u8data[i]); // send the data to write
        status = TWI_GetStatus();
		if (status != MT_DATA_ACK)
		{
//             Serial.print("TWI Write failed, data not sent. Byte #"); Serial.print(i);
//             Serial.print(" Status code is: 0x"); Serial.println(status,HEX);
			return ERROR;
		}
	}
		
	TWI_Stop();
	return SUCCESS; 
}


tStatus TWI_RegisterRead(uint8_t u8addr, uint8_t* u8data, uint8_t slaveAddress)
{
	TWI_Start(); // send a start code to begin the write
    uint8_t status = TWI_GetStatus();
    if (status != START && status != REP_START) // start not sent/acknowledged
	{
//         Serial.print("TWI Read failed. Start not sent");
//         Serial.print(" Status code is: 0x"); Serial.println(status,HEX);
		return ERROR;
	}
	
    
	TWI_Write( (slaveAddress<<1) ); // write the address shifted so that last bit is 0, meaning write request
    status = TWI_GetStatus();
	if (status != MT_SLA_ACK) // SLA+W was not acknowledged
	{
//         Serial.print("TWI Read failed. address not sent");
//         Serial.print(" Status code is: 0x"); Serial.println(status,HEX);
		return ERROR;
	}

	TWI_Write(u8addr); // send the address to read from
    status = TWI_GetStatus();
	if (status != MT_DATA_ACK) // the address was not sent
	{
//         Serial.print("TWI Read failed. address of register not sent");
//         Serial.print(" Status code is: 0x"); Serial.println(status,HEX);
		return ERROR;
	}
	
	TWI_Start(); // send a repeated start
    status = TWI_GetStatus();
	if (status != REP_START) // start not sent
	{
//         Serial.print("TWI Read failed. Repeated Start not sent");
//         Serial.print(" Status code is: 0x"); Serial.println(status,HEX);
		return ERROR;
	}

	TWI_Write( ( (slaveAddress<<1) | 1) ); // write the address shifted so that last bit is 1, meaning read request
    status = TWI_GetStatus();
	if (status != MR_SLA_ACK) // SLA+R was not acknowledged
	{
//         Serial.print("TWI Read failed");
//         Serial.print(" Status code is: 0x"); Serial.println(status,HEX);
		return ERROR;
	}
	
	*u8data = TWI_ReadNACK(); // read the data
    status = TWI_GetStatus();
	if (status != MR_DATA_NACK)
	{
//         Serial.print("TWI Read failed");
//         Serial.print(" Status code is: 0x"); Serial.println(status,HEX);
		return ERROR;
	}

	TWI_Stop();

	return SUCCESS; 
}

tStatus TWI_RegisterRead_Multiple(uint8_t u8addr, uint8_t* u8data, uint8_t numBytes, uint8_t slaveAddress)
{
	TWI_Start(); // send a start code to begin the write
    
    uint8_t status = TWI_GetStatus();
    if (status != START && status != REP_START) // start not sent/acknowledged
	{
//         Serial.print("TWI Read failed");
//         Serial.print(" Status code is: 0x"); Serial.println(status,HEX);
		return ERROR;
	}
		
	TWI_Write( (slaveAddress<<1) ); // write the address shifted so that last bit is 0, meaning write request
    status = TWI_GetStatus();
	if (status != MT_SLA_ACK) // SLA+W was not acknowledged
	{
//         Serial.print("TWI Read failed");
//         Serial.print(" Status code is: 0x"); Serial.println(status,HEX);
		return ERROR;
	}

	TWI_Write(u8addr); // send the address to read from
    status = TWI_GetStatus();
	if (status != MT_DATA_ACK) // the address was not sent
	{
//         Serial.print("TWI Read failed");
//         Serial.print(" Status code is: 0x"); Serial.println(status,HEX);
		return ERROR;
	}

	TWI_Start(); // send a repeated start
    status = TWI_GetStatus();
	if (status != REP_START) // start not sent
	{
//         Serial.print("TWI Read failed");
//         Serial.print(" Status code is: 0x"); Serial.println(status,HEX);
		return ERROR;
	}

	TWI_Write( ( (slaveAddress<<1) | 1) ); // write the address shifted so that last bit is 1, meaning read request
    status = TWI_GetStatus();
	if (status != MR_SLA_ACK) // SLA+R was not acknowledged
	{
//         Serial.print("TWI Read failed");
//         Serial.print(" Status code is: 0x"); Serial.println(status,HEX);
		return ERROR;
	}
	
	for (int i=0; i<numBytes-1; i++)
	{
		u8data[i] = TWI_ReadACK(); // read the data
        status = TWI_GetStatus();
		if (status != MR_DATA_ACK)
		{
//             Serial.print("TWI Read failed. Byte #"); Serial.print(i);
//             Serial.print(" Status code is: 0x"); Serial.println(status,HEX);
			return ERROR;
		}
	}
	
	u8data[numBytes-1] = TWI_ReadNACK(); // read the last byte, and respond with a NACK to let the slave know its the last one
    status = TWI_GetStatus();
	if (status != MR_DATA_NACK)
	{
//         Serial.print("TWI Read failed");
//         Serial.print(" Status code is: 0x"); Serial.println(status,HEX);
		return ERROR;
	}
	
	TWI_Stop();
	
	return SUCCESS; 
}

// Note that internal pullups have not been enable yet
tStatus TWI_Init(boolean internal_pullup)
{
  /* Initialize the TWI clock */
  TWSR = (1<<TWPS0); // Set the prescaler to 4 (bit 0 to 1)
  TWBR = 131; // Set the bit rate to 131
  /* Gives a SCL frequency of approximately 15 KHz (16 MHz / (16 + 2(255*4) ) ) */
  
  if (internal_pullup)
  {
//       digitalWrite(A4, 1);
//       digitalWrite(A5, 1);
  }
    
  /* Enable TWI */
  TWCR = (1<<TWINT) | (1<<TWEN);

  return SUCCESS;
}


void TWI_Start()
{
  TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN); // Set the bits on the control register to send start
    
  while (! ( TWCR & (1<<TWINT) ) ); // Wait for the TWINT flag set meaning start has been transmitted
}

/*
void TWI_Stop()
{
  TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN); // set the stop, interupt, and TW enable bit in the control register
    

}
*/


void TWI_Stop()
{
    TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN); // send the stop signal
    while(TWCR & _BV(TWSTO));
}


void TWI_Write(uint8_t u8data)
{
  while (! ( TWCR & (1<<TWINT) ) ); // Wait for the TWINT flag set meaning ready to transmit
    
    
  TWDR = u8data; // load the data into the data register
	
    
  // Clear the TWINT Flag to send the data
  TWCR = (1<<TWINT) | (1<<TWEN);
  
  while (! ( TWCR & (1<<TWINT) ) ); // Wait for the TWINT flag set meaning byte has been transmitted
}

uint8_t TWI_ReadACK()
{
  while (! ( TWCR & (1<<TWINT) ) ); // Wait for the TWINT flag set meaning ready to receive
    
  TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA); // clear interrupt bit to read byte, setting the enable acknowledge bit so an ack is sent
    
  while ( (TWCR & (1<<TWINT) ) == 0);
    
  return TWDR;
}

uint8_t TWI_ReadNACK()
{
  while (! ( TWCR & (1<<TWINT) ) ); // Wait for the TWINT flag set meaning ready to receive
    
  TWCR = (1<<TWINT) | (1<<TWEN);
    
  while ( (TWCR & (1<<TWINT) ) == 0);
    
  return TWDR;
}

uint8_t TWI_GetStatus()
{
    uint8_t status;
    // mask the status register
    status = TWSR & 0xF8;

    //Serial.print("Status = ");
    //Serial.println(status, HEX);
    return status;
}

uint8_t TWI_GetStatus_Serial()
{
    uint8_t status;
    // mask the status register
    status = TWSR & 0xF8;
    
//     Serial.print("Status = ");
//     Serial.println(status, HEX);
    return status;
}
  
