/* 
 * File:   main.c
 * Author: bocal
 *
 * Created on May 24, 2018, 10:18 AM
 */

#include <p32xxxx.h>

#define SPI_BAUD_RATE 1

/*
 * 
 */

void SPI1_Write(unsigned char data)
{
    while (!SPI1STATbits.SPITBE); // SPI Transmit Buffer Full Status bit
    SPI1BUF = data;
}

unsigned char SPI1_Read()
{
    if (SPI1STATbits.SPIRBF)     // Receive buffer is full - Auto cleared
    {
      if (SPI1STATbits.SPIROV)
      {
        SPI1STATbits.SPIROV = 0; // Overflow has occured
        return (0xff);
      }
      return (SPI1BUF & 0xFF);   // 8-bit mode
    }
    return (0);
}

#define _CS1_ON() (PORTFbits.RF0 = 1)
#define _CS1_OFF() (PORTFbits.RF0 = 0)

// DEVCFG2
//#pragma config FPLLIDIV = DIV_2         // PLL Input Divider (2x Divider)
//#pragma config FPLLMUL = MUL_20         // PLL Multiplier (20x Multiplier)
//#pragma config FPLLODIV = DIV_1         // System PLL Output Clock Divider (PLL Divide by 1)

// DEVCFG1
//#pragma config FNOSC = PRIPLL           // Oscillator Selection Bits (Primary Osc w/PLL (XT+,HS+,EC+PLL))
//#pragma config POSCMOD = HS             // Primary Oscillator Configuration (HS osc mode)

//#define SYSCLK (8000000/2*20/1) // = 80Mhz
//#define PBCLK (SYSCLK/8) // = 10 Mhz //
//#define Fsck 5000

int main()
{
    SPI1CON = 0x8220;
    SPI1BRG = SPI_BAUD_RATE; // Set baud rate
    SPI1CONbits.MSTEN = 1; // MSTEN is to enable master mode
    SPI1CONbits.ON = 1;
    TRISFbits.TRISF0 = 0; // writeable
    _CS1_ON();
    /* Page program */
//    SPI2_Write(0x06); // Write enable (WEN -> 1)
//    SPI2_Write(0x02); // Page program
//    SPI2_Write(0x01); // Add[1]
//    SPI2_Write(0x00); // Add[2]
//    SPI2_Write(0x00); // Add[3]
//    SPI2_Write('H');
//    SPI2_Write('e');
//    SPI2_Write('l');
//    SPI2_Write('l');
//    SPI2_Write('o');
//    SPI2_Write('!');
//    /* Read page */
    _CS1_OFF();
//    SPI2_Write(0x03); // Read
//    SPI2_Write(0x01); // Add[0]
//    SPI2_Write(0x00); // Add[1]
//    SPI2_Write(0x00); // Add[2]
//    _CS1_ON();
    SPI1_Write(0xAB);
    SPI1_Write(0);
    SPI1_Write(0);
    SPI1_Write(0);
    char myData;
    while (1) {
        SPI1_Write(0);
        myData = SPI1_Read();
    }
    return (0);
}