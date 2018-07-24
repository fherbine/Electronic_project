/*
 * File:   uart.c
 * Author: vomnes
 *
 * Created on May 9, 2018, 11:00 AM
 */

#include "types.h"

/* UART1 */
void UART1_Int()
{
	/* Set priorities */
	IPC6bits.U1IP = 1;
	IPC6bits.U1IS = 0;
	/* Clear Interrupt flags */
	IFS0bits.U1EIF = 0;
	IFS0bits.U1RXIF = 0;
	IFS0bits.U1TXIF = 0;
	/* Enable interrupts */
	IEC0bits.U1EIE = 1;
	IEC0bits.U1RXIE = 1;
	IEC0bits.U1TXIE = 1;

	/* Enable transmit/reception interrupt */
	U1STAbits.UTXISEL = 1;
	U1STAbits.URXISEL = 1;
}

void UART1_Init(u8 parityDataBits, u8 stopBits, u8 TRX_Mode)
{
    if (!TRX_Mode) {
	return;
    }
    U1BRG = UART_BAUD_RATE;
    U1MODEbits.PDSEL = parityDataBits;
    U1MODEbits.STSEL = stopBits;
    UART1_Int();
    U1STAbits.URXEN = TRX_Mode != 1; // Enable reception
    U1MODEbits.ON = 1; // Enable UART1 Module
    U1STAbits.UTXEN = TRX_Mode & 1; // Enable transmission
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
    return (U1RXREG); // Return the FIFO data
}

void UART1_Read_String(char *string, u32 size)
{
    u32 i;
    for (i = 0; i < size; i++)
	string[i] = UART1_Get_Data_Byte();
}

/* UART2 */
void UART2_Int()
{
	/* Set priorities */
	IPC8bits.U2IP = 1;
	IPC8bits.U2IS = 0;
	/* Clear Interrupt flags */
	IFS1bits.U2EIF = 0;
	IFS1bits.U2RXIF = 0;
	IFS1bits.U2TXIF = 0;
	/* Enable interrupts */
	IEC1bits.U2EIE = 1;
	IEC1bits.U2RXIE = 1;
	IEC1bits.U2TXIE = 1;

	/* Enable transmit/reception interrupt */
	U2STAbits.UTXISEL = 1;
	U2STAbits.URXISEL = 1;
}

void UART2_Init(u8 parityDataBits, u8 stopBits, u8 TRX_Mode)
{
    if (!TRX_Mode) {
	return;
    }
	//U2MODEbits.BRGH = 1; // Activate for 38400 BR
    U2BRG = UART_BAUD_RATE;
    U2MODEbits.PDSEL = parityDataBits;
    U2MODEbits.STSEL = stopBits;
	UART2_Int();
    U2STAbits.URXEN = TRX_Mode != 1; // Enable reception
    U2MODEbits.ON = 1; // Enable UART2 Module
    U2STAbits.UTXEN = TRX_Mode & 1; // Enable transmission
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
    return (U2RXREG); // Return the FIFO data
}

void UART2_Read_String(char *string, u32 size)
{
    u32 i;
    for (i = 0; i < size; i++)
	string[i] = UART2_Get_Data_Byte();
}