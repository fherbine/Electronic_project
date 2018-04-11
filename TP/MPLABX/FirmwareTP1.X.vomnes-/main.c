#include "types.h"
#include "xc.h"
#include "sys/attribs.h"

int ex1() {
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
    PR2 = 0;
    PR2 = 122 * 16 * p; // Setup the period

    T2CONSET = (1 << 15); // Assigning directly the right bit of TCON itself works aswell.
    //TOUS LES SCALES TMR sincremente
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

//void __ISR(_TIMER_2_VECTOR, IPL3) Timer2Handler(void) {
//    LATFbits.LATF1 ^= 1; // Switch ON/OFF LED
//    TMR2 = 0;            // Reset TIMER2
//    IFS0bits.T2IF = 0;   // Reset to 0 Interrupt TIMER2
//}
//
//void __ISR(_EXTERNAL_1_VECTOR, IPL7) Int1Handler(void) {
//    T2CONbits.ON = 0;         // STOP Timer
//    TMR2CLR = 0xFFFF;         // Clear timer
//    if (PR2 == (PERIOD / 32)) // Until I reach the maximum (8Hz)
//        PR2 = PERIOD;
//    else
//        PR2 /= 2;        // Divide period by two, thus increasing frequency,
//    T2CONbits.ON = 1;    // START Timer
//    IFS0bits.INT1IF = 0; // Reset to 0 Interrupt INT0
//}

int ex3() {
    TRISDbits.TRISD8 = 1;
    TRISFbits.TRISF1 = 0;
    LATFbits.LATF1 = 0;

    T2CON = 0;               // 0 on every bit, (timer stop, basic config)
    TMR2 = 0;                // Clean the timer register
    T2CONbits.TCKPS = 0b101; // Set scaler 1:32
    PR2 = PERIOD;            // Setup the period

    // LED
    IPC2bits.T2IP = 3; // Set priority
    IPC2bits.T2IS = 0; // Set subpriority
    IFS0bits.T2IF = 0; // Clear interrupt status flag
    IEC0bits.T2IE = 1; // Enable interrupts

    // INT1
    IPC1bits.INT1IP = 2;
    IPC1bits.INT1IS = 0;
    IFS0bits.INT1IF = 0;
    IEC0bits.INT1IE = 1;

    T2CONbits.ON = 1; //start timer at the end

    __builtin_enable_interrupts();

    INTCONbits.MVEC = 1; // Enable multi interrupts

    WDTCONbits.ON = 1; // Enable Watchdog timer: safety reset
    while (1) {
     WDTCONbits.WDTCLR = 1; // This is WDT !
    }
}

//void __ISR(_EXTERNAL_1_VECTOR, IPL7) Int1Handler(void) {
//    T2CONbits.ON = 0;         // STOP Timer
//    TMR2CLR = 0xFFFF;         // Clear timer
//    if (PR2 == (PERIOD / 32)) // Until I reach the maximum (8Hz)
//        PR2 = PERIOD;
//    else
//        PR2 /= 2;        // Divide period by two, thus increasing frequency,
//    T2CONbits.ON = 1;    // START Timer
//    IFS0bits.INT1IF = 0; // Reset to 0 Interrupt INT0
//}

void __ISR(_TIMER_2_VECTOR, IPL3) Timer2Handler(void) {
    if (T3CONbits.ON == 0) {
        T3CONbits.ON == 1;
        while (IFS0bits.T3IF == 0){
            LATFbits.LATF1 = 1; // Switch ON/OFF LED
            if (TMR3 > 0) {
                break;
            }
        }
        IFS0bits.T3IF == 0;
        LATFbits.LATF1 = 0;
        TMR3 = 0;
        T3CONbits.ON == 0;
    }
    TMR2 = 0;            // Reset TIMER2
    IFS0bits.T2IF = 0;   // Reset to 0 Interrupt TIMER2
}

//void __ISR(_TIMER_3_VECTOR, IPL3) Timer3Handler(void) {
//    LATFbits.LATF1 ^= 1; // Switch ON/OFF LED
//    TMR2 = 0;            // Reset TIMER2
//    IFS0bits.T2IF = 0;   // Reset to 0 Interrupt TIMER2
//}

void ex4() {
    TRISDbits.TRISD8 = 1;
    TRISFbits.TRISF1 = 0;
    LATFbits.LATF1 = 0;

    // Timer 2
    T2CON = 0;               // 0 on every bit, (timer stop, basic config)
    TMR2 = 0;                // Clean the timer register
    T2CONbits.TCKPS = 0b101; // Set scaler 1:32
    PR2 = PERIOD;            // Setup the period

    // Timer 3
    T3CON = 0;
    TMR3 = 0;
    T3CONbits.TCKPS = 0b101;
    PR3 = PERIOD;

    // T2
    IPC2bits.T2IP = 3; // Set priority
    IPC2bits.T2IS = 0; // Set subpriority
    IFS0bits.T2IF = 0; // Clear interrupt status flag
    IEC0bits.T2IE = 1; // Enable interrupts
    // T3
    IPC3bits.T3IP = 5; // Set priority
    IPC3bits.T3IS = 0; // Set subpriority
    IFS0bits.T3IF = 0; // Clear interrupt status flag
    IEC0bits.T3IE = 1; // Enable interrupts

    T2CONbits.ON = 1; //start timer at the end

    __builtin_enable_interrupts();

    INTCONbits.MVEC = 1; // Enable multi interrupts

    WDTCONbits.ON = 1; // Enable Watchdog timer: safety reset
    while (1) {
     WDTCONbits.WDTCLR = 1; // This is WDT !
    }
}

int main() {
    ex4();
    return (0);
}
