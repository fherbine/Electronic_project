/* 
 * File:   main.c
 * Author: bocal
 *
 * Created on May 24, 2018, 10:18 AM
 */

#define SPI_BAUD_RATE 1024

/*
 * 
 */

int main()
{
    SPI2BRG = SPI_BAUD_RATE; // Set baud rate
	SPI2CONbits.MSTEN = 1; // MSTEN is to enable master mode
    SPI2CONbits.ON = 1;
	while (1)
	{
	    while (!SPI2STATbits.SPITBE); // SPI Transmit Buffer Full Status bit
	    SPI2BUF = 'A';
	}
	return (0);
}

