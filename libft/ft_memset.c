/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luimarti <luimarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 14:23:54 by luimarti          #+#    #+#             */
/*   Updated: 2025/04/24 20:15:35 by luimarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	*ft_memset(void *ptr, int value, size_t num)
{
	unsigned char	*cptr;
	unsigned int	cvalue;
	size_t			n;

	cptr = (unsigned char *) ptr;
	cvalue = (unsigned int) value;
	n = 0;
	while (n < num)
	{
		cptr[n] = cvalue;
		n++;
	}
	return (ptr);
}

// int	main()
// {
// 	char str [30] = "Hello World!";

// 	ft_memset(str + 2, 'Z', 5);
// 	write(1, str, 12);
// 	write(1, "\n", 1);
// 	return (0);
// }