/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luimarti <luimarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 23:58:04 by dachin            #+#    #+#             */
/*   Updated: 2026/06/02 18:03:43 by luimarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static	int	do_cd(t_shell *shell, char *path)
{
	char	*oldpwd;
	char	*pwd;

	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		return (1);
	if (chdir(path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		free(oldpwd);
		return (1);
	}
	pwd = getcwd(NULL, 0);
	update_env(shell, "OLDPWD", oldpwd);
	update_env(shell, "PWD", pwd);
	free(oldpwd);
	free(pwd);
	return (0);
}

void	builtin_cd(t_shell *shell, t_cmd *cmd)
{
	char	*path;

	if (!shell || !cmd)
		return ;
	if (cmd->args[1] && cmd->args[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		shell->exit_status = 1;
		return ;
	}
	path = get_cd_path(shell, cmd);
	if (!path)
	{
		shell->exit_status = 1;
		return ;
	}
	shell->exit_status = do_cd(shell, path);
}
/* FIX: Added error handling for failed chdir() and too many arguments.
** Previously, cd silently returned exit code 0 on failure. Now it prints
** the appropriate error message and sets shell->exit_status, matching
** bash behaviour. Extracted do_cd() to handle the actual directory change
** and environment update, keeping builtin_cd() within norminette limits.
** Function was split into do_cd() and builtin_cd() to comply with
** norminette 25-line limit. */

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

void	builtin_unset(t_shell *shell, t_cmd *cmd)
{
	char	*key;
	int		index;

	if (!shell || !cmd)
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
