/*
 * File:   uart.c
 * Author: vomnes
 *
 * Created on May 9, 2018, 11:00 AM
 */

#include "types.h"

void UART1_Init(u8 parityDataBits, u8 stopBits, u8 TRX_Mode)
{
    if (!TRX_Mode) {
	return;
    }
    U1BRG = UART_BAUD_RATE;
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
    u32 i;
    for (i = 0; i < size; i++)
	UART1_Send_Data_Byte(string[i]);
    return (0);
}

u8 UART1_Get_Data_Byte()
{
    while (!U1STAbits.URXDA); // The buffer U1RXREG contains data - Can be handled with interrupt
    return (U1RXREG); // Return the FIFO data
}

void UART1_Read_String(char *string, u32 size)
{
    u32 i;
    for (i = 0; i < size; i++)
	string[i] = UART1_Get_Data_Byte();
}

/* UART2 */
void UART2_Init(u8 parityDataBits, u8 stopBits, u8 TRX_Mode)
{
    if (!TRX_Mode) {
	return;
    }
    U2BRG = UART_BAUD_RATE;
    U2MODEbits.PDSEL = parityDataBits;
    U2MODEbits.STSEL = stopBits;
    U2STAbits.URXEN = TRX_Mode != 1; // Enable reception
    U2MODEbits.ON = 1; // Enable UART1 Module
    U2STAbits.UTXEN = TRX_Mode & 1; // Enable transmission and set UxTXIF (Int)
}

void UART2_Send_Data_Byte(u8 data)
{
    while (U2STAbits.UTXBF);
    U2TXREG = data;
}

u8 UART2_Send_String(const char *string, u32 size)
{
    if ((string == NULL) || (size > MAX_BUFFER_SIZE))
	return (-1);
    u32 i;
    for (i = 0; i < size; i++)
	UART2_Send_Data_Byte(string[i]);
    return (0);
}

u8 UART2_Get_Data_Byte()
{
    while (!U2STAbits.URXDA); // The buffer U2RXREG contains data - Can be handled with interrupt
    return (U2RXREG); // Return the FIFO data
}

void UART2_Read_String(char *string, u32 size)
{
    u32 i;
    for (i = 0; i < size; i++)
	string[i] = UART2_Get_Data_Byte();
}

void ft_putstr(char *string)
{
   UART2_Send_String(string, ft_strlen(string));
}
