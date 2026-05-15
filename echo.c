/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucho <lucho@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 22:45:55 by lucho             #+#    #+#             */
/*   Updated: 2026/05/15 23:12:40 by lucho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_n_flag(char **args)
{
	int	i;

	i = 0;
	if (!args[1])
		return (0);
	if (args[1][i] == '-')
	{
		if (!args[1][i + 1])
			return (0);
		i = 1;
		while (args[1][i])
		{
			if (args[1][i] != 'n')
				return (0);
			i++;
		}
	}
	else
		return (0);
	return (1);
}

int	ft_echo(char **args)
{
	int	i;
	int	flag;

	i = 1;
	flag = 0;
	if (is_n_flag(args))
	{
		flag = 1;
		i = 2;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (flag == 0)
		printf("\n");
	return (0);
}
