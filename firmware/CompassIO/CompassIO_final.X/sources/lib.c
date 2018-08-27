
#include "types.h"

u32		ft_strlen(char *s)
{
	u32 count;

	count = 0;
	if (!s)
		return (0);
	while (s[count])
	{
		count++;
	}
	return (count);
}

int		ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned char *new_s1;
	unsigned char *new_s2;

	new_s1 = (unsigned char*)s1;
	new_s2 = (unsigned char*)s2;
	while ((*new_s1 || *new_s2) && n > 0)
	{
		if (*new_s1 != *new_s2)
			return (*new_s1 - *new_s2);
		new_s1++;
		new_s2++;
		n--;
	}
	return (0);
}

char	*ft_strchr(const char *s, int c)
{
	while (*s != (char)c && *s != '\0')
		s++;
	if (*s == (char)c)
		return ((char*)s);
	return (NULL);
}

int	ft_index(const char *s, int c)
{
	char *ptr;

	ptr = ft_strchr(s, c);
	if (ptr == NULL)
		return (-1);
	else
		return (ptr - s);
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

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char *str;

	str = (unsigned char*)s;
	while (n--)
	{
		*str = (unsigned char)c;
		str++;
	}
	return (s);
}

void ft_putstr(char *string)
{
	if (MAIN_DEBUG == UART_1)
		UART1_Send_String(string, ft_strlen(string));
	else
		UART2_Send_String(string, ft_strlen(string));
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
	if (MAIN_DEBUG == UART_1)
	{
		UART1_Send_String(string, ft_strlen(string));
		UART1_Send_String("\n\r", 2);
	}
	else
	{
		UART2_Send_String(string, ft_strlen(string));
		UART2_Send_String("\n\r", 2);
	}
}

void ft_putfloat(double nb) {
	int tmp;
	if (nb < 0)
	{
		if (MAIN_DEBUG == UART_1)
			UART1_Send_Data_Byte('-');
		else
			UART2_Send_Data_Byte('-');
		nb = -nb;
	}
	tmp = nb;
	ft_putnbr_base(tmp, 10);
	nb = (nb - tmp) * 1000000 + 0.5;
	if (MAIN_DEBUG == UART_1)
		UART1_Send_Data_Byte('.');
	else
		UART2_Send_Data_Byte('.');
	tmp = nb; ft_putnbr_base(tmp, 10);
}

