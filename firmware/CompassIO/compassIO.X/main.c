/*
 * File:   main.c
 * Author: bocal
 *
 * Created on June 22, 2018, 10:05 AM
 */

#include "types.h"

/* UART -> Bluetooth/GPS */
void __ISR(_UART2_VECTOR, IPL1) UARTHandler(void) {
	// Reception
	if (IFS1bits.U2RXIF) {
		IFS1CLR = U2RX_IFS1;
		UART2_Send_Data_Byte(UART2_Get_Data_Byte());
		LATFbits.LATF1 ^= 1;
	}
	// Transmit
	if (IFS1bits.U2TXIF)
		IFS1CLR = U2TX_IFS1;
	// Error
	if (IFS1bits.U2EIF)
		IFS1CLR = U2E_IFS1;
}

void main()
{
	LATFbits.LATF1 = 0;
	__builtin_disable_interrupts();
	UART2_Init(_8N, 0, UART_RX_TX_ON);
	Init_Delay();
	
	INTCONbits.MVEC = 1; // Enable multi interrupts
	__builtin_enable_interrupts();

	delayms(1000);
	ft_putendl("Start");
	while (1) ;
}