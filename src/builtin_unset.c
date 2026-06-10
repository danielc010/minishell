/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_unset.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: lucho <lucho@student.42.fr>                  +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/05/27 16:48:07 by dachin        #+#    #+#                 */
/*   Updated: 2026/06/10 17:00:26 by dachin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// The additional == '=' check ensures key "FOX" doesn't trigger "FOXERA=..."
int	key_index(t_shell *shell, char *key)
{
	int	i;
	int	j;

	i = 0;
	j = ft_strlen(key);
	while (shell->env[i])
	{
		if (!ft_strncmp(shell->env[i], key, j) && (shell->env[i][j] == '='
			|| shell->env[i][j] == '\0'))
			return (i);
		++i;
	}
	return (-1);
}

// For each NAME, remove the corresponding variable or function. Shift
// everythingleft by one position then sets last entry to NULL.
// No reallocation of array.
void	builtin_unset(t_shell *shell, t_cmd *cmd)
{
	char	*key;
	int		index;
	int		i;

	if (!shell || !cmd || !cmd->args[1])
		return ;
	i = 1;
	while (cmd->args[i])
	{
		if (!is_valid_identifier(cmd->args[i]))
		{
			invalid_identifier(shell, cmd->args[i], "unset");
			++i;
			continue ;
		}
		key = cmd->args[i];
		index = key_index(shell, key);
		if (index != -1)
		{
			free(shell->env[index]);
			while (shell->env[index])
			{
				shell->env[index] = shell->env[index + 1];
				++index;
			}
			shell->env[index] = NULL;
		}
		++i;
	}
}
