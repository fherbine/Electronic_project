/*
 * File:   lib.c
 * Author: vomnes
 *
 * Created on July 10, 2018, 10:34 PM
 */

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
