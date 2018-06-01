/* 
 * File:   main.c
 * Author: bocal
 *
 * Created on May 31, 2018, 2:34 PM
 */

#include "types.h"

#pragma config FPLLIDIV = DIV_2
#pragma config FPLLMUL = MUL_20
#pragma config FPLLODIV = DIV_1

#pragma config FNOSC = PRIPLL
#pragma config POSCMOD = HS

int main()
{
	UART2_Init(0, 1, 0b11);
	UART1_Init(0, 1, 0b11);
	while(1)
		UART2_Send_Data_Byte(UART1_Get_Data_Byte());
}
