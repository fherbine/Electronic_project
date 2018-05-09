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
void main()
{
//    TRISFbits.TRISF1 = 0; //writable
//    LATFbits.LATF1 = 0;
//    I2C1_Init();
    
//    LATFbits.LATF1 = 1;
//    I2C1_Send_Data( I2C1_Receive_Data(0b1010101), 0b1010101);
//    LATFbits.LATF1 = 0;
    UART1_Init(_8N, 0, UART_RX_TX_ON);
    UART1_Send_String("I'm a hacker", sizeof("I'm a hacker"));
    char c;
    while (1) {
	UART1_Read_String(&c, 1);
	UART1_Send_String(c, 1);
    }
    while(1) ;
}
