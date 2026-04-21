/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luimarti <luimarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 13:55:31 by luimarti          #+#    #+#             */
/*   Updated: 2025/05/05 17:51:12 by luimarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <string.h>

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	stop;

	stop = 0;
	while (stop < n)
	{
		if ((unsigned char)s1[stop] != (unsigned char)s2[stop])
		{
			return ((unsigned char)s1[stop] - (unsigned char)s2[stop]);
		}
		if ((unsigned char)s1[stop] == '\0')
		{
			return (0);
		}
		stop++;
	}
	return (0);
}

// int main (void)
// {
// 	char a [] = "";
// 	char b [] = "";
// 	int	i = 1;
// 	printf ("%d\n", ft_strncmp (a ,b, i));
// 	printf ("%d\n", strncmp (a, b, i));
// 	return (0);
// }