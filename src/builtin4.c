/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin4.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dachin <dachin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/06/05 11:01:33 by dachin        #+#    #+#                 */
/*   Updated: 2026/06/05 11:03:22 by dachin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// In the format as displayed when we type export with no arguments
void	print_export(char *key, char *value)
{
	ft_putstr_fd("declare -x ", 1);
	ft_putstr_fd(key, 1);
	ft_putstr_fd("=\"", 1);
	ft_putstr_fd(value, 1);
	ft_putendl_fd("\"", 1);
}

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

// POSIX rules for valid env variable identifier:
// must start with a letter (A-Z or a-z) or underscore (_)
// can only contain letters, digits (0-9) or underscores
int	is_valid_identifier(char *str)
{
	int i;

	i = 0;
	if (!ft_isalpha(str[0]) && str[0] != '_')
			return (0);
	while (str[i])
	{
		if (!ft_isalpha(str[i]) && str[i] != '_' && !ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static void	export_key_value(t_shell *shell, char *arg, char *eq)
{
	char	*key;

	key = ft_substr(arg, 0, eq - arg);
	if (!is_valid_identifier(key))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		shell->exit_status = 1;
		free(key);
		return ;
	}
	update_env(shell, key, eq + 1);
	free(key);
}

// Exporting shell variables and functions to child processes
// Create and export a variable to env: export VAR=value
void	builtin_export(t_shell *shell, t_cmd *cmd)
{
	char	*eq;

	if (!cmd->args[1])
	{
		no_arg_export(shell);
		return ;
	}
	eq = ft_strchr(cmd->args[1], '=');
	if (!eq)
	{
		if (!is_valid_identifier(cmd->args[1]))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(cmd->args[1], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			shell->exit_status = 1;
		}
		return ;
	}
	export_key_value(shell, cmd->args[1], eq);
}
