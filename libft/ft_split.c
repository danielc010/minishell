/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucho <lucho@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:59:29 by luimarti          #+#    #+#             */
/*   Updated: 2025/10/30 20:06:40 by lucho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include "libft.h"

static int	subarray_count(const char *s, char c);
static char	**fill_split(char **big, const char *s, int word_count, char c);
static void	*free_split(char **big, int filled);

char	**ft_split(const char *s, char c)
{
	char		**big;
	int			word_count;

	if (!s)
		return (NULL);
	word_count = subarray_count (s, c);
	big = (char **)ft_calloc((word_count + 1), sizeof(char *));
	if (!big)
		return (NULL);
	return (fill_split(big, s, word_count, c));
}

static int	subarray_count(const char *s, char c)
{
	int	i;
	int	trigger;
	int	count;

	i = 0;
	trigger = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] != c && trigger == 0)
		{
			trigger = 1;
			count++;
		}
		else if (s[i] == c)
			trigger = 0;
		i++;
	}
	return (count);
}

static char	**fill_split(char **big, const char *s, int word_count, char c)
{
	const char	*str;
	int			i;
	int			len;

	i = 0;
	while (*s && i < word_count)
	{
		while (*s == c)
			s++;
		len = 0;
		str = s;
		while (*s && *s != c)
		{
			s++;
			len++;
		}
		big[i] = (char *)ft_calloc(len + 1, sizeof(char));
		if (!big[i])
			return (free_split(big, i));
		ft_strlcpy (big[i], str, len + 1);
		i++;
	}
	big[i] = NULL;
	return (big);
}

static void	*free_split(char **big, int filled)
{
	int	i;

	i = 0;
	while (i < filled)
	{
		free(big[i]);
		i++;
	}
	free(big);
	return (NULL);
}

// int main (void)
// {
// 	char *array = ",,,,Hello,world,,from,,,Codam!";
// 	char del = ',';
// 	char **result;
// 	int i;
// 	i = 0;
// 	result = ft_split (array, del);
// 	if (!result)
// 	{
// 		printf ("Error! ft_splt does not work.\n");
// 		return (1);
// 	}
// 	while (result[i])
// 	{
// 		printf ("Subarray %d: %s\n", i, result[i]);
// 		i++;
// 	}
// 	i = 0;
// 		while (result[i])
// 	{
// 		free(result[i]);
// 		i++;
// 	}
// 	return (0);
// }