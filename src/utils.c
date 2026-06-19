/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luimarti <luimarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 00:38:41 by dachin            #+#    #+#             */
/*   Updated: 2026/06/19 17:03:12 by luimarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// In dup_envp if ft_strdup fails halfway, not all of env is initialized / NULL.
// As env is unitialised memory, normal NULL terminated approach does not work.
// Precision is key to know exactly where to stop.
void	free_partial_env(char **env, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(env[i]);
		++i;
	}
	free(env);
}

int	count_cmds(t_cmd *cmd)
{
	int	i;

	if (!cmd)
		return (0);
	i = 0;
	while (cmd)
	{
		cmd = cmd->next;
		++i;
	}
	return (i);
}

int	is_valid_number(char *arg)
{
	int	i;

	i = 0;
	if (arg[0] == '+' || arg[0] == '-')
		i++;
	if (!arg[i])
		return (0);
	if (digits_too_large(arg + i))
		return (0);
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		i++;
	}
	return (1);
}
