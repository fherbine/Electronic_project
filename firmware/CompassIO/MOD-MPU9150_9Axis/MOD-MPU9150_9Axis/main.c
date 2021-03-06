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

#define DEBUG_BASE 16

#define MPU9150_USER_CTRL 0x6A
#define I2C_MST_EN (1 << 5)
#define PWR_MGMT_1 0x6B
#define MPU9150_I2C_MST_CTRL 0x24
#define MULT_MST_EN (1 << 7) | (1 << 4)

#define GYRO_CONFIG 0x1B

#define MPU9150_GYRO_XOUT_H 0x43
#define MPU9150_GYRO_XOUT_L 0x44
#define MPU9150_WHO_I_AM 0x75
#define MPU9150_I2C_MST_STATUS 0x36

#define MPU9150_I2C_SLV0_ADDR 0x25
#define I2C_SLV0_RW 0x80
#define MPU9150_I2C_SLV0_REG 0x26
#define MPU9150_I2C_SLV0_CTRL 0x27
#define INT_SLV0_EN 0x80
#define _9BYTES 9
#define MPU9150_INT_PIN_CFG 0x37
#define INT_PIN_CFG 0x37
#define INT_LEVEL (1 << 7)
#define INT_OPEN (1 << 6)
#define LATCH_INT_EN (1 << 5)
#define INT_RD_CLEAR (1 << 4)
#define FSYNC_INT_LEVEL (1 << 3)
#define FSYNC_INT_EN (1 << 2)
#define I2C_BYPASS_EN (1 << 1)

#define MPU9150_I2C_SLV4_ADDR 0x31
#define MPU9150_I2C_SLV4_REG 0x32
#define MPU9150_I2C_SLV4_CTRL 0x34

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

#define MAG_XOUT_L 0x4C
#define MAG_XOUT_H 0x4D
#define MAG_YOUT_L 0x4E
#define MAG_YOUT_H 0x4F
#define MAG_ZOUT_L 0x50
#define MAG_ZOUT_H 0x51

#define MAG_CNTL 0x0A // Control mag module
#define MAG_STATUS 0x02
#define MAG_ASTC 0x0C

s16 GetFullNumber(u8 addr1, u8 addr2)
{
    ft_putstr("0b");
    s8 h = MPU9150_Read(addr1);
    ft_putnbr_base(h, 2);
    delayms(10);
    s8 l = MPU9150_Read(addr2);
    ft_putnbr_base(l, 2);
    ft_putstr(" ");
    return (s16)((h << 8) | l);
}

u16 MagGetFullNumber(u8 addr1, u8 addr2)
{
        ft_putstr("0x");
    u8 h = MAG_Read(addr1);
    ft_putnbr_base(h, 2);
    delayms(100);
    u8 l = MAG_Read(addr2);
        ft_putnbr_base(l, 2);
	ft_putstr(" ");
    return (u16)((h << 8) | l);
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
    ft_putnbr_base(GetFullNumber(GYRO_XOUT_H, GYRO_XOUT_L) / 131.0, DEBUG_BASE);
    UART2_Send_String(" GYRO Y: ", 8);
    ft_putnbr_base(GetFullNumber(GYRO_YOUT_H, GYRO_YOUT_L) / 131.0, DEBUG_BASE);
    UART2_Send_String(" GYRO Z: ", 8);
    ft_putnbr_base(GetFullNumber(GYRO_ZOUT_H, GYRO_ZOUT_L) / 131.0, DEBUG_BASE);
    ft_putstr("\n\r");
    delayms(100);
    //I2C1_Write_Data(MPU9150_ADDR, MPU9150_INT_PIN_CFG, 0x02); //toggle enable data read from magnetometer, no continuous read mode !
    //delayms(100);
//    ft_putstr("\n\r MPU9150_INT_PIN_CFG : ");
//    ft_putbinary(MPU9150_Read(MPU9150_INT_PIN_CFG));
//    delayms(100);
    I2C1_Write_Data(AK8975_ADDR, MAG_CNTL, 0x01);
    UART2_Send_String("MAG X: ", 6);
    ft_putnbr_base(GetFullNumber(MAG_XOUT_H, MAG_XOUT_L), DEBUG_BASE);
    UART2_Send_String(" MAG Y: ", 7);
    ft_putnbr_base(GetFullNumber(MAG_YOUT_H, MAG_YOUT_L), DEBUG_BASE);
    UART2_Send_String(" MAG Z: ", 7);
    ft_putnbr_base(GetFullNumber(MAG_ZOUT_H, MAG_ZOUT_L), DEBUG_BASE);
    ft_putstr("\n\r");
    ft_putbinary(MPU9150_Read(MPU9150_INT_PIN_CFG));
    ft_putstr("\n\r");
    ft_putbinary(MPU9150_Read(0x00));
    ft_putstr("\n\r MPU9150_I2C_MST_STATUS : ");
    ft_putbinary(MPU9150_Read(MPU9150_I2C_MST_STATUS));
    ft_putstr("\n\r MPU9150_INT_PIN_CFG : ");
    ft_putbinary(MPU9150_Read(MPU9150_INT_PIN_CFG));
    ft_putstr("\n\r WHO I AM : ");
    ft_putnbr_base(MPU9150_Read(MPU9150_WHO_I_AM), 16);
    ft_putstr("\n\r");
    ft_putbinary(MPU9150_Read(0x49)); // I2C DATA
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

    ft_putendl("");
    ft_putbinary(MPU9150_Read(0x36)); // I2C MASTER STATUS
    ft_putendl("");
    delayms(100);
    ft_putbinary(MPU9150_Read(0x49)); // I2C DATA
    ft_putendl("");
    delayms(100);
    ft_putbinary(MPU9150_Read(MPU9150_I2C_SLV0_ADDR)); // DEBUG
    ft_putendl("");
    delayms(100);
    ft_putnbr_base(MPU9150_Read(0x54), 16);
    ft_putendl("");
    delayms(100);
    ft_putnbr_base(MPU9150_Read(PWR_MGMT_1), 2); // I2C SLAVE4 DI
    ft_putendl("----");
}

