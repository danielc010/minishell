/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luimarti <luimarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 19:34:57 by luimarti          #+#    #+#             */
/*   Updated: 2025/04/23 19:48:45 by luimarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	ft_bzero(void *ptr, size_t num)
{
	unsigned char	*p;
	size_t			n;

	p = (unsigned char *) ptr;
	n = 0;
	while (n < num)
	{
		p[n] = 0;
		n++;
	}
}

// int	main()
// {
// 	char str [30] = "Hello World!";

// 	ft_bzero(str + 2, 2);
// 	write(1, str, 12);
// 	write(1, "\n", 1);
// 	return (0);
// }