/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luimarti <luimarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 13:29:02 by luimarti          #+#    #+#             */
/*   Updated: 2025/05/08 16:26:01 by luimarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	i;
	char			*result;

	result = ft_calloc(ft_strlen(s) + 1, sizeof (char));
	if (!result)
		return (NULL);
	i = 0;
	while (i < ft_strlen(s))
	{
		result[i] = (*f)(i, s[i]);
		i++;
	}
	return (result);
}

// static char	test_f(unsigned int i, char c)
// {
// 	(void)i;
// 	return ((char)ft_toupper(c));
// }
// int main (void)
// {
// 	char *s = "helloworld!";
// 	char *result = ft_strmapi(s, test_f);
// 	printf ("Original: %s.\n", s);
// 	printf ("Final: %s.\n", result);
// 	return (0);
// }
