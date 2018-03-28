#include "types.h"

int ex1(){
    TRISDbits.TRISD8 = 1;
    TRISFbits.TRISF1 = 0;
    LATFbits.LATF1 = 0;
    u8 val = 1;
    while (1)
    {
	if (PORTDbits.RD8 == 0){
	    if (val) {
		val = 0;
		LATFbits.LATF1 ^= 1;
	    }
	}else{
	    val = 1;
	}
    }
    return (0);
}

int ex2(){
    TRISDbits.TRISD8 = 1;
    TRISFbits.TRISF1 = 0;
    LATFbits.LATF1 = 0;
    u8 val = 1;
    u32 next_val = PERIOD; //0.5hz
    u32 timer = 0;
    while (1){
	timer++;
	if (timer % (next_val / 2) == 0)
	    LATFbits.LATF1 ^= 1;
	if (PORTDbits.RD8 == 0){
	    if (val != 0){
		val = 0;
		next_val = (next_val/2 == LIMIT_SPEED) ? PERIOD : (next_val/2);
	    }
	}else{
	    val = 1;
	}
    }
    return (0);
}

int main(){
    ex1();
    return (0);
}
