/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luimarti <luimarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:48:45 by luimarti          #+#    #+#             */
/*   Updated: 2025/05/08 20:05:10 by luimarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include "libft.h"

void	*ft_memmove(void *dest, void *src, size_t n)
{
	unsigned char	*csrc;
	unsigned char	*cdest;

	if (!dest && !src)
		return (NULL);
	csrc = (unsigned char *) src;
	cdest = (unsigned char *) dest;
	if (cdest < csrc || cdest >= (csrc + n))
		ft_memcpy(dest, src, n);
	else
	{
		while (n > 0)
		{
			n--;
			cdest[n] = csrc[n];
		}
	}
	return (dest);
}

// int main() 
// { 
// 	char csrc[100] = "HelloWorld"; 
// 	ft_memmove(csrc+5, csrc, 11); 
// 	printf("%s", csrc); 
// 	return 0; 
// } 
