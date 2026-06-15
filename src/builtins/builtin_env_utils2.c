/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_env_utils2.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: dachin <dachin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/06/15 16:31:07 by dachin        #+#    #+#                 */
/*   Updated: 2026/06/15 16:48:25 by dachin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	replace_env_key(t_shell *shell, char *key, char *keq, char *keqval)
{
	int	i;

	i = 0;
	while (shell->env[i])
	{
		if (!ft_strncmp(shell->env[i], keq, ft_strlen(keq))
			|| (!ft_strncmp(shell->env[i], key, ft_strlen(key))
				&& shell->env[i][ft_strlen(key)] == '\0'))
		{
			replace_env_entry(&shell->env[i], keqval);
			free(keq);
			return (-1);
		}
		++i;
	}
	return (i);
}
