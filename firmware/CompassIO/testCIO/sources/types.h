/* 
 * File:   types.h
 * Author: bocal
 *
 * Created on August 25, 2018, 12:22 PM
 */

#ifndef TYPES_H
#define	TYPES_H

/* includes */
# include <p32xxxx.h>
# include <sys/attribs.h>
# include <xc.h>


/* types typedef */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef char s8;
typedef short s16;
typedef long s32;
typedef double s64;


/*	General defines */
#define TRUE 1
#define FALSE 0

#define INPUT 1
#define OUTPUT 0

#define SET_USER_LED(X) TRISBbits.TRISB2 = X
#define USER_LED_ON(X) LATBbits.LATB2 = X

#define UART_1 1
#define UART_2 0

#define MAIN_DEBUG UART_2

#define _CS1_ON() (LATBbits.LATB15 = 0)
#define _CS1_OFF() (LATBbits.LATB15 = 1)


/*	Clock defines */
#define SYSCLK (8000000/2*20/8) // = 40Mhz
//#define SYSCLK 8000000 // = 80Mhz
#define PBCLK (SYSCLK/1) // = 10 Mhz

/*	UART defines */
#define UART_BAUD_RATE(X) ((PBCLK/X/16)-1)
#define MAX_BUFFER_SIZE 256

#define _9N 0b11 // 9-bit data, no parity
#define _8O 0b10 // 8-bit data, odd parity
#define _8E 0b01 // 8-bit data, even parity
#define _8N 0b00 // 8-bit data, no parity

#define UART_RX_TX_ON 3
#define UART_RX_ON 2
#define UART_TX_ON 1

#define UART1_BR 9600	//Regular 4800
#define UART2_BR 9600	//Regular 9600

/* i2c defines */
#define ACK 0
#define NACK 1
#define Fsck 5000
#define I2C_BAUD_RATE ((PBCLK/2/Fsck)-2)
#define READ_CMD 1
#define WRITE_CMD 0

/* mpu9150 defines */
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

#define MAG_ASAX 0x10
#define MAG_ASAY 0x11
#define MAG_ASAZ 0x12

#define MPU9150_ADDR 0x68
#define AK8975_ADDR 0x0C


/* functions prototypes */
void set_pps(void);

void init_servo(void);
void ServoMotorSetAngle(u8 dutyCycle);

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* TYPES_H */

