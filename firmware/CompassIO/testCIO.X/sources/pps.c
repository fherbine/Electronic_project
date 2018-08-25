#include "types.h"

void set_pps(void)
{
	TRISBbits.TRISB10 = 1;
	INT1Rbits.INT1R = 0b0011; // INT1 >> main button
	TRISAbits.TRISA4 = 1;
	U1RXRbits.U1RXR = 0b0010; // RX1 >> BT
	
	CFGCONbits.IOLOCK = 0;
	RPB7Rbits.RPB7R = 0b0101; // OC1 >> servomotor
	RPB4Rbits.RPB4R = 0b0001; // TX1 >> BT
	RPB0Rbits.RPB0R = 0b0010; // TX2 >> GPS / DEBUG
	CFGCONbits.IOLOCK = 1;
}
