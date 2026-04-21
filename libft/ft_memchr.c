/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luimarti <luimarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 16:26:36 by luimarti          #+#    #+#             */
/*   Updated: 2025/04/28 18:57:13 by luimarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <string.h>

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*ptr;
	unsigned char	spot;
	size_t			i;

	ptr = (unsigned char *)s;
	spot = (unsigned char)c;
	i = 0;
	while (i < n)
	{
		if (ptr[i] == spot)
			return ((void *)(ptr + i));
		else
			i++;
	}
	return (NULL);
}

// int main()
// {
// 	const char a [] = "Hello world!";
// 	void *spot = ft_memchr (a, 'z', 12);
// 	printf("%p\n", spot);
// 	return (0);
// }