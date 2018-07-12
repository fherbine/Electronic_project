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

void *ft_bzero(void *s, u32 size)
{
    char *str;

    str = (char*)s;
    while (size--)
    {
	*str = 0;
	str++;
    }
    return (s);
}

static s32	ft_count(s32 nb, char base)
{
	int count;

	count = 0;
	if (nb == 0)
		return (1);
	while (nb > 0)
	{
		nb = nb / base;
		count++;
	}
	return (count);
}

void	        ft_putnbr_base(s32 nb, int base)
{
	s32	n;
	int	sign_len[2];
	char	str[32];

	n = nb;
	sign_len[1] = 0;
	if (n < 0)
		n = -n;
	if (n < 0 && base == 10)
		sign_len[1] = 1;
	ft_bzero(str, 32);
	sign_len[0] = ft_count(n, base) + sign_len[1];
	sign_len[0]--;
	if (nb == 0)
		str[sign_len[0]] = '0';
	while (n)
	{
		(n % base <= 9) ? (str[sign_len[0]--] = (n % base) + '0') :
		(str[sign_len[0]--] = (n % base) + 'A' - 10);
		n /= base;
	}
	if (sign_len[1] == 1)
		str[sign_len[0]] = '-';
	ft_putstr(str);
}

#define NEWLINE '\n'

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

	int res = -1;
	char buffGPS[500];

	t_coord coord;
	coord.lat = 0.0;
	coord.lon = 0.0;
	ft_bzero(buffGPS, 500);
	u32 dest_len = 0;
	while(1)
	{
		if (dest_len >= 499)
		{
			dest_len = 0;
			ft_bzero(buffGPS, 500);
		}
		//UART2_Send_Data_Byte(UART1_Get_Data_Byte());
		// Store input in buffer
		buffGPS[dest_len] = UART1_Get_Data_Byte();
		UART2_Send_Data_Byte(buffGPS[dest_len]);
		//ft_putstr(buffGPS[dest_len]);
		ft_putstr("<");
		ft_putnbr_base(dest_len, 10);
		ft_putstr("-");
		ft_putnbr_base(buffGPS[dest_len - 1], 10);
		ft_putstr("-");
		ft_putnbr_base(buffGPS[dest_len], 10);
		ft_putstr(">");
		if (dest_len > 0 && buffGPS[dest_len - 1] == 13 && buffGPS[dest_len] == 10)
		{
			ft_putstr("NL");
			dest_len = 0;
			ft_bzero(buffGPS, 500);
		}
	//	ft_putnbr_base(dest_len, 10);
		//buffGPS[dest_len + 1] = '\0';
		//if (FALSE && dest_len > 0 && buffGPS[dest_len - 1] == 13 && buffGPS[dest_len] == 10) {
	//		buffGPS[dest_len] = '\0';
	//		buffGPS[dest_len - 1] = '\0';
	//		dest_len--;
	//		res = parse_nmea_gps(buffGPS, &coord);
	//		ft_putstr("res : ");
	//		ft_putnbr_base(res, 10);
	//		ft_putstr("lat : ");
	//		ft_putnbr_base(coord.lat, 10);
	//		ft_putstr("long : ");
	//		ft_putnbr_base(coord.lon, 10);
	//		ft_putstr("\n");
	//		ft_bzero(buffGPS, 500);
	//		dest_len = 0;
	//	}
	//	else
	//	{
	//		ft_putstr("NOLINE >> ");
			//ft_putstr(buffGPS);
	//		ft_putstr("\n");
		//}
		dest_len++;
	}
}