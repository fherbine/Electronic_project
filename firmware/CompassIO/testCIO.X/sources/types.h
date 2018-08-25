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

