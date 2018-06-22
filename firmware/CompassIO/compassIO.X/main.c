/*
 * File:   main.c
 * Author: bocal
 *
 * Created on June 22, 2018, 10:05 AM
 */

#include "types.h"

void __ISR(_UART2_VECTOR, IPL1) UARTHandler(void) {
	if (IFS1bits.U2TXIF) {
		IFS1CLR = U2TX_IFS1;
		IFS1bits.U2TXIF = 0;
	} else if (IFS1bits.U2RXIF) {
		IFS1CLR = U2RX_IFS1;
	}
}

void main()
{
	__builtin_disable_interrupts();
	UART2_Init(_8N, 0, UART_RX_TX_ON);
	Init_Delay();
	
	INTCONbits.MVEC = 1; // Enable multi interrupts
	__builtin_enable_interrupts();

	delayms(1000);
	ft_putendl("Start");
	while (1)
	{
		ft_putendl("Running");
	}
}