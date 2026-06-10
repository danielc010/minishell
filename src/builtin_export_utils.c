/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_export_utils.c                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: dachin <dachin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/06/10 16:12:27 by dachin        #+#    #+#                 */
/*   Updated: 2026/06/10 16:19:35 by dachin        ########   odam.nl         */
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

// POSIX rules for valid env variable identifier:
// must start with a letter (A-Z or a-z) or underscore (_)
// can only contain letters, digits (0-9) or underscores
int	is_valid_identifier(char *str)
{
	int	i;

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

void	exist_check(t_shell *shell, char *key)
{
	int		i;
	char	*entry;

	if (!shell || !key)
		return ;
	i = 0;
	while (shell->env[i])
	{
		if (!ft_strncmp(shell->env[i], key, ft_strlen(key))
			&& (shell->env[i][ft_strlen(key)] == '='
			|| shell->env[i][ft_strlen(key)] == '\0'))
			return ;
		++i;
	}
	entry = ft_strdup(key);
	if (!entry)
		return ;
	add_env(shell, i, entry);
}

void	invalid_identifier(t_shell *shell, char *arg)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	shell->exit_status = 1;
}
