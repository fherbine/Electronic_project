/* 
 * File:   main.c
 * Author: bocal
 *
 * Created on May 7, 2018, 5:00 PM
 */

#include "types.h"

/*
 * 
 */

void __ISR(_TIMER_2_VECTOR, IPL3SRS) Timer2Handler(void) {
//    UART2_Send_String("I'm a hacker", sizeof("I'm a hacker"));
//    UART2_Send_String("I'm a hacker", sizeof("I'm a hacker"));
//    UART2_Send_String("I'm a hacker", sizeof("I'm a hacker"));
//    UART2_Send_String("I'm a hacker", sizeof("I'm a hacker"));
//    UART2_Send_String("I'm a hacker", sizeof("I'm a hacker"));
//    UART2_Send_String("\n\r", sizeof("\n\r"));
    IFS0bits.T2IF = 0;   // Reset to 0 Interrupt TIMER2
}
// #define digitalWrite(pin, val) (_TRIS##(pin) = (val))
// digitalWrite(F1, 1);
void delayms(u32 ms) {
    while (ms)
	if (IFS0bits.T3IF == 1) {
	    IFS0bits.T3IF = 0;
	    ms--;
	}
}
void delays(u32 s) {
    while (s)
	if (IFS0bits.T3IF == 1) {
	    IFS0bits.T3IF = 0;
	    s--;
	}
}

void Init_Delay()
{
  __builtin_disable_interrupts();
  T3CON = 0;               // 0 on every bit, (timer stop, basic config)
  TMR3 = 2;                // Clean the timer register
  IFS0bits.T3IF = 0;       // Clear interrupt status flag
  T3CONbits.TCKPS = 0b111; // Set scaler 1:256
  PR3 = PBCLK/256/1000;    // Setup the period
  T3CONbits.ON = 1;
  INTCONbits.MVEC = 1; // Enable multi interrupts
  __builtin_enable_interrupts();
}

void Init_T1()
{
    T2CON = 0;               // 0 on every bit, (timer stop, basic config)
    TMR2 = 0;                // Clean the timer register
    T2CONbits.TCKPS = 0b111; // Set scaler 1:256
    PR2 = PBCLK/256/1;            // Setup the period
}

void Init_T1_Int()
{
   IPC2bits.T2IP = 3; // Set priority
   IPC2bits.T2IS = 0; // Set subpriority
   IFS0bits.T2IF = 0; // Clear interrupt status flag
   IEC0bits.T2IE = 1; // Enable interrupts
}

void UART2_Echo(u8 rcv)
{
    rcv = UART2_Get_Data_Byte();
    if (rcv == '\r') UART2_Send_Data_Byte('\n');
    if (rcv == 127) {
        UART2_Send_Data_Byte(8);
        UART2_Send_Data_Byte(' ');
        UART2_Send_Data_Byte(8);
    }
    else if (rcv == '\\') UART2_Send_String("Hello World!\r\n", 14);
    else UART2_Send_Data_Byte(rcv);
}

void main()
{
    UART2_Init(_8N, 0, UART_RX_TX_ON);
    Init_Delay();
    
//    TRISFbits.TRISF1 = 0; //writable
//    LATFbits.LATF1 = 0;
//    I2C1_Init();
    
//    LATFbits.LATF1 = 1;
//    I2C1_Send_Data( I2C1_Receive_Data(0b1010101), 0b1010101);
//    LATFbits.LATF1 = 0;

    delayms(1000);
    T2CONbits.ON = 1; //start timer at the end
    ft_putendl("Start");
    ft_putbinary(42);
    u8 rcv;
    while (1){
	UART2_Echo(rcv);
    }
}
