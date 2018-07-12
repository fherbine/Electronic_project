#include "types.h"

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
