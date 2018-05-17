/*
 * File:   spi.c
 * Author: vomnes
 *
 * Created on May 17, 2018, 10:43 AM
 */

#include "types.h"

#define SPI_BAUD_RATE 42

void SPI1_Init()
{
    int rData;

    IEC0CLR = 0x03800000; // Disable interrupts SPI1 IE20 <25-23>
    SPI1CON = 0x0;        // Stop and resets the SPI1
    rData = SPI1BUF;      // Clears the receive buffer
    /* Here to manage interrupts */

    SPI1BRG = SPI_BAUD_RATE;
    SPI1STATbits.SPIROV = 0;

}