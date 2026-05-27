/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin3.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dachin <dachin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/05/27 16:48:07 by dachin        #+#    #+#                 */
/*   Updated: 2026/05/27 17:33:42 by dachin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	print_export(char *key, char *value)
{
	ft_putstr_fd("declare -x ", 1);
	ft_putstr_fd(key, 1);
	ft_putstr_fd("=\"", 1);
	ft_putstr_fd(value, 1);
	ft_putendl_fd("\"", 1);
}

void	no_arg_export(t_shell *shell)
{
	char	*eq;
	char	*key;
	char	*value;
	int		i;

	i = 0;
	while (shell->env[i])
	{
		eq = ft_strchr(shell->env[i], '=');
		if (!eq)
		{
			++i;
			continue ;
		}
		key = ft_substr(shell->env[i], 0, eq - shell->env[i]);
		value = eq + 1;
		print_export(key, value);
		free(key);
		++i;
	}
}

void	builtin_export(t_shell *shell, t_cmd *cmd)
{
	char	*eq;
	char	*key;
	char	*value;

	if (!cmd->args[1])
	{
		no_arg_export(shell);
	}
	else
	{
		eq = ft_strchr(cmd->args[1], '=');
		if (!eq)
			return ;
		key = ft_substr(cmd->args[1], 0, eq - cmd->args[1]);
		value = eq + 1;
		update_env(shell, key, value);
		free(key);
	}
}
