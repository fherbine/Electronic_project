/*
 * File:   types.h
 * Author: bocal
 *
 * Created on March 22, 2018, 2:05 PM
 */

#ifndef TYPES_H
#define	TYPES_H
#include <p32xxxx.h>
#include "sys/attribs.h"
#include "xc.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef char s8;
typedef short s16;
typedef long s32;

#define PERIOD 32000
#define LIMIT_SPEED 1250
#define TRUE 1
#define FALSE 0

#define MPU9150_ADDR 0x68
#define AK8975_ADDR 0x0C
#define READ_CMD 1
#define WRITE_CMD 0

#define SYSCLK (8000000/2*20/1) // 80Mhz
#define PBCLK (SYSCLK/8) // 10Mhz
#define Fsck 5000
#define I2C_BAUD_RATE ((PBCLK/2/Fsck)-2)

/* UART */
/* Parity and data slection bits */
#define _9N 0b11 // 9-bit data, no parity
#define _8O 0b10 // 8-bit data, odd parity
#define _8E 0b01 // 8-bit data, even parity
#define _8N 0b00 // 8-bit data, no parity

#define UART_RX_TX_ON 3
#define UART_RX_ON 2
#define UART_TX_ON 1

#define MAX_BUFFER_SIZE 256

#define UART_BAUD_RATE ((PBCLK/4800/16)-1)

#define BITS(X) (1 << X)

#define U1E_IFS1 BITS(26)
#define U1E_IEC1 BITS(26)
#define U1RX_IFS1 BITS(27)
#define U1RX_IEC1 BITS(27)
#define U1TX_IFS1 BITS(28)
#define U1TX_IEC1 BITS(28)

#define U2E_IFS1 BITS(8)
#define U2E_IEC1 BITS(8)
#define U2RX_IFS1 BITS(9)
#define U2RX_IEC1 BITS(9)
#define U2TX_IFS1 BITS(10)
#define U2TX_IEC1 BITS(10)

typedef struct	s_coord
{
	double		lat;
	double		lon;
}				t_coord;

#endif	/* TYPES_H */
