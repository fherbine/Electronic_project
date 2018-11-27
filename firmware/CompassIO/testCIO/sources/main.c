#include "types.h"



void __ISR(_EXTERNAL_1_VECTOR, IPL1AUTO) MainButtonHandler(void) {
	IFS0bits.INT1IF = 0;
	LATBbits.LATB2 = 1;
	if (INTCONbits.INT1EP == 0) { // Button Released
			ServoMotorSetAngle(180);
			INTCONbits.INT1EP = 1;

		} else {
		ServoMotorSetAngle(0);
			INTCONbits.INT1EP = 0; // Active button1 release mode
		}
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

u8 charTest = '0';

void Init_T4()
{
    T4CON = 0;               // 0 on every bit, (timer stop, basic config)
    TMR4 = 0;                // Clean the timer register
    T4CONbits.TCKPS = 0b111; // Set scaler 1:256
    PR4 = PBCLK/256/1;       // Setup the period
}

void Init_T4_Int()
{
   IPC4bits.T4IP = 3; // Set priority
   IPC4bits.T4IS = 0; // Set subpriority
   IFS0bits.T4IF = 0; // Clear interrupt status flag
   IEC0bits.T4IE = 1; // Enable interrupts
}

void __ISR(_TIMER_4_VECTOR) Timer2Handler(void) {
    IFS0bits.T4IF = 0; // Reset to 0 Interrupt TIMER2
   // LATFbits.LATF1 ^= 1;
  //  MPU9150GetData();
} //, IPL3SRS

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

	Init_SPI1();
	__builtin_disable_interrupts();
	INTCONbits.MVEC = 1; // Enable multi interrupts
	init_servo();
	UART2_Init(_8N, 0, UART_TX_ON);
	UART1_Init(_8N, 0, UART_RX_TX_ON);
	init_button();

	//init_mag();
    //T3CONbits.ON = 1; //start timer at the end
	__builtin_enable_interrupts();
	//ServoMotorSetAngle(90);
	ft_putendl("start");
	while(42)
	{
		read_id();
		//MAG_Read(0x07);
		/*charTest = UART1_Get_Data_Byte();
		if (charTest != '0')
			UART2_Send_Data_Byte(charTest);*/
		_nop();
	}
}