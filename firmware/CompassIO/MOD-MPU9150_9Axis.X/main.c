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

u8 MPU9150_Read(u8 addr)
{
    I2C1_Send_Data(addr, MPU9150_ADDR);
    return I2C1_Receive_Data(MPU9150_ADDR);
}

u8 MAG_Read(u8 addr)
{
    I2C1_Send_Data(addr, AK8975_ADDR);
    delayms(100);
    return I2C1_Receive_Data(AK8975_ADDR);
}

#define PWR_MGMT_1 0x6B

#define GYRO_CONFIG 0x1B

#define MPU9150_GYRO_XOUT_H 0x43
#define MPU9150_GYRO_XOUT_L 0x44
#define MPU9150_WHO_I_AM 0x75
#define MPU9150_I2C_MST_STATUS 0x36
#define MPU9150_INT_PIN_CFG 0x37

#define ACCEL_XOUT_H 0x3B // [15:8]
#define ACCEL_XOUT_L 0x3C // [7:0]
#define ACCEL_YOUT_H 0x3D // [15:8]
#define ACCEL_YOUT_L 0x3E // [7:0]
#define ACCEL_ZOUT_H 0x3F // [15:8]
#define ACCEL_ZOUT_L 0x40 // [7:0]

#define GYRO_XOUT_H 0x43 // [15:8]
#define GYRO_XOUT_L 0x44 // [7:0]
#define GYRO_YOUT_H 0x45 // [15:8]
#define GYRO_YOUT_L 0x46 // [7:0]
#define GYRO_ZOUT_H 0x47 // [15:8]
#define GYRO_ZOUT_L 0x48 // [7:0]

#define MAG_XOUT_L 0x03
#define MAG_XOUT_H 0x04
#define MAG_YOUT_L 0x05
#define MAG_YOUT_H 0x06
#define MAG_ZOUT_L 0x07
#define MAG_ZOUT_H 0x08

#define MAG_CNTL 0x0A // Control mag module
#define MAG_STATUS 0x02
#define MAG_ASTC 0x0C

s16 GetFullNumber(u8 addr1, u8 addr2)
{
    s8 h = MPU9150_Read(addr1);
//    ft_putnbr_base(h, 2);
    delayms(10);
    s8 l = MPU9150_Read(addr2);
//    ft_putnbr_base(l, 2);
    return (s16)((h << 8) | l);
}

s16 MagGetFullNumber(u8 addr1, u8 addr2)
{
    s8 h = MAG_Read(addr1);
    delayms(100);
    ft_putnbr_base(h, 2);
    ft_putstr("-");
    s8 l = MAG_Read(addr2);
    ft_putnbr_base(l, 2);
    ft_putendl("");
    return (s16)((h << 8) | l);
}

void GetData()
{
    ft_putstr("\033[H\033[2J");
    UART2_Send_String("ACCEL X: ", 9);
    s16 accel_x = GetFullNumber(ACCEL_XOUT_H, ACCEL_XOUT_L) / 16384.0 * 100;
    ft_putnbr_base(accel_x, 10);
    UART2_Send_String(" ACCEL Y: ", 10);
    s16 accel_y = GetFullNumber(ACCEL_YOUT_H, ACCEL_YOUT_L) / 16384.0 * 100;
    ft_putnbr_base(accel_y, 10);
    UART2_Send_String(" ACCEL Z: ", 10);
    s16 accel_z = GetFullNumber(ACCEL_ZOUT_H, ACCEL_ZOUT_L) / 16384.0 * 100;
    ft_putnbr_base(accel_z, 10);
    ft_putstr("\n\r");
    if (accel_x > 75 || accel_y > 75 || accel_z > 75)
    {
	ft_putendl("Module reversed");
    }
    UART2_Send_String("GYRO X: ", 7);
    ft_putnbr_base(GetFullNumber(GYRO_XOUT_H, GYRO_XOUT_L) / 131.0, 10);
    UART2_Send_String(" GYRO Y: ", 8);
    ft_putnbr_base(GetFullNumber(GYRO_YOUT_H, GYRO_YOUT_L) / 131.0, 10);
    UART2_Send_String(" GYRO Z: ", 8);
    ft_putnbr_base(GetFullNumber(GYRO_ZOUT_H, GYRO_ZOUT_L) / 131.0, 10);
    ft_putstr("\n\r");
    delayms(100);
    //I2C1_Write_Data(MPU9150_ADDR, MPU9150_INT_PIN_CFG, 0x02); //toggle enable data read from magnetometer, no continuous read mode !
    //delayms(100);
//    ft_putstr("\n\r MPU9150_INT_PIN_CFG : ");
//    ft_putbinary(MPU9150_Read(MPU9150_INT_PIN_CFG));
//    delayms(100);
    UART2_Send_String("MAG X: ", 6);
    ft_putnbr_base(MagGetFullNumber(MAG_XOUT_H, MAG_XOUT_L), 10);
    UART2_Send_String(" MAG Y: ", 7);
    ft_putnbr_base(MagGetFullNumber(MAG_YOUT_H, MAG_YOUT_L), 10);
    UART2_Send_String(" MAG Z: ", 7);
    ft_putnbr_base(MagGetFullNumber(MAG_ZOUT_H, MAG_ZOUT_L), 10);
    ft_putstr("\n\r");
    ft_putbinary(MPU9150_Read(MPU9150_INT_PIN_CFG));
    ft_putstr("\n\r");
    ft_putbinary(MPU9150_Read(0x00));
    ft_putstr("\n\r MPU9150_I2C_MST_STATUS : ");
    ft_putbinary(MPU9150_Read(MPU9150_I2C_MST_STATUS));
    ft_putstr("\n\r MPU9150_INT_PIN_CFG : ");
    ft_putbinary(MPU9150_Read(MPU9150_INT_PIN_CFG));
    ft_putstr("\n\r");
}

