
#include "types.h"



void Init_SPI2()
{
    SPI2BRG = 127; // Set baud rate
    SPI2CONbits.MSTEN = 1; // MSTEN is to enable master mode
    SPI2CONbits.CKE = 1;
    SPI2CONbits.CKP = 0;
    SPI2CONbits.ON = 1;
    TRISDbits.TRISD5 = 0; // writeable
}

void SPI2_Write(unsigned char data)
{
    while (!SPI2STATbits.SPITBE); // SPI Transmit Buffer Full Status bit
    SPI2BUF = data;
}

void Handle_SPI(u8 input, u8 *output)
{
    SPI2BUF = input;
    while (!SPI2STATbits.SPIRBF);
    *output = SPI2BUF;
}

u8 SPI2_Read()
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
