/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luimarti <luimarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 13:47:23 by luimarti          #+#    #+#             */
/*   Updated: 2025/05/05 17:58:33 by luimarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include "libft.h"

int		searcher(const char *set, char c);
char	*new_str(const char *s1, size_t start, size_t len);

char	*ft_strtrim(char const *s1, char const *set)
{
	int		i;
	int		j;

	i = 0;
	j = ft_strlen(s1) - 1;
	if (ft_strlen(s1) == 0)
		return (ft_strdup(""));
	while (searcher (set, s1[i]))
		i++;
	while (searcher (set, s1[j]))
		j--;
	return (new_str(s1, i, j - i +1));
}

int	searcher(const char *set, char c)
{
	int	i;

	i = 0;
	while (set[i] != '\0')
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

char	*new_str(const char *s1, size_t start, size_t len)
{
	char	*str;
	size_t	i;

	str = ft_calloc(len + 1, sizeof(char));
	i = 0;
	if (len <= 0 || start >= ft_strlen(s1))
		return (ft_strdup(""));
	if (!str)
		return (NULL);
	while (i < len)
	{
		str[i] = s1[start + i];
		i++;
	}
	return (str);
}

// int main(void)
// {
// 	const char *src = "ooo___HelloCodam___!ooo";
// 	const char *trimmer = "o_!";
// 	const char *result;
// 	result = ft_strtrim(src, trimmer);
// 	printf ("String after trimming: %s\n", result);
// 	return (0);
// }
// // int main(void)
// // {
// // 	const char *src = "";
// // 	const char *trimmer = "";
// // 	const char *result;
// // 	result = ft_strtrim(src, trimmer);
// // 	printf ("String after trimming: %s\n", result);
// // 	return (0);
// // }
