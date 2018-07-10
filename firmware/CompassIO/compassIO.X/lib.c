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

char	*ft_strcat(char dest[500], const char c)
{
	size_t		dest_len;
	size_t		i;

	dest_len = ft_strlen(dest);
	dest[dest_len + i] = c;
	dest[dest_len + i] = '\0';
	return (dest);
}