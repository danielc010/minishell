/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_export.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: dachin <dachin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/06/05 11:01:33 by dachin        #+#    #+#                 */
/*   Updated: 2026/06/10 16:24:37 by dachin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// If no NAMEs are given,a list of all names that are exported in this shell is
// printed.
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
			ft_putstr_fd("declare -x ", 1);
			ft_putstr_fd(shell->env[i], 1);
			ft_putstr_fd("\n", 1);
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

void	export_key_value(t_shell *shell, char *arg, char *eq)
{
	char	*key;

	key = ft_substr(arg, 0, eq - arg);
	if (!is_valid_identifier(key))
	{
		invalid_identifier(shell, arg);
		free(key);
		return ;
	}
	update_env(shell, key, eq + 1);
	free(key);
}

void	export_no_value(t_shell *shell, char *arg)
{
	if (!is_valid_identifier(arg))
	{
		invalid_identifier(shell, arg);
		return ;
	}
	exist_check(shell, arg);
}

// Exporting shell variables and functions to child processes
// Create and export a variable to env: export VAR=value
void	builtin_export(t_shell *shell, t_cmd *cmd)
{
	char	*eq;
	int		i;

	if (!cmd->args[1])
	{
		no_arg_export(shell);
		return ;
	}
	i = 1;
	while (cmd->args[i])
	{
		eq = ft_strchr(cmd->args[i], '=');
		if (!eq)
			export_no_value(shell, cmd->args[i]);
		else
			export_key_value(shell, cmd->args[i], eq);
		++i;
	}
}
