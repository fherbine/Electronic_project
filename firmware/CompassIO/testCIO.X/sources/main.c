#include "types.h"



void __ISR(_EXTERNAL_1_VECTOR, IPL1AUTO) MainButtonHandler(void) {
	IFS0bits.INT1IF = 0;
	LATBbits.LATB2 = 1;
	ServoMotorSetAngle(180);
//	if (INTCONbits.INT1EP == 0) { // Button Released
//
//			INTCONbits.INT1EP = 1;
//
//		} else {
//			INTCONbits.INT1EP = 0; // Active button1 release mode
//		}
}

void __ISR(_TIMER_2_VECTOR, IPL1AUTO) t2Handler(void) {
	IFS0bits.T2IF = 0;     // Clear interrupt status flag
	//LATBbits.LATB7 ^= 1;
	_nop();
}

u8 test = FALSE;

void __ISR(_UART1_VECTOR, IPL1) UART1Handler(void) {
	// Reception
	//LATBbits.LATB2 ^= 1;
	if (IFS1bits.U1RXIF) {
		IFS1bits.U1RXIF = 0;
		test = TRUE;
	}
	// Transmit
	if (IFS1bits.U1TXIF)
		IFS1bits.U1TXIF = 0;
	// Error
	if (IFS1bits.U1EIF)
		IFS1bits.U1EIF = 0;
}

void init_button(void)
{
	INTCONbits.INT1EP = 1; //1->lorsqu'on entre, 0 lorsqu'on sort l'interrupt se produit*/
//	// INT1 - Button
	IPC1bits.INT1IP = 1;
	IPC1bits.INT1IS = 0;
	IFS0bits.INT1IF = 0;
	IEC0bits.INT1IE = 1;
}

void main()
{
//	TRISBbits.TRISB0 = 0;
//	LATBbits.LATB0 = 0;

	TRISBbits.TRISB2 = 0;
	//CFGCONbits.IOLOCK = 0;
	//DEVCFG3bits.IOL1WAY = 1; // only one reconfiguration

	//CFGCONbits.IOLOCK = 1;
	set_pps();
	LATBbits.LATB2 = 0;
//
	__builtin_disable_interrupts();
	INTCONbits.MVEC = 1; // Enable multi interrupts
	init_servo();
	UART2_Init(_8N, 0, UART_TX_ON);
	UART1_Init(_8N, 0, UART_RX_TX_ON);
	init_button();
	__builtin_enable_interrupts();
	//ServoMotorSetAngle(90);
	while(42)
	{
		if (test == TRUE)
		{
			LATBbits.LATB2 ^= 1;
			UART2_Send_Data_Byte(UART1_Get_Data_Byte());
			test = FALSE;
		}
		_nop();
	}
}