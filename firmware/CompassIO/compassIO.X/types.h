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

#define SYSCLK (8000000/2*20/1) // = 80Mhz
#define PBCLK (SYSCLK/8) // = 10 Mhz
#define Fsck 5000
#define I2C_BAUD_RATE ((PBCLK/2/Fsck)-2)

/* UART */

#define U2E_IFS1 BITS(8)
#define U2E_IEC1 BITS(8)
#define U2RX_IFS1 BITS(9)
#define U2RX_IEC1 BITS(9)
#define U2TX_IFS1 BITS(10)
#define U2TX_IEC1 BITS(10)

/* Parity and data slection bits */
#define _9N 0b11 // 9-bit data, no parity
#define _8O 0b10 // 8-bit data, odd parity
#define _8E 0b01 // 8-bit data, even parity
#define _8N 0b00 // 8-bit data, no parity

#define UART_RX_TX_ON 3
#define UART_RX_ON 2
#define UART_TX_ON 1

#define MAX_BUFFER_SIZE 256

#define UART_BAUD_RATE ((PBCLK/9600/16)-1)

#define BITS(X) (1 << X)

#define SPI_BAUD_RATE 1

/* SPISTAT */
#define SPIROV_BIT BITS(6)

/* SPICON */
#define ON_BIT BITS(15)
#define SMP_BIT BITS(9)
#define MSTEN_BIT BITS(5)

#define IEC0_SPI1E BITS(23)
#define IEC0_SPI1TX BITS(24)
#define IEC0_SPI1RX BITS(25)
#define IEC1_SPI2E BITS(5)
#define IEC1_SPI2TX BITS(6)
#define IEC1_SPI2RX BITS(7)
#define MASTER (ON_BIT | MSTEN_BIT)
#define SLAVE ON_BIT

u32     ft_strlen(const char *s);
void    ft_putnbr_base(s32 nb, int base);
void    ft_putbinary(u32 nb);
void    ft_putendl(char *string);

#endif	/* TYPES_H */