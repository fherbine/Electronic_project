/* 
 * File:   main.c
 * Author: bocal
 *
 * Created on May 7, 2018, 5:00 PM
 */

#include "header.h"

/*
 * 
 */
int main(int argc, char** argv)
{
    TRISDbits.TRISD8 = 1; //1 == readeable
    TRISFbits.TRISF1 = 0; //writable
    LATFbits.LATF1 = 0;
    u8 val = 1;
    while (1) {
        if (PORTDbits.RD8 == 0) {
            if (val) {
                val = 0;
                LATFbits.LATF1 ^= 1;
            }
        } else {
            val = 1;
        }
    }
    return (EXIT_SUCCESS);
}

