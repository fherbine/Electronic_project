#include <p32xxxx.h>
#include <sys/attribs.h>
#include <xc.h>

// DEVCFG3
// USERID = No Setting
#pragma config PMDL1WAY = ON            // Peripheral Module Disable Configuration (Allow only one reconfiguration)
#pragma config IOL1WAY = ON             // Peripheral Pin Select Configuration (Allow only one reconfiguration)
#pragma config FUSBIDIO = ON            // USB USID Selection (Controlled by the USB Module)
#pragma config FVBUSONIO = ON           // USB VBUS ON Selection (Controlled by USB Module)

// DEVCFG2
#pragma config FPLLIDIV = DIV_12        // PLL Input Divider (12x Divider)
#pragma config FPLLMUL = MUL_24         // PLL Multiplier (24x Multiplier)
#pragma config UPLLIDIV = DIV_12        // USB PLL Input Divider (12x Divider)
#pragma config UPLLEN = OFF             // USB PLL Enable (Disabled and Bypassed)
#pragma config FPLLODIV = DIV_256       // System PLL Output Clock Divider (PLL Divide by 256)

// DEVCFG1
#pragma config FNOSC = FRCDIV           // Oscillator Selection Bits (Fast RC Osc w/Div-by-N (FRCDIV))
#pragma config FSOSCEN = ON             // Secondary Oscillator Enable (Enabled)
#pragma config IESO = ON                // Internal/External Switch Over (Enabled)
#pragma config POSCMOD = OFF            // Primary Oscillator Configuration (Primary osc disabled)
#pragma config OSCIOFNC = OFF           // CLKO Output Signal Active on the OSCO Pin (Disabled)
#pragma config FPBDIV = DIV_8           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/8)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Clock Switch Disable, FSCM Disabled)
#pragma config WDTPS = PS1048576        // Watchdog Timer Postscaler (1:1048576)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable (Watchdog Timer is in Non-Window Mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (WDT Disabled (SWDTEN Bit Controls))
#pragma config FWDTWINSZ = WINSZ_25     // Watchdog Timer Window Size (Window Size is 25%)

// DEVCFG0
#pragma config JTAGEN = ON              // JTAG Enable (JTAG Port Enabled)
#pragma config ICESEL = ICS_PGx3        // ICE/ICD Comm Channel Select (Communicate on PGEC3/PGED3)
#pragma config PWP = OFF                // Program Flash Write Protect (Disable)
#pragma config BWP = OFF                // Boot Flash Write Protect bit (Protection Disabled)
#pragma config CP = OFF                 // Code Protect (Protection Disabled)


void __ISR(_EXTERNAL_1_VECTOR, IPL1AUTO) MainButtonHandler(void) {
	IFS0bits.INT1IF = 0;
	LATBbits.LATB2 = 1;
//	if (INTCONbits.INT1EP == 0) { // Button Released
//
//			INTCONbits.INT1EP = 1;
//
//		} else {
//			INTCONbits.INT1EP = 0; // Active button1 release mode
//		}
}

void main()
{
	TRISBbits.TRISB2 = 0;
	TRISBbits.TRISB10 = 1;
	//CFGCONbits.IOLOCK = 0;
	//DEVCFG3bits.IOL1WAY = 1; // only one reconfiguration

	INT1R = 0b0011;
	//CFGCONbits.IOLOCK = 1;

	LATBbits.LATB2 = 0;
//
	__builtin_disable_interrupts();
	INTCONbits.MVEC = 1; // Enable multi interrupts
	INTCONbits.INT1EP = 1; //1->lorsqu'on entre, 0 lorsqu'on sort l'interrupt se produit*/
//	// INT1 - Button
	IPC1bits.INT1IP = 1;
	IPC1bits.INT1IS = 0;
	IFS0bits.INT1IF = 0;
	IEC0bits.INT1IE = 1;
	__builtin_enable_interrupts();
	while(42);
}