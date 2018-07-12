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