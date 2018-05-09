/*
 * File:   uart.c
 * Author: vomnes
 *
 * Created on May 9, 2018, 11:00 AM
 */

#include "types.h"

#define UART_RX_TX_ON 3
#define UART_RX_ON 2
#define UART_TX_ON 1

#define MAX_BUFFER_SIZE 256

void UART1_Init(u8 parityDataBits, u8 stopBits, u8 TRX_Mode)
{
    if (!TRX_Mode) {
	return;
    }
    U1BRG = BAUD_RATE;
    U1MODEbits.PDSEL = parityDataBits;
    U1MODEbits.STSEL = stopBits;
    U1STAbits.URXEN = TRX_Mode != 1; // Enable reception
    U1MODEbits.ON = 1; // Enable UART1 Module
    U1STAbits.UTXEN = TRX_Mode & 1; // Enable transmission and set UxTXIF (Int)
}

void UART1_Send_Data_Byte(u8 data)
{
    while (U1STAbits.UTXBF);
    U1TXREG = data;
}

u8 UART1_Send_String(const char *string, u32 size)
{
    if ((string == NULL) || (size > MAX_BUFFER_SIZE))
	return (-1);
    while (i++ < size)
	UART1_Send_Data_Byte(string[i]);
    return (0);
}

u8 UART1_Get_Data_Byte()
{
    while (!U1STAbits.URXDA); // The buffer U1RXREG contains data - Can be handled with interrupt
    return (U1RXREG); // Return the FIFO data
}