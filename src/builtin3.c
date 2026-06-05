/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin3.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: lucho <lucho@student.42.fr>                  +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/05/27 16:48:07 by dachin        #+#    #+#                 */
/*   Updated: 2026/06/05 08:09:09 by dachin        ########   odam.nl         */
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

int is_valid_identifier(char *str)
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

// Exporting shell variables and functions to child processes
// Create and export a variable to env: export VAR=value
void	builtin_export(t_shell *shell, t_cmd *cmd)
{
	char	*eq;
	char	*key;
	char	*value;

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
	key = ft_substr(cmd->args[1], 0, eq - cmd->args[1]);
	if (!is_valid_identifier(key))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(cmd->args[1], 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		shell->exit_status = 1;
		free(key);
		return ;
	}
	value = eq + 1;
	update_env(shell, key, value);
	free(key);
}
/* FIX: Added validation for export identifiers.
** Previously, builtin_export accepted any string as a variable name,
** including invalid identifiers like "A-", "123" or "=". Now we validate
** the key with is_valid_identifier() before calling update_env(). A valid
** identifier must start with a letter or '_' and contain only letters,
** digits and '_'. If invalid, we print "not a valid identifier" and set
** shell->exit_status = 1, matching bash behaviour. Added is_valid_identifier()
** as a helper function to keep builtin_export within norminette limits. */
