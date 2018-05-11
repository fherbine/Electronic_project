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


void MPU9150_Write(u8 addr, u8 data)
{
    I2C1_Send_Data(addr, MPU9150_ADDR);
    I2C1_Send_Data(data, MPU9150_ADDR);
}

void MPU9150_Read(u8 addr)
{
    I2C1_Send_Data(addr, MPU9150_ADDR);
    I2C1_Receive_Data(MPU9150_ADDR);
}

#define PWR_MGMT_1 0x6B

#define GYRO_CONFIG 0x1B

#define MPU9150_GYRO_XOUT_H 0x43
#define MPU9150_GYRO_XOUT_L 0x44
#define MPU9150_WHO_I_AM 0x75

void __ISR(_TIMER_2_VECTOR, IPL3SRS) Timer2Handler(void) {
//    UART2_Send_String("Read -> ", 8);
//    ft_putnbr_base(PWR_MGMT_1, 16);
//    MPU9150_Read(PWR_MGMT_1);
//    UART2_Send_String("Who I am: ", 10);
//    MPU9150_Read(MPU9150_WHO_I_AM);
    IFS0bits.T2IF = 0;   // Reset to 0 Interrupt TIMER2
}

void MPU9150_Init()
{
    ft_putendl("Init");
//    MPU9150_Read(PWR_MGMT_1);
//    MPU9150_Write(0x24, 0x40);
//    delayms(100);
//    MPU9150_Write(PWR_MGMT_1, 0b00000000); //0b00100000
//    delayms(100);
//    MPU9150_Write(0x19, 0x04);
//    delayms(100);
//    MPU9150_Write(0x1A, 0x03);
//    delayms(100);
//    MPU9150_Write(0x6A, 0x01);
//    delayms(100);
//    MPU9150_Write(0x1B, 0x18);
//    delayms(100);
//    MPU9150_Read(PWR_MGMT_1);
    MPU9150_Read(0x1C);
    MPU9150_Write(0x1C, 0x08);
    MPU9150_Read(0x1C);
    delayms(100);
}

void main()
{
    __builtin_disable_interrupts();
    UART2_Init(_8N, 0, UART_RX_TX_ON);
    Init_Delay();
    Init_T2();
    
//    TRISFbits.TRISF1 = 0; //writable
//    LATFbits.LATF1 = 0;
    I2C1_Init();
    
//    LATFbits.LATF1 = 1;
//    LATFbits.LATF1 = 0;

    Init_T2_Int();

    MPU9150_Init();
    
    delayms(1000);
    T2CONbits.ON = 1; //start timer at the end
    INTCONbits.MVEC = 1; // Enable multi interrupts
    __builtin_enable_interrupts();
    ft_putendl("Start");
    //ft_putbinary(255);

    while (1) {
    }
}
