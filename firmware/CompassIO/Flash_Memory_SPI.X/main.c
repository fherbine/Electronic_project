/* 
 * File:   main.c
 * Author: bocal
 *
 * Created on May 24, 2018, 10:18 AM
 */

#include <p32xxxx.h>

#define _CS1_ON() (PORTDbits.RD5 = 0)
#define _CS1_OFF() (PORTDbits.RD5 = 1)

// DEVCFG2
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider (2x Divider)
#pragma config FPLLMUL = MUL_20         // PLL Multiplier (20x Multiplier)
#pragma config FPLLODIV = DIV_1         // System PLL Output Clock Divider (PLL Divide by 1)

// DEVCFG1
#pragma config FNOSC = PRIPLL           // Oscillator Selection Bits (Primary Osc w/PLL (XT+,HS+,EC+PLL))
#pragma config POSCMOD = HS             // Primary Oscillator Configuration (HS osc mode)

#define SYSCLK (8000000/2*20/1) // = 80Mhz
#define PBCLK (SYSCLK/8) // = 10 Mhz //
//#define Fsck 5000

#define SPI_BAUD_RATE 1
typedef unsigned char u8;
typedef unsigned long u32;
typedef long s32;

#define MAX_BUFFER_SIZE 256

#define UART_BAUD_RATE ((PBCLK/9600/16)-1)

/*
 * 
 */

/* UART2 */

void UART2_Init(u8 parityDataBits, u8 stopBits, u8 TRX_Mode)
{
    if (!TRX_Mode) {
	return;
    }
    U2BRG = UART_BAUD_RATE;
    U2MODEbits.PDSEL = parityDataBits;
    U2MODEbits.STSEL = stopBits;
    U2STAbits.URXEN = TRX_Mode != 1; // Enable reception
    U2MODEbits.ON = 1; // Enable UART1 Module
    U2STAbits.UTXEN = TRX_Mode & 1; // Enable transmission and set UxTXIF (Int)
}

void UART2_Send_Data_Byte(u8 data)
{
    while (U2STAbits.UTXBF);
    U2TXREG = data;
}

u8 UART2_Send_String(const char *string, u32 size)
{
    if ((string == NULL) || (size > MAX_BUFFER_SIZE))
	return (-1);
    u32 i;
    for (i = 0; i < size; i++)
	UART2_Send_Data_Byte(string[i]);
    return (0);
}

u32		ft_strlen(const char *s)
{
	char *str = s;
	while (*s++);
	return (s-str);
}

void ft_putstr(char *string)
{
   UART2_Send_String(string, ft_strlen(string));
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
	ft_putstr("\n\r");
}
void ft_putbinary(u32 nb)
{
    char str[35];
    u8 i;
    ft_bzero(str, 35);
    str[0] = '0';
    str[1] = 'b';
    for (i = 0; i < 32; i++) {
	str[33-i] = '0' + ((nb >> i) & 1);
    }
    ft_putstr(str);
	ft_putstr("\n\r");
}
/* ===== */


void SPI2_Write(unsigned char data)
{
    while (!SPI2STATbits.SPITBE); // SPI Transmit Buffer Full Status bit
    SPI2BUF = data;
}

void Handle_SPI(unsigned char input, unsigned char *output)
{
    SPI2BUF = input;
    while (!SPI2STATbits.SPIRBF);
    *output = SPI2BUF;
}

unsigned char SPI2_Read()
{
    if (SPI2STATbits.SPIRBF)     // Receive buffer is full - Auto cleared
    {
      if (SPI2STATbits.SPIROV)
      {
        SPI2STATbits.SPIROV = 0; // Overflow has occured
        return (0xff);
      }
      return (SPI2BUF & 0xFF);   // 8-bit mode
    }
    return (0);
}

#define FM_JEDECID 0x9F
#define FM_ID 0xAB
#define FM_WRITE_ENABLE 0x06
#define FM_PAGE_PROGRAM 0x02
#define FM_READ 0x03

int main()
{
	UART2_Init(0, 0, 3);
//    SPI1CON = 0x8220;
    SPI2BRG = 127; // Set baud rate
    SPI2CONbits.MSTEN = 1; // MSTEN is to enable master mode
	SPI2CONbits.CKE = 1;
	SPI2CONbits.CKP = 0;
    SPI2CONbits.ON = 1;
    TRISDbits.TRISD5 = 0; // writeable
    unsigned char output;
	_CS1_OFF();
    _CS1_ON();
/*	Read JEDECID
    Handle_SPI(FM_JEDECID, &output);
    Handle_SPI(0, &output);
	ft_putnbr_base(output, 16);
    Handle_SPI(0, &output);
	ft_putnbr_base(output, 16);
    Handle_SPI(0, &output);
	ft_putnbr_base(output, 16);
	Handle_SPI(0, &output);
	ft_putnbr_base(output, 16);
	Read ID
    Handle_SPI(FM_ID, &output);
    Handle_SPI(0, &output);
    Handle_SPI(0, &output);
    Handle_SPI(0, &output);
	Handle_SPI(0, &output);
	ft_putnbr_base(output, 16);*/
	/* Write */
//	Handle_SPI(FM_WRITE_ENABLE, &output);
//	Handle_SPI(FM_PAGE_PROGRAM, &output);
//	Handle_SPI(0x02, &output);
//	Handle_SPI(0x0, &output);
//	Handle_SPI(0x0, &output);
//	Handle_SPI('H', &output);
//	Handle_SPI('e', &output);
//	Handle_SPI('l', &output);
//	Handle_SPI('l', &output);
//	Handle_SPI('o', &output);
//	Handle_SPI('!', &output);
//	_CS1_OFF();
//    _CS1_ON();
	/* Read */
	Handle_SPI(FM_READ, &output);
	Handle_SPI(0x01, &output);
	Handle_SPI(0x0, &output);
	Handle_SPI(0x0, &output);
	Handle_SPI(0x0, &output);
	UART2_Send_Data_Byte(output);
	Handle_SPI(0x0, &output);
	UART2_Send_Data_Byte(output);
	Handle_SPI(0x0, &output);
	UART2_Send_Data_Byte(output);
	Handle_SPI(0x0, &output);
	UART2_Send_Data_Byte(output);
	Handle_SPI(0x0, &output);
	UART2_Send_Data_Byte(output);
    _CS1_OFF();
    return (0);
}
