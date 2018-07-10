/*
 * File:   main.c
 * Author: bocal
 *
 * Created on June 22, 2018, 10:05 AM
 */

#include "types.h"

char buffBT[500];

#define NEWLINE 13

/* UART -> Bluetooth/GPS */
void __ISR(_UART1_VECTOR, IPL1SRS) UART1Handler(void) {
	// Reception
	if (IFS0bits.U1RXIF) {
		IFS0CLR = U1RX_IFS1;
		u32 dest_len = ft_strlen(buffBT);
		buffBT[dest_len] = UART1_Get_Data_Byte();
		UART2_Send_Data_Byte(buffBT[dest_len]);
		buffBT[dest_len + 1] = '\0';
		if (buffBT[dest_len] == NEWLINE) {
			parser_gps_bluetooth(buffBT);
			ft_bzero(buffBT, 500);
		}
//		ft_putnbr_base(buffBT[dest_len], 10);
		LATFbits.LATF1 ^= 1;
	}
	// Transmit
	if (IFS0bits.U1TXIF)
		IFS0CLR = U1TX_IFS1;
	// Error
	if (IFS0bits.U1EIF)
		IFS0CLR = U1E_IFS1;
}

/* UART -> GPS/Debug */
void __ISR(_UART2_VECTOR, IPL1SRS) UART2Handler(void) {
	// Reception
	if (IFS1bits.U2RXIF) {
		IFS1CLR = U2RX_IFS1;
		UART1_Send_Data_Byte(UART2_Get_Data_Byte());
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
	UART1_Init(_8N, 0, UART_RX_TX_ON);
	Init_Delay();
	
	INTCONbits.MVEC = 1; // Enable multi interrupts
	__builtin_enable_interrupts();

	delayms(1000);
	ft_putendl("Start");
	while (1) ;
}