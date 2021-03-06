/*
 * File:   spi.c
 * Author: vomnes
 *
 * Created on May 17, 2018, 10:43 AM
 */

#include "types.h"

/* == SPI1 == */

void SPI1_Init()
{
    u8 rData;
    
    IEC0CLR = IEC0_SPI1E | IEC0_SPI1TX | IEC0_SPI1RX; // Disable interrupts SPI1 IE20 <25-23>
    SPI1CON = 0x0;        // Stop and resets the SPI1
    rData = SPI1BUF;      // Clears the receive buffer
    IEC0CLR = IEC0_SPI1E | IEC0_SPI1TX | IEC0_SPI1RX; // Disable interrupts SPI1 IE20 <25-23>
    /* Here to manage interrupts */

    SPI1BRG = SPI_BAUD_RATE; // Set baud rate
    SPI1STATCLR = SPIROV_BIT;    // Clear Receive Overflow Flag Bit
    SPI1CON = ON_BIT | SMP_BIT | MSTEN_BIT; // MSTEN is to enable master mode
}

u8 SPI1_Read_Data_Ready()
{
  return SPI1STATbits.SPIRBF;
}


u8 SPI1_Write_Data_Ready()
{
  /* Automatically cleared in hardware when the SPI module
     transfers data from SPITXB to SPISR */
  return SPI1STATbits.SPITBE;
}

// SDIxpin -> SPIxSR
u8 SPI1_Read()
{
    if (SPI1STATbits.SPIRBF)     // Receive buffer is full - Auto cleared
    {
      if (SPI1STATbits.SPIROV)
        SPI1STATbits.SPIROV = 0; // Overflow has occured
      return (SPI1BUF & 0xFF);   // 8-bit mode
    }
    return (0);
}

// SPIxSR -> SDOxpin
void SPI1_Write(u8 data)
{

    while (!SPI1STATbits.SPITBE); // SPI Transmit Buffer Full Status bit
    UART2_Send_Data_Byte(data);
    SPI1BUF = data;
}

void SPI1_Write_String(char *data, unsigned int len)
{
  while (len)
  {
    SPI1BUF = *data++;
    while (SPI1_Write_Data_Ready()) ; /* Wait until SPITBE is cleared */
    len--;
  }
}

/* == SPI2 == */

void SPI2_Init()
{
    //u8 rData;

    //IEC1CLR = IEC1_SPI2E | IEC1_SPI2TX | IEC1_SPI2RX; // Disable interrupts SPI1 IE20 <25-23>
    SPI2CON = 0;        // Stop and resets the SPI1
    //rData = SPI2BUF;      // Clears the receive buffer
    //IEC1CLR = IEC1_SPI2E | IEC1_SPI2TX | IEC1_SPI2RX; // Disable interrupts SPI1 IE20 <25-23>
    /* Here to manage interrupts */

    SPI2BRG = SPI_BAUD_RATE; // Set baud rate
    //SPI2STATCLR = SPIROV_BIT;    // Clear Receive Overflow Flag Bit
    SPI2CON = MASTER; // MSTEN is to enable master mode
}

u8 SPI2_Read_Data_Ready()
{
  /* If SPIRBF is set the buffer is full */
  return SPI2STATbits.SPIRBF;
}


u8 SPI2_Write_Data_Ready()
{
  /* Automatically cleared in hardware when the SPI module
     transfers data from SPITXB to SPISR */
  return SPI2STATbits.SPITBE;
}

// SDIxpin -> SPIxSR
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

// SPIxSR -> SDOxpin
void SPI2_Write(u8 data)
{
    while (!SPI2STATbits.SPITBE); // SPI Transmit Buffer Full Status bit
    SPI2BUF = data;
}

void SPI2_Write_String(char *data, unsigned int len)
{
  while (len)
  {
    SPI2BUF = *data++;
    while (SPI2_Write_Data_Ready()) ; /* Wait until SPITBE is cleared */
    len--;
  }
}
