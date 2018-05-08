/* 
 * File:   main.c
 * Author: bocal
 *
 * Created on May 7, 2018, 5:00 PM
 */

#include "header.h"


// DEVCFG2
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider (2x Divider)
#pragma config FPLLMUL = MUL_20         // PLL Multiplier (20x Multiplier)
#pragma config FPLLODIV = DIV_1         // System PLL Output Clock Divider (PLL Divide by 1)

// DEVCFG1
#pragma config FNOSC = PRIPLL           // Oscillator Selection Bits (Primary Osc w/PLL (XT+,HS+,EC+PLL))
#pragma config POSCMOD = HS             // Primary Oscillator Configuration (HS osc mode)
/*
 * 
 */

#define MPU9150_ADDR 0x68
#define READ_CMD 1
#define WRITE_CMD 0

#define SYSCLK (8000000/2*20/1) // = 80Mhz
#define PBCLK (SYSCLK/8) // = 10 Mhz
#define Fsck 5000
#define BAUD_RATE ((PBCLK/2/Fsck)-2)

void StartI2CI()
{
    I2C1CONbits.SEN = 1; // Start condition enabled
}

void IdleI2CI()
{
    /* Wait process complete */
    while (I2C1CONbits.SEN || I2C1STATbits.TBF || I2C1CONbits.PEN ||
	   I2C1CONbits.RCEN || I2C1CONbits.ACKEN) ;
}

s8 MasterWriteI2C1(u8 data)
{
    while (I2C1STATbits.TRSTAT); /* Wait master transmit is finished */
    I2C1TRN = data; /* Set data in Transmit Data Register */
    while (I2C1STATbits.TRSTAT || I2C1STATbits.TBF); /* Wait master transmit is finished */
    /* CHECK WAY TO HANDLE ACK */
    return (I2C1STATbits.ACKSTAT);/* Check if Acknowledge Status bit was received, no error  */
}

void StopI2CI()
{
    I2C1CONbits.PEN = 1;
}

s8 MasterReadI2C1()
{
    while (I2C1STATbits.TRSTAT); /* Wait master transmit is finished */
    I2C1CONbits.RCEN = 1; /* Received bit enabled */
    while (I2C1CONbits.RCEN) ; /* Waiting for receive the 8 bits */
    I2C1STATbits.I2COV = 0;
    return I2C1RCV;
}

void OpenI2C1()
{
    I2C1CON = 0;
    I2C1CONbits.A10M = 0; // 7-bit slave address
    I2C1BRG = BAUD_RATE; // Set baud rate
    I2C1CONbits.ON = 1;
}

void I2C1SendData(u8 data, u8 addr)
{
    StartI2CI();
    IdleI2CI(); // SEN
    MasterWriteI2C1((addr << 1) | WRITE_CMD);
    //while(MasterWriteI2C1((addr << 1) | WRITE_CMD));
    IdleI2CI(); // TBF - Transmit Buffer Full
    MasterWriteI2C1(data);
    //while(MasterWriteI2C1(data));
    StopI2CI();
    IdleI2CI(); // PEN - Stop Condition Enabled
}

u8 I2C1ReceiveData(u8 addr)
{
    s8 data;
    StartI2CI();
    IdleI2CI(); // SEN
    MasterWriteI2C1((addr << 1) | READ_CMD);
    //while(MasterWriteI2C1((addr << 1) | READ_CMD));
    IdleI2CI(); // TBF - Transmit Buffer Full
    data = MasterReadI2C1();
    StopI2CI();
    IdleI2CI(); // TBF - Transmit Buffer Full
    return data;
}

// #define digitalWrite(pin, val) (_TRIS##(pin) = (val))
// digitalWrite(F1, 1);
void main()
{
    TRISFbits.TRISF1 = 0; //writable
    LATFbits.LATF1 = 0;
    OpenI2C1();
    
    LATFbits.LATF1 = 1;
    I2C1SendData( I2C1ReceiveData(0b1010101) ,0b1010101);
    LATFbits.LATF1 = 0;
    while(1) ;
}