void MPU9150_Init()
{
    ft_putendl("MPU9150 - Reset");
    //I2C1_Write_Data(MPU9150_ADDR, 0x6b, 0x80);
    delayms(500);
    ft_putendl("MPU9150 - Start -> Init");
    //I2C1_Write_Data(MPU9150_ADDR, INT_PIN_CFG, I2C_BYPASS_EN);
    I2C1_Write_Data(MPU9150_ADDR, MPU9150_I2C_MST_CTRL, MULT_MST_EN);
    delayms(100);
    I2C1_Write_Data(MPU9150_ADDR, MPU9150_I2C_SLV0_ADDR, I2C_SLV0_RW | AK8975_ADDR); // Set mag slave address
    delayms(100);
    I2C1_Write_Data(MPU9150_ADDR, MPU9150_I2C_SLV0_REG, 0x00);
    delayms(100);
    I2C1_Write_Data(MPU9150_ADDR, MPU9150_I2C_SLV0_CTRL, INT_SLV0_EN | 11);
    delayms(100);
//    I2C1_Write_Data(MPU9150_ADDR, MPU9150_I2C_SLV4_ADDR, I2C_SLV0_RW | AK8975_ADDR); // Set mag slave address
//    delayms(100);
//    I2C1_Write_Data(MPU9150_ADDR, MPU9150_I2C_SLV4_REG, 0x00);
//    delayms(100);
//    I2C1_Write_Data(MPU9150_ADDR, MPU9150_I2C_SLV4_CTRL, 0b10000000); //INT_SLV0_EN | _6BITS);
//    delayms(100);
    ft_putbinary(MPU9150_Read(MPU9150_I2C_SLV0_ADDR)); // I2C SLAVE4 DI
    ft_putendl("----");
    I2C1_Write_Data(MPU9150_ADDR, MPU9150_USER_CTRL, I2C_MST_EN);
    delayms(100);
    ft_putendl("");
    ft_putbinary(MPU9150_Read(0x36)); // I2C MASTER STATUS
    delayms(100);
    ft_putendl("");
    ft_putbinary(MPU9150_Read(0x49)); // I2C DATA
    ft_putendl("");
    delayms(100);
//    ft_putbinary(MPU9150_Read(MPU9150_I2C_SLV0_ADDR));
//    ft_putendl("");
//    ft_putbinary(MPU9150_Read(MPU9150_I2C_SLV0_REG));
//    ft_putendl("");
//    ft_putbinary(MPU9150_Read(MPU9150_I2C_SLV0_CTRL));
//    ft_putendl("");
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
    I2C1_Write_Data(MPU9150_ADDR, PWR_MGMT_1, 0x0);
    delayms(100);
//    I2C1_Write_Data(MPU9150_ADDR, PWR_MGMT_1, 0x80);
//    delayms(100);
//    I2C1_Write_Data(MPU9150_ADDR, PWR_MGMT_1, 0x0);
//    delayms(100);
    ft_putbinary(MPU9150_Read(PWR_MGMT_1)); // I2C SLAVE4 DI
    ft_putendl("^---- PWR_MGMT_1");
}

void MAG_Init()
{
    //
    ft_putendl("MAG - Start -> Init");
//    I2C1_Write_Data(AK8975_ADDR, 0x0A, I2C_BYPASS_EN); // Activate direct acces to slaves
//    delayms(100);
    I2C1_Write_Data(AK8975_ADDR, MAG_CNTL, 0x01); // Activate slave
//    delayms(100);
//    I2C1_Write_Data(AK8975_ADDR, 0x0A, 0x0); // Desactivate direct acces to slave
//    I2C1_Write_Data(MPU9150_ADDR, MAG_CNTL, 0x0F); // Enter Fuse ROM access mode
//    delayms(100);
//    MAG_Read(ASAX);
//    delayms(100);
//    MAG_Read(ASAY);
//    delayms(100);
//    MAG_Read(ASAZ);
//    delayms(100);
//    I2C1_Write_Data(MPU9150_ADDR, MAG_CNTL, 0x00); // Power down Mag
//    delayms(100);
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
