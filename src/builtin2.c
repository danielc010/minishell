/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin2.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dachin <dachin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/05/20 23:58:04 by dachin        #+#    #+#                 */
/*   Updated: 2026/06/03 16:41:56 by dachin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Relating to directory change
void	builtin_cd(t_shell *shell, t_cmd *cmd)
{
	char	*oldpwd;
	char	*pwd;
	char	*path;

	if (!shell || !cmd)
		return ;
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		return ;
	path = get_cd_path(shell, cmd);
	if (!path)
	{
		free(oldpwd);
		return ;
	}
	if (chdir(path) != 0)
	{
		free(oldpwd);
		return ;
	}
	pwd = getcwd(NULL, 0);
	update_env(shell, "OLDPWD", oldpwd);
	update_env(shell, "PWD", pwd);
	free(oldpwd);
	free(pwd);
}

void	builtin_exit(t_shell *shell, t_cmd *cmd)
{
	if (!shell || !cmd)
		return ;
	ft_putendl_fd("exit", 2);
	if (cmd->args[1])
		exit(ft_atoi(cmd->args[1]));
	else
		exit(shell->exit_status);
}

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

// For each NAME, remove the corresponding variable or function. Shift everything
// left by one position then sets last entry to NULL. No reallocation of array.
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
