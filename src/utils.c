/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luimarti <luimarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 00:38:41 by dachin            #+#    #+#             */
/*   Updated: 2026/06/02 13:45:00 by luimarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

// We copy because envp is owned by the OS/runtime — you
// can't modify it. But your shell needs to modify the
// environment when the user runs export or unset.
// Those built-ins add, change, or remove variables.
// If you used envp directly, those changes would be impossible.
// With your own copy in shell.env, you can freely modify it.

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
