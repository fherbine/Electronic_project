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

/*
 * RD6 >> nRST
 * RD5 >> ON_OFF
 */

int res = -1;
char buffGPS[500];

t_coord coord;

unsigned char  rst = 0, tr0 = 0, on_off = 0, gps = 0;
unsigned short tmp = 0, tmp2 = 0;

void	gps_power_on(void)
{
	tmp++;
	if (tmp == 100 && !rst) // 100ms after PIC power on
	{
		LATDbits.LATD6 = 1;
		rst = 1;
	}
	else if (tmp == 110 && !on_off)
	{
		LATDbits.LATD5 = 1;
		on_off = 1;
	}
	else if (tmp == 310 && on_off)
	{
		LATDbits.LATD5 = 0;
		on_off = 0;
	}
	else if (tmp > 310)
	{
		tmp = 0;
		gps = 1;
	}
}

void	gps_power_off(void)
{
	if (tmp == 0)
		LATDbits.LATD5 = 1;
	else if (tmp == 200)
		LATDbits.LATD5 = 0;
	tmp++;
}

void __ISR(_TIMER_2_VECTOR, IPL1) Timer2Handler(void) {
	TMR2 = 0;
	if (!gps)
		gps_power_on();
	else if ((PORTDbits.RD8 == 0 && gps) || gps == 2)
	{
		gps = 2;
		gps_power_off();
	}
	//LATDbits.LATD4 ^= 1;
	IFS0bits.T2IF = 0;
}

void ft_putfloat(double nb) { int tmp; if (nb < 0) { UART2_Send_Data_Byte('-'); nb = -nb; } tmp = nb; ft_putnbr_base(tmp, 10); nb = (nb - tmp) * 1000000 + 0.5; UART2_Send_Data_Byte('.'); tmp = nb; ft_putnbr_base(tmp, 10); }

void HandleGPS(void) {
	// Store input in buffer
	u32 dest_len = ft_strlen(buffGPS);
	buffGPS[dest_len] = UART1_Get_Data_Byte();
//	ft_putnbr_base(UART1_Get_Data_Byte(), 10);
//	UART2_Send_Data_Byte(UART1_Get_Data_Byte());
//	UART2_Send_Data_Byte(buffGPS[dest_len]);
	buffGPS[dest_len + 1] = '\0';
//	ft_putstr("Here");
	if (dest_len > 0 && buffGPS[dest_len - 1] == 13 && buffGPS[dest_len] == 10)
	{
		if (!ft_strncmp(buffGPS, "$GPRMC,", 7)) {
			buffGPS[dest_len - 1] = '\0';
			buffGPS[dest_len] = '\0';
			res = parse_nmea_gps(buffGPS, &coord);
			if (res == 1)
			{
				ft_putfloat(coord.lat);
				UART2_Send_Data_Byte('-');
				ft_putfloat(coord.lon);
			}
		}
		dest_len = 0;
		ft_bzero(buffGPS, 500);
	}
	LATFbits.LATF1 ^= 1;
}

/* UART -> Bluetooth */
void __ISR(_UART1_VECTOR, IPL2SRS) UART1Handler(void) {
	// Reception
	if (IFS0bits.U1RXIF) {
		IFS0CLR = U1RX_IFS1;
		HandleGPS();
	}
	// Transmit
	if (IFS0bits.U1TXIF)
		IFS0CLR = U1TX_IFS1;
	// Error
	if (IFS0bits.U1EIF)
		IFS0CLR = U1E_IFS1;
}

/* UART -> GPS/Debug */
void __ISR(_UART2_VECTOR, IPL2SRS) UART2Handler(void) {
	// Reception
	if (IFS1bits.U2RXIF) {
		IFS1CLR = U2RX_IFS1;
		LATFbits.LATF1 ^= 1;
	}
	// Transmit
	if (IFS1bits.U2TXIF)
		IFS1CLR = U2TX_IFS1;
	// Error
	if (IFS1bits.U2EIF)
		IFS1CLR = U2E_IFS1;
}

int main()
{
	__builtin_disable_interrupts();
	TRISDbits.TRISD6 = 0;				// RD6 is an output
	LATDbits.LATD6 = 0;					// RD6 is low by default

	TRISDbits.TRISD5 = 0;				// RD5 is an output
	LATDbits.LATD5 = 0;					// RD5 is low by default

	TRISDbits.TRISD8 = 1;

	T2CON = 0;               // 0 on every bit, (timer stop, basic config)
	TMR2 = 0;                // Clean the timer register
	IFS0bits.T2IF = 0;     // Clear interrupt status flag
	T2CONbits.TCKPS = 0b110; // Set scaler 1:256
	PR2 = PBCLK/64/1000;    // Setup the period > Periph. Bus Clk (set by user) / scaler > sec | 20ms

  IPC2bits.T2IP = 1; // Set priority
  IPC2bits.T2IS = 2; // Set subpriority
  IFS0bits.T2IF = 0; // Clear interrupt status flag
  IEC0bits.T2IE = 1; // Enable interrupts
	T2CONbits.ON = 1;


	INTCONbits.MVEC = 1; // Enable multi interrupts
	UART2_Init(0, 1, 0b11);
	UART1_Init(0, 1, 0b11);
	__builtin_enable_interrupts();

	coord.lat = 0.0;
	coord.lon = 0.0;
	ft_bzero(buffGPS, 500);
	while(1);
}
