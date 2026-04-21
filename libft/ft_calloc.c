/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luimarti <luimarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 10:18:48 by luimarti          #+#    #+#             */
/*   Updated: 2025/05/08 19:34:13 by luimarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

void	*ft_calloc(size_t nmemb, size_t size)
{
	unsigned char	*tmp;
	size_t			i;

	if (nmemb == 0 || size == 0)
		return (malloc(0));
	if (nmemb > __SIZE_MAX__ / size)
		return (NULL);
	tmp = malloc (nmemb * size);
	i = 0;
	if (!tmp)
		return (NULL);
	while (i < nmemb * size)
		tmp [i++] = 0;
	return (tmp);
}
//nmemb * size needed in case of the size of char changes in the system.
// int main ()
// {
// 	size_t n = 10;
//     char *my_calloc = (char *)ft_calloc(n, sizeof(char));
//     char *std_calloc = (char *)calloc(n, sizeof(char));
//     int i = 0;
//     int identical = 1;

//     if (!my_calloc || !std_calloc)
//     {
//         printf("Error: Memory allocation failed.\n");
//         free(my_calloc);
//         free(std_calloc);
//         return (1);
//     }

//     while (i < (int)n)
//     {
//         if (my_calloc[i] != std_calloc[i])
//         {
//             identical = 0;
//             break;
//         }
//         i++;
//     }

//     if (identical)
//         printf("✅ Both versions match.\n");
//     else
//         printf("❌ Versions does not match.\n");

//     free(my_calloc);
//     free(std_calloc);
//     return (0);
// }	