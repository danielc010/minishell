/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luimarti <luimarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 17:25:54 by luimarti          #+#    #+#             */
/*   Updated: 2025/04/25 21:45:43 by luimarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <string.h>
#include <stdio.h>

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	d_len;
	size_t	s_len;

	i = 0;
	d_len = 0;
	s_len = 0;
	while (d_len < size && dest[d_len] != '\0')
		d_len++;
	while (src[s_len] != '\0')
		s_len++;
	if (d_len == size)
		return (size + s_len);
	while (d_len + i < size - 1 && src[i] != '\0')
	{
		dest[d_len + i] = src[i];
		i++;
	}
	if (d_len + i < size)
		dest[d_len + i] = '\0';
	return (d_len + s_len);
}

// int main ()
// {
// 	char dest [12] = "Hello";
// 	char src [] = "World!";
// 	size_t res;
// 	res = ft_strlcat (dest, src, 12);
// 	printf ("%zu", res);
// 	return (0);
// }

//(line 31)Using size instead of d_len because not known
//if there is a \0 at the end, so the end of dest is not detected.
// Here (line 39) the \0 is detected and we can use d_len.