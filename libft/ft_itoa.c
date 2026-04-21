/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luimarti <luimarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 18:09:50 by luimarti          #+#    #+#             */
/*   Updated: 2025/05/08 14:17:43 by luimarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include "libft.h"

static size_t	check_length(long num);
static char		*memo_alloc(size_t len);

char	*ft_itoa(int n)
{
	int		i;
	size_t	len;
	char	*result;
	long	num;

	num = n;
	len = check_length(num);
	result = memo_alloc(len);
	if (!result)
		return (NULL);
	if (n == 0)
		result[0] = '0';
	if (num < 0)
	{
		result[0] = '-';
		num = -num;
	}
	i = len - 1;
	while (num != 0)
	{
		result[i--] = (num % 10) + '0';
		num = num / 10;
	}
	result[len] = '\0';
	return (result);
}

static size_t	check_length(long num)
{
	int	counter;

	counter = 0;
	if (num < 0)
	{
		counter++;
		num = -num;
	}
	if (num == 0)
		counter++;
	while (num != 0)
	{
		num = num / 10;
		counter++;
	}
	return (counter);
}

static char	*memo_alloc(size_t len)
{
	char	*temp;

	temp = ft_calloc(len +1, sizeof(char));
	if (!temp)
		return (NULL);
	return (temp);
}
// int main (void)
// {
// 	char *result;	
// 	result = ft_itoa(-10004);
// 	printf("%s\n", result);
// 	free (result);
// 	return (0);
// }
