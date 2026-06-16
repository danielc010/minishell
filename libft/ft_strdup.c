/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucho <lucho@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 15:17:01 by luimarti          #+#    #+#             */
/*   Updated: 2026/06/16 23:27:42 by lucho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char	*ft_strdup(const char *s)
{
	size_t		length;
	size_t		i;
	char		*copy;

	if (!s)
		return (NULL);
	length = 0;
	i = 0;
	while (s[length] != '\0')
	{
		length++;
	}
	copy = (char *)malloc((length + 1) * sizeof(char));
	if (copy == NULL)
		return (NULL);
	while (i <= length)
	{
		copy[i] = s[i];
		i++;
	}
	return (copy);
}

// int main ()
// {
// 	const char *src = "HelloWorld!";
// 	char *copy = ft_strdup(src);
// 	char *testcopy = strdup(src);
// 	size_t i = 0;
// 	int equals = 1;

// 	if (!copy || !testcopy)
// 	{
// 		printf ("Error: Duplication failed.\n");
// 		free(copy);
// 		free(testcopy);
// 		return (1);
// 	}
// 	while (copy[i] != '\0' && testcopy[i] != '\0')
// 	{
// 		if (copy[i] != testcopy[i])
// 			equals = 0;
// 		i++;
// 	}
// 	if (equals)
// 		printf ("✅ Match!: %s\n", copy);
// 	else
// 		printf ("❌ Both strings do not match!\n");
// 	free(copy);
// 	free(testcopy);
// 	return (0);
// }
