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
#include "math.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef char s8;
typedef short s16;
typedef long s32;
typedef double s64;

#include "export.h"

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

#define _CS1_ON() (PORTDbits.RD5 = 0)
#define _CS1_OFF() (PORTDbits.RD5 = 1)


#define FM_JEDECID 0x9F
#define FM_ID 0xAB
#define FM_WRITE_ENABLE 0x06
#define FM_WRITE_DISABLE 0x04
#define FM_PAGE_PROGRAM 0x02
#define FM_READ 0x03
#define FM_STATUS_REGISTER_READ 0x05
#define FM_ERASE_SECTOR 0xD8

#define TIMER4_100MS 10
#define TIMER4_500MS 2

#define ONE_HALF_SEC 1500
#define FIVE_SEC 5000

void    ft_putnbr_base(s32 nb, int base);
void    ft_putbinary(u32 nb);
void    ft_putendl(char *string);
void    Init_SPI2();
void    Handle_SPI(u8 input, u8 *output);
u8      SPI2_Read();
void    delayms(u32 ms);
s32     read_data(u32 addr, u32 size);
void    store_double(u32 addr, s64 data);
void    store_int(u32 addr, u32 data);

#endif	/* TYPES_H */