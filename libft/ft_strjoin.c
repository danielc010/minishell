/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strjoin.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: luimarti <luimarti@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/05 13:42:43 by luimarti      #+#    #+#                 */
/*   Updated: 2026/06/15 15:32:17 by dachin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "libft.h"
#include <stdio.h>
#include <stdlib.h>

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*result;
	int		i;
	int		j;

	result = (char *)malloc((ft_strlen(s1)+(ft_strlen(s2)) + 1) * sizeof(char));
	i = 0;
	j = 0;
	if (!result)
		return (NULL);
	while (s1[i] != '\0')
	{
		result[j] = s1[i];
		i++;
		j++;
	}
	i = 0;
	while (s2[i] != '\0')
	{
		result[j] = s2[i];
		i++;
		j++;
	}
	result [j] = '\0';
	return (result);
}
// int main (void)
// {
//     char *a = "Hello, ";
//     char *b = "world!";
//     char *result = ft_strjoin (a, b);
//     if (!result)
//     {
//         printf ("ERROR: Memorie allocation failed.\n");
//         free (result);
//         return (1);
//     }
//     printf ("Success! result = %s\n", result);
//     free (result);
//     return (0);
// }