void __ISR(_TIMER_2_VECTOR, IPL3SRS) Timer2Handler(void) {
//    UART2_Send_String("Read -> ", 8);
//    MPU9150_Read(MPU9150_GYRO_XOUT_H);
//    UART2_Send_String("PWR_MGMT_1: ", 12);
//    MPU9150_Read(PWR_MGMT_1);
//    UART2_Send_String("Who I am: ", 10);
//    MPU9150_Read(MPU9150_WHO_I_AM);
    GetData();
    IFS0bits.T2IF = 0;   // Reset to 0 Interrupt TIMER2
}



void MPU9150_Init()
{
    ft_putendl("MPU9150 - Start -> Init");
    I2C1_Write_Data(MPU9150_ADDR, 0x24, 0x40);
    delayms(100);
    I2C1_Write_Data(MPU9150_ADDR, 0x25, 0x8C);
    delayms(100);
    I2C1_Write_Data(MPU9150_ADDR, 0x26, 0x02);
    delayms(100);
    I2C1_Write_Data(MPU9150_ADDR, 0x27, 0x88);
    delayms(100);
    I2C1_Write_Data(MPU9150_ADDR, 0x28, 0x0C);
    delayms(100);
    I2C1_Write_Data(MPU9150_ADDR, 0x29, 0x0A);
    delayms(100);
    I2C1_Write_Data(MPU9150_ADDR, 0x2A, 0x81);
    delayms(100);
    I2C1_Write_Data(MPU9150_ADDR, 0x64, 0x01);
    delayms(100);
    I2C1_Write_Data(MPU9150_ADDR, 0x67, 0x03);
    delayms(100);
    I2C1_Write_Data(MPU9150_ADDR, 0x01, 0x80);
    delayms(100);

    I2C1_Write_Data(MPU9150_ADDR, 0x34, 0x04);
    delayms(100);
    I2C1_Write_Data(MPU9150_ADDR, 0x64, 0x00);
    delayms(100);
    I2C1_Write_Data(MPU9150_ADDR, 0x6A, 0x00);
    delayms(100);
    I2C1_Write_Data(MPU9150_ADDR, 0x64, 0x01);
    delayms(100);
    I2C1_Write_Data(MPU9150_ADDR, 0x6A, 0x20);
    delayms(100);
    I2C1_Write_Data(MPU9150_ADDR, 0x34, 0x13);
    delayms(100);
    ft_putendl("Stop -> Init");
    delayms(100);
}
//    ft_putbinary(MPU9150_Read(PWR_MGMT_1));
//    delayms(100);

#define ASAX 0x10
#define ASAY 0x11
#define ASAZ 0x12

void MPU9150_On()
{
    I2C1_Write_Data(MPU9150_ADDR, PWR_MGMT_1, 0x00);
    delayms(100);
}

void MAG_Init()
{
    ft_putendl("MAG - Start -> Init");
    I2C1_Write_Data(AK8975_ADDR, MAG_CNTL, 0x00); // Power down Mag
    delayms(100);
    I2C1_Write_Data(AK8975_ADDR, MAG_CNTL, 0x0F); // Enter Fuse ROM access mode
    delayms(100);
//    MAG_Read(ASAX);
//    delayms(100);
//    MAG_Read(ASAY);
//    delayms(100);
//    MAG_Read(ASAZ);
//    delayms(100);
    I2C1_Write_Data(AK8975_ADDR, MAG_CNTL, 0x00); // Power down Mag
    delayms(100);
    ft_putendl("Stop -> Init");
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

    MAG_Init();
    MPU9150_Init();
    MPU9150_On();
    
    delayms(1000);
    T2CONbits.ON = 1; //start timer at the end
    INTCONbits.MVEC = 1; // Enable multi interrupts
    __builtin_enable_interrupts();
    ft_putendl("Start");
    //ft_putbinary(255);

    while (1) {
    }
}
