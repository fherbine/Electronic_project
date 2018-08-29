/* 
 * File:   types.h
 * Author: bocal
 *
 * Created on August 26, 2018, 5:07 PM
 */

#ifndef TYPES_H
#define	TYPES_H

/* includes */
# include <p32xxxx.h>
# include <sys/attribs.h>
# include <xc.h>
# include <math.h>
//# include <string.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef char s8;
typedef short s16;
typedef long s32;
typedef double s64;

struct s_taskflag {
	u8 Mag;
	u8 CalMag;
	u8 Bluetooth;
	u8 GPS;
	u8 switchPos;
	u8 displayDist;
};

typedef struct	s_coord
{
	u8 completed;
	float lat;
	float lon;
}				t_coord;

struct s_data {
	u32 init_distance;
	u32 current_distance; // --> no flash
	t_coord dest_coord; // BT coord
	t_coord init_coord; // GPS init coord
	t_coord current_coord; // GPS coord --> no flsh
	u8 store_data;
};

/*	General defines */
#define TRUE 1
#define FALSE 0
#define INPUT 1
#define OUTPUT 0

#define SET_USER_LED(X) TRISBbits.TRISB2 = X
#define USER_LED_ON(X) LATBbits.LATB2 = X

#define BITS(X) (1 << X)

/*	Clock defines */
#define SYSCLK (8000000/2*20/8) // = 10Mhz
#define PBCLK (SYSCLK/1) // = 10 Mhz

/* UART defines */
#define UART1_BR 9600	//Regular 4800
#define UART2_BR 4800	//Regular 9600bonjour bonjour je suis un canard
#define UART_1 1
#define UART_2 0
#define MAIN_DEBUG UART_1

#define UART_BAUD_RATE(X) ((PBCLK/X/16)-1)
#define MAX_BUFFER_SIZE 256

#define _9N 0b11 // 9-bit data, no parity
#define _8O 0b10 // 8-bit data, odd parity
#define _8E 0b01 // 8-bit data, even parity
#define _8N 0b00 // 8-bit data, no parity

#define UART_RX_TX_ON 3
#define UART_RX_ON 2
#define UART_TX_ON 1

#define U1E_IFS1 BITS(7)
#define U1RX_IFS1 BITS(8)
#define U1TX_IFS1 BITS(9)

#define U2E_IFS1 BITS(21)
#define U2RX_IFS1 BITS(22)
#define U2TX_IFS1 BITS(23)

/* Flash defines */
#define _CS1_ON() (LATBbits.LATB15 = 0)
#define _CS1_OFF() (LATBbits.LATB15 = 1)
#define _ENABLE_CS1() (TRISBbits.TRISB15 = OUTPUT)

#define FM_JEDECID 0x9F
#define FM_ID 0xAB
#define FM_WRITE_ENABLE 0x06
#define FM_WRITE_DISABLE 0x04
#define FM_PAGE_PROGRAM 0x02
#define FM_READ 0x03
#define FM_STATUS_REGISTER_READ 0x05
#define FM_ERASE_SMALL_SECTOR 0x20
#define FM_ERASE_SECTOR 0xD8

#define STORE_MAG_X 0x0
#define STORE_MAG_Y 0x010000
#define STORE_DEST_LAT_X1000 0x020000
#define STORE_DEST_LONG_X1000 0x030000

/* I2C defines */
#define MPU9150_ADDR 0x68
#define AK8975_ADDR 0x0C
#define READ_CMD 1
#define WRITE_CMD 0

#define Fsck 5000
#define I2C_BAUD_RATE ((PBCLK/2/Fsck)-2)
#define ACK 0
#define NACK 1

/* Magnetometer defines */
#define MAG_ADDR 0x0E
#define MAG_X_H 0x01
#define MAG_X_L 0x02
#define MAG_Y_H 0x03
#define MAG_Y_L 0x04
#define MAG_Z_H 0x05
#define MAG_Z_L 0x06
#define MAG_WHO_AM_I 0x07
#define MAG_CTRL_REG1 0x10
#define MAG_CTRL_REG2 0x11

/*  GPS  */
#define NMEA_GPRMC_LATITUDE 2
#define NMEA_GPRMC_LONGITUDE 4

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* TYPES_H */

