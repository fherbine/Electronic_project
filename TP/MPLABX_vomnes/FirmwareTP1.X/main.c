#include "types.h"
int main(){
    TRISDbits.TRISD8 = 1;
    TRISFbits.TRISF1 = 0;
    u8 thz = 5;
    while (1)
    {
        if (PORTDbits.RD8 == 1){
            thz *= 2;
        }
        LATFbits.LATF1 = 1;
        sleep(thz * 100);
        LATFbits.LATF1 = 0;
        sleep(thz * 100);

    }
    return (0);
}
