#include "types.h"



void Init_SPI1()
{
    SPI1BRG = 127; // Set baud rate
    SPI1CONbits.MSTEN = 1; // MSTEN is to enable master mode
    SPI1CONbits.CKE = 1;
    SPI1CONbits.CKP = 0;
    SPI1CONbits.ON = 1;
    TRISBbits.TRISB15 = 0; // writeable
	LATBbits.LATB15 = 0;
}

void SPI1_Write(unsigned char data)
{
    while (!SPI1STATbits.SPITBE); // SPI Transmit Buffer Full Status bit
    SPI1BUF = data;
}

void Handle_SPI(u8 input, u8 *output)
{
    SPI1BUF = input;
    while (!SPI1STATbits.SPIRBF);
    *output = SPI1BUF;
}

u8 SPI1_Read()
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
