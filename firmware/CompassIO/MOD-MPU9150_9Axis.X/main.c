/* 
 * File:   main.c
 * Author: bocal
 *
 * Created on May 7, 2018, 5:00 PM
 */

#include "header.h"

/*
 * 
 */

#define MPU9150_ADDR 0x68
#define READ_CMD 1
#define WRITE_CMD 0

void StartI2CI()
{
    I2C1CONbits.SEN = 1; // Start condition enabled
}

void IdleI2CI()
{
    /* Wait process complete */
    while (I2C1CONbits.SEN || I2C1STATbits.TBF || I2C1CONbits.PEN ||
	   I2C1CONbits.RCEN || I2C1CONbits.ACKEN || I2C1STATbits.ACKSTAT) ;
}

s8 MasterWriteI2C1(u8 data)
{
    I2C1TRN = data; /* Set data in Transmit Data Register */

    if (I2C1STATbits.IWCOL)
	return (-1); /* Write Collision Detect - Failed to write in I2CxTRN because I2C is busy */
    else
    {
	while (I2C1STATbits.TRSTAT); /* Wait master transmit is finished */
	IdleI2CI();
	if (I2C1STATbits.ACKSTAT) /* Check if Acknowledge Status bit was received, no error  */
	    return (-2); /* Data no received by the slave an error occured */
    }
    return (0);
}

void StopI2CI()
{
    I2C1CONbits.PEN = 1;
}

s8 MasterReadI2C1()
{
    I2C1CONbits.RCEN = 1; /* Received bit enabled */
    while (I2C1CONbits.RCEN) ; /* Waiting for receive the 8 bits */
    I2C1STATbits.I2COV = 0;
    return (I2C1RCV);
}

void I2C1SendData(u8 data, u8 addr)
{
    StartI2CI();
    IdleI2CI(); // SEN
    MasterWriteI2C1((addr << 1) | WRITE_CMD);
    IdleI2CI(); // TBF - Transmit Buffer Full
    MasterWriteI2C1(data);
    StopI2CI();
    IdleI2CI(); // PEN - Stop Condition Enabled
}

void I2C1ReceiveData(u8 addr)
{
    StartI2CI();
    IdleI2CI(); // SEN
    MasterWriteI2C1((addr << 1) | READ_CMD);
    IdleI2CI(); // TBF - Transmit Buffer Full
    
}

void main()
{
    TRISDbits.TRISD8 = 1; //1 == readeable
    TRISFbits.TRISF1 = 0; //writable
    LATFbits.LATF1 = 0;
    u8 val = 1;
    while (1) {
        if (PORTDbits.RD8 == 0) {
            if (val) {
                val = 0;
                LATFbits.LATF1 ^= 1;
            }
        } else {
            val = 1;
        }
    }
}

