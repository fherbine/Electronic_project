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
  T3CON = 0;               // 0 on every bit, (timer stop, basic config)
  TMR3 = 2;                // Clean the timer register
  IFS0bits.T3IF = 0;       // Clear interrupt status flag
  T3CONbits.TCKPS = 0b111; // Set scaler 1:256
  PR3 = PBCLK/256/1000;    // Setup the period
  T3CONbits.ON = 1;
}

void Init_T2()
{
    T2CON = 0;               // 0 on every bit, (timer stop, basic config)
    TMR2 = 0;                // Clean the timer register
    T2CONbits.TCKPS = 0b111; // Set scaler 1:256
    PR2 = PBCLK/256/1;       // Setup the period
}

void Init_T2_Int()
{
   IPC2bits.T2IP = 3; // Set priority
   IPC2bits.T2IS = 0; // Set subpriority
   IFS0bits.T2IF = 0; // Clear interrupt status flag
   IEC0bits.T2IE = 1; // Enable interrupts
}

void UART2_Echo()
{
    u8 rcv = UART2_Get_Data_Byte();
    if (rcv == '\r') UART2_Send_Data_Byte('\n');
    if (rcv == 127) {
        UART2_Send_Data_Byte(8);
        UART2_Send_Data_Byte(' ');
        UART2_Send_Data_Byte(8);
    }
    else if (rcv == '\\') UART2_Send_String("Hello World!\r\n", 14);
    else UART2_Send_Data_Byte(rcv);
}

#define IFS0_T2 BITS(8)

void __ISR(_TIMER_2_VECTOR) Timer2Handler(void) {
    emmit_SPI();
    LATFbits.LATF1 ^= 1;
    IFS0CLR = IFS0_T2; // Reset to 0 Interrupt TIMER2
} //, IPL3SRS

void emmit_SPI(void)
{
    ft_putstr(">>");
    ft_putnbr_base(SPI2_Write_Data_Ready(), 10);
    ft_putendl("");
    if (SPI2_Write_Data_Ready())
    {
        ft_putendl("here");
	SPI2_Write(1);
        ft_putendl("there");
    }
}

void receipt_SPI(void)
{
    ft_putendl("Start");
    u8 data = 0;
    while (1)
    {
        if (SPI2_Read_Data_Ready())
            ft_putendl("Datas received");
        data = SPI2_Read();

        ft_putstr("BUFFER STATUS (READ) >> ");
        ft_putbinary(SPI2BUF);
        ft_putendl("");
        if (data != 0xff && data != 0)
        {
            ft_putstr("==> ");
            UART2_Send_Data_Byte(data);
        }
        data = 0;
    }
}

void main()
{
    __builtin_disable_interrupts();
    UART2_Init(_8N, 0, UART_RX_TX_ON);
    Init_Delay();
    Init_T2();
    
    TRISFbits.TRISF1 = 0; //writable
//    LATFbits.LATF1 = 0;
    
    LATFbits.LATF1 ^= 1;
//    LATFbits.LATF1 = 0;

    Init_T2_Int();
    delayms(100);

    /* MPU9150 */
    //I2C1_Init();
    //MAG_Init();
    //MPU9150_Init();
    //MPU9150_On();
    /* ======= */

    /* SPI */
    SPI2_Init();


    delayms(1000);
    T2CONbits.ON = 1; //start timer at the end
    INTCONbits.MVEC = 1; // Enable multi interrupts
    __builtin_enable_interrupts();
    //ft_putbinary(255);

    //while (1) ;
    receipt_SPI();
}
