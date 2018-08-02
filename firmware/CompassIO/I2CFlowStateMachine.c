// https://www.microchip.com/forums/m567148.aspx

void __ISR(_I2C_1_VECTOR, ipl6) I2C1Handler(void){
    // Clear interrupt flags
    IFS0bits.I2C1MIF = 0;  // I2C1 Master Event
    IFS0bits.I2C1BIF = 0;  // I2C1 Bus Collision Event
    // Main actions
    I2C.BusStatus = I2C1STAT; // copy Status
    TASKFLAG.I2CSERV = 1; // Start I2C1 service in main
    return;
} // End of Int Service _I2CIntHandler


/* Master State Machine */
#define _I2CSTART 1
#define _I2CADDRESS 2
#define _I2CRCVSTART 3
#define _I2CREAD 4
#define _I2CWRITE 5
#define _I2CSTOP 6

void I2C1Service() {
  TASKFLAG.I2CSERV = 0; // Stop I2C1 service
  /* === START === */
  if (I2C.Status == _I2CSTART) {
    I2C1CONbits.SEN = 1;
    I2C.Status = _I2CADDRESS;
    return;
  }
  /* === ADDRESS === */
  if (I2C.Status == _I2CADDRESS) {
    I2C.SlaveAddress = I2C.DataTX[0];
    I2C.TXindex = 1;
    I2C.RXindex = 1;
    I2C1TRN = I2C.SlaveAddress;
    if (I2C.SlaveAddress & 1) { // Check R/W bit
      I2C.Status = _I2CRCVSTART;
    } else {
      I2C.Status = _I2CWRITE;
    }
    return;
  }
  /* === RCV START === */
  if (I2C.Status == _I2CRCVSTART) { // Read data
    I2C1CONbits.RCEN = 1; // Enable master receive
    I2C.Status = _I2CREAD;
    return;
  }
  /* === READ === */
  if (I2C.Status == _I2CREAD) {
    I2C.DataRX[I2C.RXindex] = I2C1RCV; // Copy to buffer
  }
  /* === WRITE === */
  if (I2C.Status == _I2CWRITE) { // Send data

  }
}

int main() {
  if (TASKFLAG.I2CSERV)
    I2C1Service();
}
