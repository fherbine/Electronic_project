 /*
 * File:   utils.c
 * Author: vomnes
 *
 * Created on May 10, 2018, 10:14 PM
 */

#include "types.h"

//u32		ft_strlen(const char *s)
//{
//	char *str = s;
//	while (*s++);
//	return (s-str);
//}

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
	if (n < 0 && base == 10)
	{
		n = -n;
		sign_len[1] = 1;
	}
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
}

void ft_putendl(char *string)
{
   UART2_Send_String(string, ft_strlen(string));
   UART2_Send_String("\n\r", 2);
}

void ft_putfloat(double nb) { int tmp; if (nb < 0) { UART2_Send_Data_Byte('-'); nb = -nb; } tmp = nb; ft_putnbr_base(tmp, 10); nb = (nb - tmp) * 1000000 + 0.5; UART2_Send_Data_Byte('.'); tmp = nb; ft_putnbr_base(tmp, 10); }
