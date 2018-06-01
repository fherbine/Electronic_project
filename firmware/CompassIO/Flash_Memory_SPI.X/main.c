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

void Handle_SPI(unsigned char input, unsigned char *output)
{
    SPI1BUF = input;
    while (!SPI1STATbits.SPIRBF);
    *output = SPI1BUF;
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
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider (2x Divider)
#pragma config FPLLMUL = MUL_20         // PLL Multiplier (20x Multiplier)
#pragma config FPLLODIV = DIV_1         // System PLL Output Clock Divider (PLL Divide by 1)

// DEVCFG1
#pragma config FNOSC = PRIPLL           // Oscillator Selection Bits (Primary Osc w/PLL (XT+,HS+,EC+PLL))
#pragma config POSCMOD = HS             // Primary Oscillator Configuration (HS osc mode)

#define SYSCLK (8000000/2*20/1) // = 80Mhz
#define PBCLK (SYSCLK/8) // = 10 Mhz //
//#define Fsck 5000

int main()
{
    SPI1CON = 0x8220;
    SPI1BRG = 127; // Set baud rate
    SPI1CONbits.MSTEN = 1; // MSTEN is to enable master mode
    SPI1CONbits.ON = 1;
    TRISFbits.TRISF0 = 0; // writeable
    unsigned char output;
    _CS1_ON();
    Handle_SPI(0xAB, &output);
    Handle_SPI(0, &output);
    Handle_SPI(0, &output);
    Handle_SPI(0, &output);
     _CS1_OFF();
    return (0);
}
