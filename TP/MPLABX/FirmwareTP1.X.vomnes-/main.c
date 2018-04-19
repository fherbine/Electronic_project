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

u8 limit = 380;
u8 level = 2;
u8 step = 0;
u8 count = 0;
u8 inc = TRUE;
u8 dim_active = FALSE; // ne plus utiliser de globales --> se servir de registre

void led_dim() {
    if (count < step) {
      LATFbits.LATF1 = 1;
    } else {
      LATFbits.LATF1 = 0;
    }
    count++;
    if (count > limit) {
        count = 0;
        if (step == limit && inc == TRUE) {
            inc = FALSE;
        } else if (step == 0 && inc == FALSE) {
            inc = TRUE;
        }
        if (count < (limit / 2)) {
             if (inc == TRUE) {
                step += 1;
            } else {
                step -= 1;
            }
        } else {
             if (inc == TRUE) {
                step += level;
            } else {
                step -= level;
            }
        }
    }
}

void led_blinky() {
    LATFbits.LATF1 ^= 1; // Switch ON/OFF LED
}

void __ISR(_TIMER_2_VECTOR, IPL1) Timer2Handler(void) {
    if (dim_active) {
     led_dim();
    } else {
     led_blinky();
    }
    TMR2 = 0;
    IFS0bits.T2IF = 0;
}

void __ISR(_TIMER_3_VECTOR, IPL3) Button2secHandler(void) {
    /* Active dim mode */
    dim_active = TRUE;
    /* Update timer frequency for dim mode */
    T2CONbits.ON = 0;
    TMR2CLR = 0xFFFF;
    T2CONbits.TCKPS = 0b000; // Set scaler 1:0
    PR2 = 5;            // Setup the period
    T2CONbits.ON = 1;
    /* Reset Interrupt */
    TMR3 = 0;
    IFS0bits.T3IF = 0;
}

void SpeedUpFrequencyT2() {
     T2CONbits.ON = 0;         // STOP Timer
     TMR2CLR = 0xFFFF;         // Clear timer
     if (PR2 == (PERIOD / 32)) // Until I reach the maximum (8Hz)
       PR2 = PERIOD;
     else
       PR2 /= 2;        // Divide period by two, thus increasing frequency,
     T2CONbits.ON = 1;    // START Timer
}

void __ISR(_EXTERNAL_1_VECTOR, IPL6) ButtonHandler(void) {
       if (INTCONbits.INT1EP == 1){
           // Stop timer
	   TMR3 = 0;
	   T3CONbits.ON = 0;

	   // Get button pushing instead of getting release
           INTCONbits.INT1EP = 0;

	   /* Not dim mode -> ++ speed frequency */
	   if (!dim_active) {
	     SpeedUpFrequencyT2();
	   }
       } else {
	   // Launch Timer3 - Check 2sec pressed button
	   TMR3 = 0;
	   T3CONbits.ON = 1;
           INTCONbits.INT1EP = 1; // Active button release mode
       }
    IFS0bits.INT1IF = 0; // Reset to 0 Interrupt INT0

}

void ex4() {

    __builtin_enable_interrupts();

    INTCONbits.MVEC = 1; // Enable multi interrupts
    TRISDbits.TRISD8 = 1;
    TRISFbits.TRISF1 = 0;
    LATFbits.LATF1 = 0;

    // Timer 2 - DIM
    T2CON = 0;               // 0 on every bit, (timer stop, basic config)
    TMR2 = 0;                // Clean the timer register
    T2CONbits.TCKPS = 0b101;
    PR2 = PERIOD;
    
    //Timer 3 - 2sec on button
    T3CON = 0;
    TMR3 = 0;
    T3CONbits.TCKPS = 0b110; // Set scaler 1:64
    PR3 = PERIOD; //on reste appuye deux secondes

    INTCONbits.INT1EP = 0; //0->lorsqu'on entre, 1 lorsqu'on sort l'interrupt
    //se produit

    // INT1
    IPC1bits.INT1IP = 6;
    IPC1bits.INT1IS = 0;
    IFS0bits.INT1IF = 0;
    IEC0bits.INT1IE = 1;

    // T2
    IPC2bits.T2IP = 1; // Set priority
    IPC2bits.T2IS = 2; // Set subpriority
    IFS0bits.T2IF = 0; // Clear interrupt status flag
    IEC0bits.T2IE = 1; // Enable interrupts
    // T3
    IPC3bits.T3IP = 3; // Set priority
    IPC3bits.T3IS = 3; // Set subpriority
    IFS0bits.T3IF = 0; // Clear interrupt status flag
    IEC0bits.T3IE = 1; // Enable interrupts

    T2CONbits.ON = 1; //start timer at the end

//    INTCONbits.INT1EP = 1;

    WDTCONbits.ON = 1; // Enable Watchdog timer: safety reset
    while (1) {
     WDTCONbits.WDTCLR = 1; // This is WDT !
    }
}

int main() {
    ex4();
    return (0);
}
