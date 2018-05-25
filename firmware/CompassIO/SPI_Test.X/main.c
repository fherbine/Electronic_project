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

void SPI2_Write(unsigned char data)
{
    while (!SPI2STATbits.SPITBE); // SPI Transmit Buffer Full Status bit
    SPI2BUF = data;
}

unsigned char SPI2_Read()
{
    if (SPI2STATbits.SPIRBF)     // Receive buffer is full - Auto cleared
    {
      if (SPI2STATbits.SPIROV)
      {
        SPI2STATbits.SPIROV = 0; // Overflow has occured
        return (0xff);
      }
      return (SPI2BUF & 0xFF);   // 8-bit mode
    }
    return (0);
}

#define _CS1_ON() (TRISFbits.TRISF0 = 1)
#define _CS1_OFF() (TRISFbits.TRISF0 = 0)

// DEVCFG3
// USERID = No Setting

// DEVCFG2
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider (2x Divider)
#pragma config FPLLMUL = MUL_20         // PLL Multiplier (20x Multiplier)
#pragma config FPLLODIV = DIV_1         // System PLL Output Clock Divider (PLL Divide by 1)

// DEVCFG1
#pragma config FNOSC = PRI              // Oscillator Selection Bits (Primary Osc (XT,HS,EC))
#pragma config FSOSCEN = ON             // Secondary Oscillator Enable (Enabled)
#pragma config IESO = ON                // Internal/External Switch Over (Enabled)
#pragma config POSCMOD = HS             // Primary Oscillator Configuration (HS osc mode)
#pragma config OSCIOFNC = ON            // CLKO Output Signal Active on the OSCO Pin (Enabled)
#pragma config FPBDIV = DIV_8           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/8)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Clock Switch Disable, FSCM Disabled)
#pragma config WDTPS = PS1048576        // Watchdog Timer Postscaler (1:1048576)
#pragma config FWDTEN = ON              // Watchdog Timer Enable (WDT Enabled)

// DEVCFG0
#pragma config DEBUG = OFF              // Background Debugger Enable (Debugger is disabled)
#pragma config ICESEL = ICS_PGx2        // ICE/ICD Comm Channel Select (ICE EMUC2/EMUD2 pins shared with PGC2/PGD2)
#pragma config PWP = OFF                // Program Flash Write Protect (Disable)
#pragma config BWP = OFF                // Boot Flash Write Protect bit (Protection Disabled)
#pragma config CP = OFF                 // Code Protect (Protection Disabled)

//// DEVCFG2
//#pragma config FPLLIDIV = DIV_2         // PLL Input Divider (2x Divider)
//#pragma config FPLLMUL = MUL_20         // PLL Multiplier (20x Multiplier)
//#pragma config FPLLODIV = DIV_1         // System PLL Output Clock Divider (PLL Divide by 1)
//
//// DEVCFG1
//#pragma config FNOSC = PRIPLL           // Oscillator Selection Bits (Primary Osc w/PLL (XT+,HS+,EC+PLL))
//#pragma config POSCMOD = HS             // Primary Oscillator Configuration (HS osc mode)
////
//#define SYSCLK (8000000/2*20/1) // = 80Mhz
//#define PBCLK (SYSCLK/8) // = 10 Mhz //
//#define Fsck 5000

int main()
{
    SPI2BRG = SPI_BAUD_RATE; // Set baud rate
    SPI2CONbits.MSTEN = 1; // MSTEN is to enable master mode
    SPI2CONbits.ON = 1;
    _CS1_ON();
    /* Page program */
    SPI2_Write(0x06); // Write enable (WEN -> 1)
    SPI2_Write(0x02); // Page program
    SPI2_Write(0x01); // Add[1]
    SPI2_Write(0x00); // Add[2]
    SPI2_Write(0x00); // Add[3]
    SPI2_Write('H');
    SPI2_Write('e');
    SPI2_Write('l');
    SPI2_Write('l');
    SPI2_Write('o');
    SPI2_Write('!');
//    /* Read page */
//    SPI2_Write(0x03); // Read
//    SPI2_Write(0x01); // Add[0]
//    SPI2_Write(0x00); // Add[1]
//    SPI2_Write(0x00); // Add[2]
    SPI2_Write(0b1010);
    while (1) {
        unsigned char myData = SPI2_Read();
    }
    _CS1_OFF();
    return (0);
}