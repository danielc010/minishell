/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_unset.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: lucho <lucho@student.42.fr>                  +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/05/27 16:48:07 by dachin        #+#    #+#                 */
/*   Updated: 2026/06/05 11:01:59 by dachin        ########   odam.nl         */
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
		if ((!ft_strncmp(shell->env[i], key, j)) && (shell->env[i][j] == '='))
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

	if (!shell || !cmd || !cmd->args[1])
		return ;
	key = cmd->args[1];
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
}
