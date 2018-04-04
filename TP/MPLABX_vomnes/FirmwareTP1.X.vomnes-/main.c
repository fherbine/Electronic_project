#include "types.h"
#include "xc.h"
#include "sys/attribs.h"

int ex1() {
    TRISDbits.TRISD8 = 1;
    TRISFbits.TRISF1 = 0;
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
    return (0);
}

int ex2_1() {
    TRISDbits.TRISD8 = 1;
    TRISFbits.TRISF1 = 0;
    LATFbits.LATF1 = 0;
    u8 val = 1;
    u32 next_val = PERIOD; //0.5hz
    u32 timer = 0;
    while (1) {
        timer++;
        if (timer % (next_val / 2) == 0)
            LATFbits.LATF1 ^= 1;
        if (PORTDbits.RD8 == 0) {
            if (val != 0) {
                val = 0;
                next_val = (next_val / 2 == LIMIT_SPEED) ? PERIOD : (next_val / 2);
            }
        } else {
            val = 1;
        }
    }
    return (0);
}

int ex2_2() {
    TRISDbits.TRISD8 = 1;
    TRISFbits.TRISF1 = 0;
    LATFbits.LATF1 = 0;

    u8 scale = 0b111; // Prescaler 1:256
    u8 released = 1;
    u8 p = 1;
    T2CON = 0; // 0 on every bit, (timer stop, basic config)
    T2CONSET = (scale << 4); // Set scaler
    TMR2 = 0; // Counter to 0
    PR2 = 122 * 16 * p; // Setup the period
    T2CONSET = (1 << 15); // Assigning directly the right bit of TCON itself works aswell.
    while (1) {
        if (IFS0bits.T2IF) // Check for the event triggered by completion of counter
        {
            LATFbits.LATF1 ^= 1; // Toggle
            IFS0bits.T2IF = 0; // Clear register
        }
        if (PORTDbits.RD8 == 0) {
            if (released) {
                released = 0;
                TMR2CLR = 0xFFFF; // Clear timer
                T2CONCLR = (0b1 << 15); // Stop timer
                PR2 /= 2; // Divide period by two, thus increasing frequency,
                if (PR2 == (122 * 16 / 32)) // Until I reach the maximum (8Hz)
                    PR2 = 122 * 16;
                T2CONSET = (0b1 << 15); // Restart timer
            }
        } else
            released = 1;
    }
    return (0);
}

void __ISR(_TIMER_2_VECTOR, ipl7) Timer1Handler(void) {
    if (IFS0bits.T2IF) {
        LATFbits.LATF1 ^= 1; // Toggle
        TMR2 = 0x0;
        IFS0CLR = 0x100; // Clear timer 2
    }
}

int ex3() {
    TRISDbits.TRISD8 = 1;
    TRISFbits.TRISF1 = 0;
    LATFbits.LATF1 = 0;

    T2CON = 0x0; // 0 on every bit, (timer stop, basic config)
    TMR2 = 0x0; // Clean the timer register
    T2CONSET = (0b111 << 4); // Set scaler
    PR2 = 122 * 16 * 1; // Setup the period

    IPC2SET = 0b01100; // Set priority level = 3
    IPC2SET = 0b01;   // Set subpriority level = 1
    // => One operation by assigning PC2SET = 0b01101

    IFS0CLR = 0x100; // Clear the timer 2 interrupt status flag
    IEC0SET = 0x100; // Enable timer 2 interrupts

    T2CONSET = 0x8000; // Start the timer

    while (1) {
        WDTCONSET = 0x01; // This is WDT !
    }
}

int main() {
    ex3();
    return (0);
}
