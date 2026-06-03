/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: dachin <dachin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/05/21 00:38:41 by dachin        #+#    #+#                 */
/*   Updated: 2026/06/03 10:20:16 by dachin        ########   odam.nl         */
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

// We copy because envp is owned by the OS/runtime — you
// can't modify it. But your shell needs to modify the
// environment when the user runs export or unset.
// Those built-ins add, change, or remove variables.
// If you used envp directly, those changes would be impossible.
// With your own copy in shell.env, you can freely modify it.
void	dup_envp(t_shell *shell, char **envp)
{
	int		i;
	char	**envp_cpy;

	if (!shell || !envp)
		return ;
	i = 0;
	while (envp[i])
		++i;
	envp_cpy = malloc(sizeof(char *) * (i + 1));
	if (!envp_cpy)
		return ;
	i = 0;
	while (envp[i])
	{
		envp_cpy[i] = ft_strdup(envp[i]);
		if (!envp_cpy[i])
		{
			free_partial_env(envp_cpy, i);
			return ;
		}
		++i;
	}
	envp_cpy[i] = NULL;
	shell->env = envp_cpy;
}

void	free_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		++i;
	}
	free(arr);
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
