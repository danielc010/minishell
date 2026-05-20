/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: dachin <dachin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/05/20 23:39:07 by dachin        #+#    #+#                 */
/*   Updated: 2026/05/21 00:32:52 by dachin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	replace_env_entry(char **entry, char *newval)
{
	char	*tmp;

	tmp = *entry;
	*entry = newval;
	free(tmp);
}

void	update_env(t_shell *shell, char *key, char *value)
{
	int		i;
	char	*keyequal;
	char	*keyequalvalue;
	int		found;

	if (!shell || !key || !value)
		return ;
	keyequal = ft_strjoin(key, "=");
	keyequalvalue = ft_strjoin(keyequal, value);
	found = 0;
	i = 0;
	while (shell->env[i])
	{
		if (!ft_strncmp(shell->env[i], keyequal, ft_strlen(keyequal)))
		{
			found = 1;
			replace_env_entry(&shell->env[i], keyequalvalue);
			break ;
		}
		++i;
	}
	free(keyequal);
	if (!found)
		free(keyequalvalue);
}

char	*get_env(t_shell *shell, char *key)
{
	int		i;
	char	*keyequal;

	if (!shell || !key)
		return (NULL);
	keyequal = ft_strjoin(key, "=");
	i = 0;
	while (shell->env[i])
	{
		if (!ft_strncmp(shell->env[i], keyequal, ft_strlen(keyequal)))
		{
			free(keyequal);
			return (shell->env[i] + ft_strlen(keyequal));
		}
		++i;
	}
	free(keyequal);
	return (NULL);
}

char	*get_cd_path(t_shell *shell, t_cmd *cmd)
{
	char	*path;

	if (!cmd->args[1])
		path = get_env(shell, "HOME");
	else if (!ft_strncmp(cmd->args[1], "-", 2))
		path = get_env(shell, "OLDPWD");
	else
		path = cmd->args[1];
	return (path);
}

void	builtin_cd(t_shell *shell, t_cmd *cmd)
{
	char	*oldpwd;
	char	*pwd;
	char	*path;

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
