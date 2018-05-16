/*
 * File:   i2c.c
 * Author: vomnes
 *
 * Created on May 8, 2018, 2:00 PM
 */

#include "types.h"
#define ACK 0
#define NACK 1

void Start_I2CI()
{
    I2C1CONbits.SEN = 1; // Start condition enabled
}

void Idle_I2CI()
{
    /* Wait process complete */
    while (I2C1CONbits.SEN || I2C1STATbits.TBF || I2C1CONbits.PEN ||
	   I2C1CONbits.RCEN || I2C1CONbits.ACKEN) ;
}

s8 Master_Write_I2C1(u8 data)
{
    while (I2C1STATbits.TRSTAT); /* Wait master transmit is finished */
    I2C1TRN = data; /* Set data in Transmit Data Register */
    while (I2C1STATbits.TRSTAT || I2C1STATbits.TBF); /* Wait master transmit is finished */
    return (I2C1STATbits.ACKSTAT);/* Check if Acknowledge Status bit was received, no error  */
}

void Stop_I2CI()
{
    I2C1CONbits.PEN = 1;
}

s8 Master_Read_I2C1()
{
    while (I2C1STATbits.TRSTAT); /* Wait master transmit is finished */
    I2C1CONbits.RCEN = 1; /* Received bit enabled */
    while (I2C1CONbits.RCEN) ; /* Waiting for receive the 8 bits */
    I2C1STATbits.I2COV = 0;
    return I2C1RCV;
}

#define I2C_BAUD_RATE ((PBCLK/2/Fsck)-2)

void I2C1_Init()
{
    I2C1CON = 0;
    I2C1CONbits.A10M = 0; // 7-bit slave address
    I2C1BRG = I2C_BAUD_RATE; // Set baud rate
    I2C1CONbits.ON = 1;
}

void NotAckI2C1()
{
    I2C1CONbits.ACKDT = 1;
    I2C1CONbits.ACKEN = 1;
    while (I2C1CONbits.ACKEN) ;
    I2C1CONbits.ACKDT = 0;
}

void I2C1_Send_Data(u8 data, u8 addr)
{
    Start_I2CI();
    Idle_I2CI(); // SEN
    if (Master_Write_I2C1((addr << 1) | WRITE_CMD) == NACK)
	Nop();
    //while(MasterWriteI2C1((addr << 1) | WRITE_CMD));
    Idle_I2CI(); // TBF - Transmit Buffer Full
    Master_Write_I2C1(data);
    //while(MasterWriteI2C1(data));
    Stop_I2CI();
    Idle_I2CI(); // PEN - Stop Condition Enabled
}


void I2C1_Write_Data(u8 slave_address, u8 addr, u8 data)
{
    Start_I2CI();
    Idle_I2CI(); // SEN
    Master_Write_I2C1((slave_address << 1) | WRITE_CMD);
    //while(MasterWriteI2C1((addr << 1) | WRITE_CMD));
    Idle_I2CI(); // TBF - Transmit Buffer Full
    Master_Write_I2C1(addr);
    Idle_I2CI(); // TBF - Transmit Buffer Full
    Master_Write_I2C1(data);
    Idle_I2CI(); // TBF - Transmit Buffer Full
    //while(MasterWriteI2C1(data));
    Stop_I2CI();
    Idle_I2CI(); // PEN - Stop Condition Enabled
}

u8 I2C1_Receive_Data(u8 addr)
{
    s8 data;
    Idle_I2CI();
    Start_I2CI();
    Idle_I2CI(); // SEN
    Master_Write_I2C1((addr << 1) | READ_CMD);
    //while(MasterWriteI2C1((addr << 1) | READ_CMD));
    Idle_I2CI(); // TBF - Transmit Buffer Full
    data = Master_Read_I2C1();
    //NotAckI2C1();
    Stop_I2CI();
    return data;
}
